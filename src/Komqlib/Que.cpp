/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) Que.C 1.3@(#)
 */
//---------------------------------------------------------------------+
//  File:     Que.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CQue 
//            Class.
//
//            This class represents an MQ Series Queue.
//  
//  Dev Log: 
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#include "Que.h"





//
// Constants that define how a Queue is opened
//
const int CQue::Output		= 0x01;
const int CQue::Exclusive	= 0x02;
const int CQue::Input		= 0x04;
const int CQue::Browse		= 0x08;




//
// constants used to define the type of Get to perform
//
const char CQue::doBrowseFirst		= 'F';
const char CQue::doBrowseNext		= 'N';
const char CQue::doGetNext			= 'G';
const char CQue::doGetUnderCursor	= 'U';







//---------------------------------------------------------------------+
//  Method:   InitVars
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Initializes Member Variables.  Called from multiple
//            versions of the Constructor.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/21/99 DMT Intializing Queue Name to NULLs too.
//  01/08/00 DRC Added Return Receipt variables.
//---------------------------------------------------------------------+
void CQue::InitVars()
{

	is_InstanceName		= "Queue";

	m_lExpiration		= KO_MAX_EXPIRY;
	m_bIsOpen			= FALSE;
	m_lMsgLen			= 0;
	m_iQueHandle		= -1;
	m_lBatchInterval	= 0;
	m_bSyncpointEnable	= FALSE;
	m_bPartialMsgs		= FALSE;
	m_lPersistence		= KO_DEFAULT_PERSISTENCE;	// Persistent
	m_lMsgType			= MQMT_DATAGRAM;
	m_lFeedbackCode		= 0;
	m_lReportOptions	= 0;
	m_iFormatSize		= FORMAT_LEN - 1;
	m_cGetMode			= 'X';
	m_lWaitInterval		= KO_DEFAULT_WAIT_INTERVAL;
	m_lReqDataLen		= 0;
	m_lDeadReason		= 0;
	m_bMatchMsgID		= FALSE;
	m_bMatchCorlID		= FALSE;
	m_lPriority         = KO_DEFAULT_PRIORITY;
	m_bFailIfQuiescing  = FALSE;
	m_bXlateKeyValues   = FALSE;
#ifdef _ver2_
	m_bLogReceipt       = FALSE;
	m_bDataReceipt      = FALSE;
#endif

	//-----------------------------------------------------------------+
	// Note there are two lengths for these char arrays.
	// The allocated space and the actual defined length needed.
	// To keep the data attributes of the class aligned on 32 bit
	// boundries, they are extended by one char for null termination 
	// and then sized up to the nearest divisor of 4.
	//
	// Thus the MSG_ID_LEN will be something smaller than the
	// MSG_ID_ALLOC.
	//-----------------------------------------------------------------+
	memset(m_szMsgID,		'\0', MSG_ID_ALLOC);
	memset(m_szCorlID,		'\0', CORL_ID_ALLOC);
	memset(m_szCorlID,		'0',  CORL_ID_LEN);
	memset(m_szReplyToQue,	'\0', REPLY_TO_QUE_ALLOC);
	memset(m_szFormat,		'\0', FORMAT_ALLOC);
	memset(m_szName,        '\0', QUEUE_NAME_ALLOC); // DMT 02/21/99

	strcpy (m_szFormat, KO_USE_DEFAULT_FORMAT);


}








//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Class Initialization
//
//  Args:     ptr to CLog    - Log Class
//            ptr to CQueMgr - Queue Manager Class
//            char*          - Queue Name
//            BOOL           - True if Puts and Gets should Fail if
//                             Queue Manager is Quiescing.
//            BOOL           - True if Message and Correl IDs should
//                             be converted from the HOST character set 
//                             into a Neutral Binary representation on 
//                             Gets and vice versa on Puts.
//            BOOL           - True if Processing Logs are to be returned 
//                             as a processing receipt (PUT only).
//            BOOL           - True if Structured Data from the log is to be
//                             returned as a processing receipt (PUT only).
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//  01/08/00  DRC Added Return Receipt variables.
//---------------------------------------------------------------------+
long CQue::initSelf(CLog* vp_Log, CQueMgr* v_pcQM, char* szName,
#ifdef _ver2_
					BOOL vb_fail,        BOOL vb_xlate,
                    BOOL vb_LogReceipt,  BOOL vb_DataReceipt) {
#else
					BOOL vb_fail,        BOOL vb_xlate) {
#endif

	long ll_return = KOCC_GOOD;

	ll_return = CAny2::initSelf();

	if (ll_return == KOCC_GOOD) {
		
		strncpy(m_szName, szName, QUEUE_NAME_LEN);

		m_pcLog = vp_Log;

		if (m_pcLog == NULL) {

			iu_errors.AddError(KORC_NULL_LOG_POINTER, KOCC_FAIL, 
				"NULL Log Pointer",
				"CQue::initSelf:1",
				is_InstanceName.getPointer());

			ll_return = KOCC_FAIL;
		}


		m_pcQueMgr = v_pcQM;

		if (m_pcQueMgr == NULL) {

			iu_errors.AddError(KORC_NULL_QM_POINTER, KOCC_FAIL, 
				"NULL Queue Manager Pointer",
				"CQue::initSelf:2",
				is_InstanceName.getPointer());

			ll_return = KOCC_FAIL;
		}

		m_bFailIfQuiescing = vb_fail;
		m_bXlateKeyValues  = vb_xlate;
#ifdef _ver2_
        m_bLogReceipt      = vb_LogReceipt;
        m_bDataReceipt     = vb_DataReceipt;
#endif
	}

	return ll_return;
}





#ifdef _ver2_
//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   Daniel R Creger
//  Written:  January 2000
//
//  Desc:     Class Initialization
//
//  Args:     refer to 7 parm version
//
//  Returns:  refer to 7 parm version
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//  01/08/00  DRC Added 5 parm method for upward compatability.
//---------------------------------------------------------------------+
long CQue::initSelf(CLog* vp_Log, CQueMgr* v_pcQM, char* szName,
					BOOL vb_fail, BOOL vb_xlate) {

	return initSelf(vp_Log,  v_pcQM,   szName,
					vb_fail, vb_xlate, FALSE,  FALSE);
}
#endif



//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     ptr to CQueMgr class - Queue Manager
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CQue::CQue(CQueMgr* v_pcQM)
{
	m_pcQueMgr = v_pcQM;
	InitVars();

}






//---------------------------------------------------------------------+
//  Method:   Constructor (no parms)
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
CQue::CQue(){

	m_pcQueMgr = NULL;
	InitVars();

}



	





//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CQue::~CQue()
{
	//delete[] m_szBuff;
}






//---------------------------------------------------------------------+
//  Method:   QOpen
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Sets the Queue Name and opens the Queue using the 
//            specified open options.
//
//  Args:     char* Queue Name
//            int	Open Options - can be CQue::Output,
//                                        CQue::Input,   (the default)
//                                        CQue::Exclusive,
//                                        CQue::Browse,
//                                      or more than 1 "OR'ed" together.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::QOpen(const char* Name, const int mode)
{
	strcpy(m_szName, Name);

	return QOpen(mode);
}








//---------------------------------------------------------------------+
//  Method:   QOpen
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     opens the Queue using the 
//            specified open options.
//
//  Args:     int	Open Options - can be CQue::Output,
//                                        CQue::Input,   (the default)
//                                        CQue::Exclusive,
//                                        CQue::Browse,
//                                      or more than 1 "OR'ed" together.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  04/19/99 DMT Open for Shared Input was always on, regardless of
//               the other options specified.
//---------------------------------------------------------------------+
long CQue::QOpen(const int mode)
{
	CString	ls_msg;
	long	rc;

	rc = m_pcQueMgr->InitQ( m_iQueHandle, 
					(char*)m_szName, 
//DMT 4/19/99		0 == (mode & Exclusive), 
					Input == (mode & Input), 
					Exclusive == (mode & Exclusive),
					Browse == (mode & Browse),
					Output == (mode & Output));

	if (0 != rc) {

		ls_msg << "Error opening Queue '"
			   << m_szName
			   << "'.";

		m_pcLog->LogError(KORC_QUEUE_ERROR, rc,
						  ls_msg,
						  "CQue::QOpen::1",
						  is_InstanceName.getPointer());
	}
	else {
		m_bIsOpen = TRUE;
	}

	return rc;
}








//---------------------------------------------------------------------+
//  Method:   BrowseFirst
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Front-End Method for GetMsg.  Use to Browse the First
//            Msg matching specified Msg ID and Correl ID.
//
//            Synchpoint (aka Batch Indicator) Option is Forced to OFF.  
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//            char*         Message ID.  Matches provided Msg ID unless
//                                       NULL is specified.
//
//            ulong         Correl. ID.  Matches provided Correl ID 
//                                       unless 0 is specified.
//            
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::BrowseFirst(CBuffer& cBuffer, 
					   const char *szMsgID, 
					   ulong ulCorlID)
{
	m_bSyncpointEnable = FALSE;
	return GetMsg(cBuffer, doBrowseFirst, szMsgID, ulCorlID);
}







//---------------------------------------------------------------------+
//  Method:   BrowseNext
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Front-End Method for GetMsg.  Use to Browse the Next
//            Msg matching specified Msg ID and Correl ID.
//
//            Synchpoint (aka Batch Indicator) Option is Forced to OFF.  
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//            char*         Message ID.  Matches provided Msg ID unless
//                                       NULL is specified.
//
//            ulong         Correl. ID.  Matches provided Correl ID 
//                                       unless 0 is specified.
//            
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::BrowseNext(CBuffer& cBuffer)
{
	m_bSyncpointEnable = FALSE;
	return GetMsg(cBuffer, doBrowseNext);
}








//---------------------------------------------------------------------+
//  Method:   GetUnderCursor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Front-End Method for GetMsg.  Use to Get the Last Message
//            Browsed.
//
//            Synchpoint (aka Batch Indicator) Option is Forced to ON.  
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::GetUnderCursor(CBuffer& cBuffer)
{
	m_bSyncpointEnable = TRUE;
	return GetMsg(cBuffer, doGetUnderCursor);
}










//---------------------------------------------------------------------+
//  Method:   GetNext
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Front-End Method for GetMsg.  Use to Get the Next
//            Msg matching specified Msg ID and Correl ID.
//
//            Synchpoint (aka Batch Indicator) Option is Forced to ON.  
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//            char*         Message ID.  Matches provided Msg ID unless
//                                       NULL is specified. (default)
//
//            ulong         Correl. ID.  Matches provided Correl ID 
//                                       unless 0 is specified.
//                                       (default)
//            
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::GetNext(CBuffer& cBuffer, 
				   const char *szMsgID, 
				   ulong ulCorlID)
{
	m_bSyncpointEnable = TRUE;
    return GetMsg(cBuffer, doGetNext, szMsgID, ulCorlID);
}








//---------------------------------------------------------------------+
//  Method:   GetMsg
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Get Method.  
//
//            Use one of the front end methods to set the "Get Mode" 
//            and set the Synchpoint Option.
//
//            *** NOTE!!! ***
//            This method does not set the Synchpoint Option and uses
//            it's last state.  Be sure to use the "SetSyncpointEnable"
//            method to explicitly specify the Synchpoint Option.
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//            GetMode:      doBrowseFirst		= 'F';
//                          doBrowseNext		= 'N';
//                          doGetNext			= 'G'; (default)
//                          doGetUnderCursor	= 'U';
//
//            char*         Message ID.  Matches provided Msg ID unless
//                                       NULL is specified. (default)
//
//            ulong         Correl. ID.  Matches provided Correl ID 
//                                       unless 0 is specified.
//                                       (default)
//            
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//
//  03/04/99 DMT Found bug. Not matching Msg ID when should because
//               comparison operator was used (==) instead of 
//               assignment operator (=).  Added braces too for
//               clarity.
//---------------------------------------------------------------------+
long CQue::GetMsg(CBuffer& cBuffer, const char cGetMode, 
				  const char* szMsgID, ulong ulCorlID)
{
	long 	rc = KOCC_GOOD;
	CString	ls_msg;

	if (NULL == szMsgID)
	{								// DMT 03/04/99
		//m_bMatchMsgID == FALSE;	// DMT 03/04/99
		m_bMatchMsgID = FALSE;
	}								// DMT 03/04/99
	else
	{
		//m_bMatchMsgID == TRUE;	// DMT 03/04/99
		m_bMatchMsgID = TRUE;
		strncpy(m_szMsgID, szMsgID, sizeof(m_szMsgID)-1);
	}




	if (0 == ulCorlID)
	{								// DMT 03/04/99
		m_bMatchCorlID = FALSE;
	}								// DMT 03/04/99
	else
	{
		sprintf(m_szCorlID,"%0*ld", MSG_ID_LEN, ulCorlID);
		m_bMatchCorlID = TRUE;
	}




	m_cGetMode = cGetMode;

	//
	// NOTE: I added a bunch of member variables to the class
	// to complete the API call.
	//
	rc = m_pcQueMgr->Get(m_iQueHandle,// Handle to this Que
				cBuffer.getPointer(),// Pointer to message store
				cBuffer.getSize(),	// size of message store
				m_lMsgLen,			// size of the message returned.
				m_lExpiration,		// Expiration Interval of Msg
				m_bSyncpointEnable,	// Syncpoint indication
				m_bPartialMsgs,		// accept partial messages?
				m_lPersistence,		// persist indicator
				m_szMsgID,			// message id
				m_szCorlID,			// Correllation ID
				m_szReplyToQue,		// The reqly Que
				m_lMsgType,			// Message Type
				m_lFeedbackCode,	// The feedback code.
				m_lReportOptions,	// 
				m_szFormat,			// Message Format
				m_cGetMode,			// Get mode.
				m_lWaitInterval,	// How long to wait.
				m_bMatchMsgID,		// match the message ID
				m_bMatchCorlID,  	// match the correllation ID
				m_lPriority,
				m_bFailIfQuiescing,
				m_bXlateKeyValues);

	if (rc > KOCC_WARNING)
	{

		ls_msg << "Error receiving message from Queue '"
			   << m_szName
			   << "'.";

		m_pcLog->LogError(KORC_QUEUE_ERROR, rc,
						  ls_msg,
						  "CQue::GetMsg:1",
						  is_InstanceName.getPointer());
	}


	if (rc == KOCC_GOOD) {

		if (isdec(m_szCorlID)) {
			m_lCorlID = atol(m_szCorlID);
		}
		else {
			rc = KOCC_FAIL;
	
			ls_msg << "Correllation ID '" << m_szCorlID 
				   << "' is not numeric.";

			m_pcLog->LogError(KORC_INVALID_CORREL_ID, rc,
					ls_msg,
		  			"CQue::GetMsg:1",
  					is_InstanceName.getPointer());
		}
	}



	return rc;
}










//---------------------------------------------------------------------+
//  Method:   GetMsgA
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Get Message Method that calls the Administrative Get
//            in CQueMgr and thus in the API.  The Admin version of
//            Get bypasses standard KO Edits/Defaults and allows
//            the retrieval of "Dead Letter" messages.
//
//            *** NOTE!!! ***
//            This method does not set the Synchpoint Option and uses
//            it's last state.  Be sure to use the "SetSyncpointEnable"
//            method to explicitly specify the Synchpoint Option.
//
//  Args:     ref CBuffer	Buffer Class to receive Msg Data
//
//            GetMode:      doBrowseFirst		= 'F';
//                          doBrowseNext		= 'N';
//                          doGetNext			= 'G'; (default)
//                          doGetUnderCursor	= 'U';
//
//            char*         Message ID.  Matches provided Msg ID unless
//                                       NULL is specified. (default)
//
//            ulong         Correl. ID.  Matches provided Correl ID 
//                                       unless 0 is specified.
//                                       (default)            
//
//  Returns:  0 if good, 1 if a warning occurs, or 2 if an error.
//  
//  Dev Log:  
//  History: 
//  04/04/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetMsgA(CBuffer& cBuffer, const char cGetMode, 
				  const char* szMsgID, ulong ulCorlID)
{
	long 	rc = KOCC_GOOD;
	CString	ls_msg;

	if (NULL == szMsgID)
	{
		m_bMatchMsgID = FALSE;
	}
	else
	{
		m_bMatchMsgID = TRUE;
		strncpy(m_szMsgID, szMsgID, sizeof(m_szMsgID)-1);
	}




	if (0 == ulCorlID)
	{
		m_bMatchCorlID = FALSE;
	}
	else
	{
		sprintf(m_szCorlID,"%0*ld", MSG_ID_LEN, ulCorlID);
		m_bMatchCorlID = TRUE;
	}




	m_cGetMode = cGetMode;

	rc = m_pcQueMgr->GetA(m_iQueHandle,// Handle to this Que
				cBuffer.getPointer(),// Pointer to message store
				cBuffer.getSize(),	// size of message store
				m_lMsgLen,			// size of the message returned.
				m_lExpiration,	    // Expiration Interval
				m_bSyncpointEnable,	// Syncpoint indication
				m_bPartialMsgs,		// accept partial messages?
				m_lPersistence,		// persist indicator
				m_szMsgID,			// message id
				m_szCorlID,			// Correllation ID
				m_szReplyToQue,		// The reqly Que
				m_lMsgType,			// Message Type
				m_lFeedbackCode,	// The feedback code.
				m_lReportOptions,	// 
				m_szFormat,			// Formatting string
				m_cGetMode,			// Get mode.
				m_lWaitInterval,	// How long to wait.
				m_bMatchMsgID,		// match the message ID
				m_bMatchCorlID,	// match the correllation ID
				m_lPriority,
				m_bFailIfQuiescing,
				m_bXlateKeyValues);

	if (rc > KOCC_WARNING)
	{

		ls_msg << "Error receiving message from Queue '"
			   << m_szName
			   << "'.";

		m_pcLog->LogError(KORC_QUEUE_ERROR, rc,
						  ls_msg,
						  "CQue::GetMsg:1",
						  is_InstanceName.getPointer());
	}


	if (rc == KOCC_GOOD) {

		if (isdec(m_szCorlID)) {
			m_lCorlID = atol(m_szCorlID);
		}
		else {
			rc = KOCC_FAIL;
	
			ls_msg << "Correllation ID '" << m_szCorlID 
				   << "' is not numeric.";

			m_pcLog->LogError(KORC_INVALID_CORREL_ID, rc,
					ls_msg,
		  			"CQue::GetMsg:1",
  					is_InstanceName.getPointer());
		}
	}



	return rc;
}







//---------------------------------------------------------------------+
//  Method:   GetAttrLng
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Retrieves selected attributes from the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  Long attribute value.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long CQue::GetAttrLng( int ai_Type){
return m_pcQueMgr->GetAttrLng(ai_Type);
}






//---------------------------------------------------------------------+
//  Method:   GetAttrStr
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Retrieves selected attributes from the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  Pointer to the string attribute value.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
char* CQue::GetAttrStr( int ai_Type){
return m_pcQueMgr->GetAttrStr(ai_Type);
}






//---------------------------------------------------------------------+
//  Method:   setAttrLng
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Sets the value for a selected attributes in the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long  CQue::SetAttrLng( int ai_Type, long al_Arg){
return m_pcQueMgr->SetAttrLng(ai_Type, al_Arg);
}




//---------------------------------------------------------------------+
//  Method:   setAttrStr
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Sets the value for a selected attributes in the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long  CQue::SetAttrStr( int ai_Type, char* as_Arg){
return m_pcQueMgr->SetAttrStr(ai_Type, as_Arg);
}





//---------------------------------------------------------------------+
//  Method:   Good
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns TRUE if the last Operation against MQ Series API
//            was successful.
//
//  Args:     None
//
//  Returns:  TRUE if the last MQ Series operation was had a completion
//            code of 0 (good).  FALSE otherwise.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
BOOL CQue::Good()
{

	return 0 == m_pcQueMgr->GetLastReasonCode();
}











//---------------------------------------------------------------------+
//  Method:   GetMsgTextLen
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns the length of a message that was just "gotten".
//
//  Args:     None
//
//  Returns:  Long, length of message data.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::GetMsgTextLen() const
{
	return m_lMsgLen;
}






//---------------------------------------------------------------------+
//  Method:   GetMsgID
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns the Message ID of the last message "gotten".
//
//  Args:     None
//
//  Returns:  Ptr to character string containing Msg ID.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CQue::GetMsgID() const
{
	return m_szMsgID;
}






//---------------------------------------------------------------------+
//  Method:   GetCorlID
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns Pointer to character string containing 
//            the Correllation ID of the message just "gotten".
//
//  Args:     None
//
//  Returns:  pointer to character string containing Correl ID.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char * CQue::GetCorlID() const
{
	return m_szCorlID;
}






//---------------------------------------------------------------------+
//  Method:   GetCorlIDBin
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets Numeric version of last Correllation ID.
//
//  Args:     None
//
//  Returns:  ulong - numeric representation of last Correl ID.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const ulong CQue::GetCorlIDBin() const
{
	return m_lCorlID;
}






//---------------------------------------------------------------------+
//  Method:   SetMsgID
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Lets you set Message ID, before Put.
//
//  Args:     ptr to character string containing Message ID.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CQue::SetMsgID(const char* szMsgID)
{
	if (NULL == szMsgID) {

		m_pcLog->LogError(KORC_INVALID_MSG_ID, KOCC_FAIL, 
			"NULL Message ID",
			"CQue::SetMsgID:1",
			is_InstanceName.getPointer());

		//throw CExcBadMsgID("NULL Message ID", *this);
	}

	sprintf(m_szMsgID,"%-*.*s",MSG_ID_LEN,MSG_ID_LEN,szMsgID);
}







//---------------------------------------------------------------------+
//  Method:   SetCorlID
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Sets Correllation ID.  Converts provided numeric 
//            represenation to string for internal storage.
//
//  Args:     ulong Correllation ID.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CQue::SetCorlID(const ulong ulCorlID)
{
	m_lCorlID = ulCorlID;
	sprintf(m_szCorlID,"%0*d",CORL_ID_LEN, m_lCorlID);

}






//---------------------------------------------------------------------+
//  Method:   Put
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Put - simplified. Only takes a pointer to a message 
//                  string.  Get length by calling strlen. String must 
//                  be smaller than the buffer size.
//
//            NOTE - Synchpoint Option always on.  All PUTs subject
//                   to COMMIT/ROLLBACK.
//
//  Args:     ref CBuffer - Buffer containing Message Data.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  Currently, this method always turns Synchpoint option on.
//            Not sure why this is needed and it's probably just an
//            artifact from Chris Hamilton's code.
//
//            If we enable an "Error Queue" to log status, we'll 
//            probably want to put these messages on a queue without
//            the synchpoint option.  If this is done in the future,
//            we will need a way to PUT and control the synchpoint
//            option.
//
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::Put(CBuffer& cBuffer)
{
	int rc = 0;
	CString	ls_msg;

	m_bSyncpointEnable = TRUE;
	
	if (NULL == cBuffer.getPointer()) {

		m_pcLog->LogError(KORC_NULL_BUFFER_POINTER, KOCC_FAIL, 
			"NULL message pointer!",
			"CQue::Put:1", 
			is_InstanceName.getPointer());

		return KOCC_FAIL;
	}

	rc = m_pcQueMgr->Put(m_iQueHandle,// Handle to this Que
				cBuffer.getPointer(), // Pointer to message store
				strlen(cBuffer.getPointer()),	// size of the message put.
				m_lExpiration,		// Message expiration value
				m_bSyncpointEnable,	// Syncpoint indication
				m_lPersistence,		// persist indicator
				m_szMsgID,			// message id
				m_szCorlID,			// Correllation ID
				m_szReplyToQue,		// The reqly Que
				m_lReportOptions,	// 
				m_lMsgType,			// Message Type
				m_lFeedbackCode,  	// The feedback code.
				m_szFormat,
				m_lPriority,
				m_bFailIfQuiescing,
#ifdef _ver2_
				m_bXlateKeyValues, 
                                m_bLogReceipt, 
                                m_bDataReceipt);
#else
				m_bXlateKeyValues);
#endif

	if (0 != rc) {

		ls_msg << "Error putting message onto Queue '"
			   << m_szName
			   << "'.";

		m_pcLog->LogError(KORC_QUEUE_ERROR, rc,
						  ls_msg,
						  "CQue::Put:1",
						  is_InstanceName.getPointer());
	}

	return rc;
}








//---------------------------------------------------------------------+
//  Method:   PutA
//  Author:   No one
//  Written:  Hasn't been
//
//  Desc:     The Administrative PUT counterpart to CQueueMgr::PutA
//            has not been implemented in this class.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  Just a note that this hasn't been implemented here.
//  History: 
//  00/00/00  xxx Created original version.
//---------------------------------------------------------------------+







//---------------------------------------------------------------------+
//  Method:   GetReason
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets the Last Reason Code of the last MQ Series 
//            operation.
//
//  Args:     None
//
//  Returns:  long - last Reason code.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::GetReason()
{
	return m_pcQueMgr->GetLastReasonCode();
}






//---------------------------------------------------------------------+
//  Method:   GetCompCode
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets the last Completion Code of the last MQ Series
//            operation.
//
//  Args:     None
//
//  Returns:  long - last Completion Code.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::GetCompCode()
{
	return m_pcQueMgr->GetLastCompCode();
}






//---------------------------------------------------------------------+
//  Method:   IsOpen
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns TRUE if the Queue is open.
//
//  Args:     None
//
//  Returns:  see above.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
BOOL CQue::IsOpen()
{
	return m_bIsOpen;
}






//---------------------------------------------------------------------+
//  Method:   Close
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Closes the Queue.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQue::Close()
{
	long	ll_return	= KOCC_GOOD;
	CString	ls_msg;

	//
	// If the queue is not yet open, the handle will be -1
	// and it doesn't need to be closed.
	//
	if (-1 == m_iQueHandle) {
		ll_return = KOCC_GOOD;
	}
	else {

		ll_return = m_pcQueMgr->QClose(m_iQueHandle);

		if (ll_return != KOCC_GOOD) {

			ls_msg << "Error closing Queue '"
				   << m_szName
				   << "'.";

			m_pcLog->LogError(KORC_QUEUE_ERROR, ll_return,
							  ls_msg,
							  "CQue::GetMsg:1",
							  is_InstanceName.getPointer());
		}

	}

	return ll_return;

}




//
//
//---------------------------------------------------------------------+
//---------------------------------------------------------------------+
//  Note:  The remaining methods are Get/Set methods for private
//         member variables.  The doc-blocks are not complete,
//         except where needed.
//---------------------------------------------------------------------+
//---------------------------------------------------------------------+
//
//





//---------------------------------------------------------------------+
//  Method:   GetExpiration
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Gets the Expiration Interval of the last Message.
//
//  Args:     None
//
//  Returns:  long - Expiration Interval
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetExpiration() {
	return m_lExpiration;
}



//---------------------------------------------------------------------+
//  Method:   SetExpiration
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Sets the expiration interval
//
//  Args:     long	Expiration Interval.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetExpiration(long	vl_value) {
	m_lExpiration = vl_value;
}








//---------------------------------------------------------------------+
//  Method:   GetReplyToQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
char* CQue::GetReplyToQueueName() {
	return m_szReplyToQue;
}



//---------------------------------------------------------------------+
//  Method:   SetReplyToQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetReplyToQueueName(char* szName) {

	if (NULL == szName) {

		m_pcLog->LogError(KORC_INVALID_REPLY_Q_NAME, KOCC_FAIL, 
			"NULL ReplyTo Queue Name",
			"CQue::SetReplyToQueueName:1",
			is_InstanceName.getPointer());
	}

	// Copy Value, left-aligning and right padding with " "
	// up to Que_Len width.
	sprintf(m_szReplyToQue,"%-*.*s",REPLY_TO_QUE_LEN, 
		                            REPLY_TO_QUE_LEN, szName);
}









//---------------------------------------------------------------------+
//  Method:   GetMsgFormat
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
char* CQue::GetMsgFormat() {
	return m_szFormat;
}




//---------------------------------------------------------------------+
//  Method:   SetMsgFormat
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetMsgFormat(char* szFormat) {

	if (NULL == szFormat) {

		m_pcLog->LogError(KORC_INVALID_MSG_FORMAT, KOCC_FAIL, 
			"NULL Message Format",
			"CQue::SetMsgFormat:1",
			is_InstanceName.getPointer());
	}

	// Copy Value, left-aligning and right padding with " "
	// up to FORMAT_LEN width.
	sprintf(m_szFormat,"%-*.*s",FORMAT_LEN, 
		                        FORMAT_LEN, szFormat);


}










//---------------------------------------------------------------------+
//  Method:   GetWaitInterval
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetWaitInterval() {
	return m_lWaitInterval;
}


//---------------------------------------------------------------------+
//  Method:   SetWaitInterval
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetWaitInterval(long	vl_value) {
	m_lWaitInterval = vl_value;
}









//---------------------------------------------------------------------+
//  Method:   GetMsgType
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetMsgType() {
	return m_lMsgType;
}



//---------------------------------------------------------------------+
//  Method:   SetMsgType
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetMsgType(long vl_value) {
	m_lMsgType = vl_value;
}










//---------------------------------------------------------------------+
//  Method:   GetPersistence
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetPersistence() {
	return m_lPersistence;
}



//---------------------------------------------------------------------+
//  Method:   SetPersistence
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetPersistence(long vl_value) {
	m_lPersistence = vl_value;
}










//---------------------------------------------------------------------+
//  Method:   GetFeedbackCode
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetFeedbackCode() {
	return m_lFeedbackCode;
}



//---------------------------------------------------------------------+
//  Method:   SetFeedbackCode
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetFeedbackCode(long vl_value) {
	m_lFeedbackCode = vl_value;
}












//---------------------------------------------------------------------+
//  Method:   GetReportOptions
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetReportOptions() {
	return m_lReportOptions;
}



//---------------------------------------------------------------------+
//  Method:   SetReportOptions
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetReportOptions(long vl_value) {
	m_lReportOptions = vl_value;
}











//---------------------------------------------------------------------+
//  Method:   GetDeadReason
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  This function is probably obsolete because normal get
//            doesn't return Dead Reason Code.
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetDeadReason() {
	return m_lDeadReason;
}








//---------------------------------------------------------------------+
//  Method:   GetPriority
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQue::GetPriority() {
	return m_lPriority;
}




//---------------------------------------------------------------------+
//  Method:   SetPriority
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetPriority(long vl_value) {
	m_lPriority = vl_value;
}









//---------------------------------------------------------------------+
//  Method:   GetFailIfQuiescing
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
BOOL CQue::GetFailIfQuiescing() {
	return m_bFailIfQuiescing;
}




//---------------------------------------------------------------------+
//  Method:   SetFailIfQuiescing
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetFailIfQuiescing(BOOL vb_value) {
	m_bFailIfQuiescing = vb_value;
}














//---------------------------------------------------------------------+
//  Method:   GetTranslate
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
BOOL CQue::GetTranslate() {
	return m_bXlateKeyValues;
}



//---------------------------------------------------------------------+
//  Method:   SetTranslate
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  Note - ASCII/EBCDIC Translation is not implemented in
//                   the API, but all the parameters for it are in
//                   place.
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetTranslate(BOOL vb_value) {
	m_bXlateKeyValues = vb_value;
}












//---------------------------------------------------------------------+
//  Method:   GetSyncpointEnable
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
BOOL CQue::GetSyncpointEnable() {
	return m_bSyncpointEnable;
}




//---------------------------------------------------------------------+
//  Method:   SetSyncpointEnable
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Sets the SynchPoint (aka Batch Indicator) option to 
//            do Gets with or without Commits/Rollbacks.
//
//            The "Get Front End" methods set this appropriately,
//            but you must use this if you call "GetMsg" or "GetMsgA"
//            directly.  GetMsg and GetMsgA do not set this value
//            and use whatever the last state was.
//
//            Note that the current version of PUT always sets
//            Syncpoint to TRUE.
//
//  Args:     TRUE  - enable commits rollbacks
//            FALSE - Get is immediate.
//
//  Returns:  Nothing
//  
//  Dev Log:  See note on Put Method.  Will need to change Put to use
//            established Synchpoint option when we want to implement
//            Status logging to a Queue.
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
void CQue::SetSyncpointEnable(BOOL vb_value) {
	m_bSyncpointEnable = vb_value;
}
