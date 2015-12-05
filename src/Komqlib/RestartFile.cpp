/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) RestartFile.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     RestartFile.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CRestartFile
//            Class.
//
//            This class represents a Restart File and is derived from
//            CParamFile.  It's a sibling to CStartParamFile.
//  
//  Dev Log:  CorrelID member to store CorrelID of control 
//            msg.  For Put processing,this would be expected count + 1
//
//
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//  02/12/00 DRC Added the show method
//---------------------------------------------------------------------+
#include <stdlib.h>
#include <fstream.h>
#include "RestartFile.h"


char* CRestartFile::m_szMsgID;
ulong CRestartFile::m_lDataMsgs;
ulong CRestartFile::m_lCorlID;
ulong CRestartFile::m_lExpected;
ulong CRestartFile::m_lCtlMsgs;
#ifdef _ver2_
char* CRestartFile::m_szRestartKey;
#else
CString CRestartFile::m_sRestartKey;
#endif


//---------------------------------------------------------------------+
//  Table that defined parameters in Restart File
//
//	Keyword, Type, Pointer to Member Var, Required, Loaded
//
//	NULL-TypeInvalid entry indicates end of table.
//---------------------------------------------------------------------+
CRestartFile::SParamTable CRestartFile::sParamTable[] =
{
	{	"ControlMessagesProcessed",	TypeLong,    &m_lCtlMsgs,     TRUE, FALSE },
	{	"DataMessagesRead",		TypeLong,    &m_lDataMsgs,    TRUE, FALSE },
	{	"CorrelationID",			TypeLong,    &m_lCorlID,      TRUE, FALSE },
	{	"MessageID",			TypeString,  &m_szMsgID,      TRUE, FALSE },
	{     "ExpectedMessageCount",       TypeLong,    &m_lExpected,    TRUE, FALSE },
#ifdef _ver2_
	{     "RestartKey",                 TypeString,  &m_szRestartKey, TRUE, FALSE },
#else
	{     "RestartKey",                 TypeString,  &m_sRestartKey,  TRUE, FALSE },
#endif
	{     NULL,					TypeInvalid, NULL,	      TRUE, FALSE }
};




//---------------------------------------------------------------------+
//  Method:   GetParamTable
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets Pointer to the Parameter Table
//
//  Args:     None
//
//  Returns:  Pointer to Parameter Table
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const CRestartFile::SParamTable* CRestartFile::GetParamTable()
{
	return sParamTable;
}






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Initializes Member variables.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CRestartFile::CRestartFile() : CParamFile()
{
	m_szMsgID		= NULL;
	m_lDataMsgs		= 0;
	m_lCorlID		= 0;
	m_lCtlMsgs		= 0;
	m_lExpected		= 0;
	is_InstanceName = "Restart Parameter File";
#ifdef _ver2_
	SetString(&m_szRestartKey, "000000000000");
#else
	m_sRestartKey   = "000000000000";
#endif


}






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Initializes Member Variables
//
//  Args:     char*  - File Name
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CRestartFile::CRestartFile(const char* filename) : CParamFile(filename)
{
	m_szMsgID		= NULL;
	m_lDataMsgs		= 0;
	m_lCorlID		= 0;
	m_lCtlMsgs		= 0;
	m_lExpected		= 0;
	is_InstanceName = "Restart Parameter File";
#ifdef _ver2_
	SetString(&m_szRestartKey, "000000000000");
#else
	m_sRestartKey    = "000000000000";
#endif

}






//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Releases Memory for Msg ID.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CRestartFile::~CRestartFile()
{
//
//    Release storage from dynamically allocated string parameters
//
	if (m_szMsgID != NULL) {
		delete[] m_szMsgID;
		m_szMsgID = NULL;
	}
#ifdef _ver2_
	if (m_szRestartKey != NULL) {
		delete[] m_szRestartKey;
		m_szRestartKey = NULL;
	}
#endif
	CAny2::termSelf();
}






//---------------------------------------------------------------------+
//  Method:   Init
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     Control Queue Reference
//
//  Returns:  Nothing
//  
//  Dev Log:  This is Junk, Commented Out.
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
//void CRestartFile::Init()
//{
//	// Do initialization of restart file here.
//
//	m_lDataMsgs = 0;//cq.GetRecCount();
//	m_lCtlMsgs	= 0;//cq.GetCtlMsgCount();
//
//	if (NULL != m_szMsgID)
//		delete[] m_szMsgID;
//
//	m_szMsgID = new char[/*strlen(cq.GetMsgID())+1*/ 25];
//
//	//strncpy(m_szMsgID, cq.GetMsgID(), sizeof(m_szMsgID) - 1);
//	strncpy(m_szMsgID, "MSG ID NOT INITIALIZED", sizeof(m_szMsgID) - 1);
//
//	//m_lCorlID = cq.GetCorlID();
//
//	WriteRestartRec(/*cq*/);
//	
//}






//---------------------------------------------------------------------+
//  Method:   GetMsgID
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
char* CRestartFile::GetMsgID() {
	return m_szMsgID;
}




//---------------------------------------------------------------------+
//  Method:   SetMsgID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetMsgID(char* szMsgID) {

	if (szMsgID == NULL) {

		iu_errors.AddError(KORC_INVALID_MSG_ID, KOCC_FAIL,
			"Internal Error.  Message ID cannot be NULL.",
			"CRestartFile::SetMsgID:1",
			is_InstanceName.getPointer());

		return;
	}


	if (strlen(szMsgID) > 24) {

		iu_errors.AddError(KORC_INVALID_MSG_ID, KOCC_FAIL,
			"Internal Error.  Message ID longer than 24 characters.",
			"CRestartFile::SetMsgID:2",
			is_InstanceName.getPointer());

		return;
	}


	try {

		if (NULL != m_szMsgID) {
			delete[] m_szMsgID;
		}

		m_szMsgID = new char[25];

		strcpy(m_szMsgID, szMsgID);

	}

	catch (...) {
		
		iu_errors.AddError(KORC_UNSPECIFIED, KOCC_FAIL,
			"Unexpected Error.",
			"CRestartFile::SetMsgID:1",
			is_InstanceName.getPointer());

	}

}




//---------------------------------------------------------------------+
//  Method:   GetRecCount
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Returns the Number of Data Messages Processed.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
ulong CRestartFile::GetDataMsgsRead()
{
	return m_lDataMsgs;
}



//---------------------------------------------------------------------+
//  Method:   SetDataMsgsRead
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetDataMsgsRead(ulong vul_value) {
	m_lDataMsgs = vul_value;
}



//---------------------------------------------------------------------+
//  Method:   GetCorlID
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
ulong CRestartFile::GetCorlID() {
	return m_lCorlID;
}







//---------------------------------------------------------------------+
//  Method:   SetCorlID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetCorlID(ulong vul_value) {
	m_lCorlID = vul_value;
}




//---------------------------------------------------------------------+
//  Method:   GetExpectedCount
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
ulong CRestartFile::GetExpectedCount()  {
	return m_lExpected;
}



//---------------------------------------------------------------------+
//  Method:   SetExpectedCount
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetExpectedCount(ulong vul_value) {
	m_lExpected = vul_value;
}




//---------------------------------------------------------------------+
//  Method:   GetCtlMsgCount
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
ulong CRestartFile::GetCtlMsgCount()
{
	return m_lCtlMsgs;
}




//---------------------------------------------------------------------+
//  Method:   SetCtlMsgCount
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetCtlMsgCount(ulong vul_value) {
	m_lCtlMsgs = vul_value;
}



//---------------------------------------------------------------------+
//  Method:   SetRestartKey
//  Author:   Daniel R Creager
//  Written:  October 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  10/18/99 DRC Created original version.
//---------------------------------------------------------------------+
void CRestartFile::SetRestartKey(char* as_Arg1) {
#ifdef _ver2_
	if (as_Arg1 == NULL) {
		iu_errors.AddError(KORC_MISSING_PARMS, KOCC_FAIL,
			"Internal Error.  Restart Key cannot be NULL.",
			"CRestartFile::SetRestartKey:1",
			is_InstanceName.getPointer());
		return;
	}

	if (strlen(as_Arg1) > 12) {
		iu_errors.AddError(KORC_INVALID_BUFFER_SIZE, KOCC_FAIL,
			"Internal Error.  Restart Key longer than 12 characters.",
			"CRestartFile::SetRestartKey:2",
			is_InstanceName.getPointer());
		return;
	}

	try {
		//if (NULL != m_szRestartKey) {
		//	delete[] m_szRestartKey;
		//}

	    //m_szRestartKey = new char[12];
		strcpy(m_szRestartKey, as_Arg1);

	}

	catch (...) {
		iu_errors.AddError(KORC_UNSPECIFIED, KOCC_FAIL,
			"Unexpected Error.",
			"CRestartFile::SetRestartKey:3",
			is_InstanceName.getPointer());
	}
#else
	m_sRestartKey    = as_Arg1;
#endif
}




//---------------------------------------------------------------------+
//  Method:   GetRestartKey
//  Author:   Daniel R Creager
//  Written:  October 1999
//
//  Desc:     Get/Set Method for Member Variable
//
//  Args:     Get/Set Method for Member Variable
//
//  Returns:  Get/Set Method for Member Variable
//  
//  Dev Log:  
//  History: 
//  10/18/99 DRC Created original version.
//---------------------------------------------------------------------+
char* CRestartFile::GetRestartKey() {
#ifdef _ver2_
	return m_szRestartKey;
#else
    return m_sRestartKey.getPointer();
#endif
}



//---------------------------------------------------------------------+
//  Method:   Checkpoint
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Calls Local Write Restart Record Method
//            Logs SyncPoint
//
//  Args:     Control Queue Reference
//
//  Returns:  Nothing
//  
//  Dev Log:  Junk, Commented Out
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
//void CRestartFile::Checkpoint(/*CCtlQue& cq*/)
//{
//	//cq.ClearUncommittedMsgCount();
//
//	WriteRestartRec(/*cq*/);
//
//	//cout << "Checkpoint! " << cq << endl;
//}






//---------------------------------------------------------------------+
//  Method:   WriteRestartRec
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Opens Restart File
//            Streams Out Restart Info
//            Closes Restart File
//
//  Args:     None
//
//  Returns:  long	KOCC_GOOD if okay
//                  KOCC_FAIL if an error occurs.
//  
//  Dev Log:  Consider having this loop through the Parameter Table 
//            and use parameter name stored there.  Simplies 
//            maintenance.
//
//            TODO  Verify Correl ID is written out properly on 
//                  Windows platforms and doesn't come out as all 0's.
//
//  History: 
//  01/15/99 CAH Created original version.
//  03/14/99 DMT Significant changes.
//  01/10/00 DRC Added RestartKey to the Restart Attributes
//---------------------------------------------------------------------+
long CRestartFile::WriteRestartRec() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;
	CString	ls_errno_desc;

	try {

		open(m_szFileName, ios::out);


#ifndef _WIN32
		if (rdbuf()->is_open()) 
#else
		if (is_open()) 
#endif
		{
*this << "MessageID="                                 << m_szMsgID      << endl
	  << "CorrelationID=" << setfill('0') << setw(12) << m_lCorlID      << endl
	  << "ExpectedMessageCount="          << setw(12) << m_lExpected    << endl
	  << "DataMessagesRead="              << setw(12) << m_lDataMsgs    << endl
#ifdef _ver2_
	  << "ControlMessagesProcessed="      << setw(12) << m_lCtlMsgs     << endl
	  << "RestartKey="                    << setw(12) << m_szRestartKey << endl;
#else
	  << "ControlMessagesProcessed="      << setw(12) << m_lCtlMsgs     << endl;
#endif

			close();

		}

		else {

			ll_return = KOCC_FAIL;

			TranslateErrno(ls_errno_desc, errno);

			ls_msg << "Cannot open restart file '"
				   << m_szFileName << "'.  " 
				   << ls_errno_desc;

			iu_errors.AddError(KORC_OPEN_OUTPUT_FAIL, ll_return, 
				ls_msg,
				"CRestartFile::WriteRestartRec:1", 
				is_InstanceName.getPointer());

		}
	}

	catch (...) {

		ll_return = KOCC_FAIL;

		TranslateErrno(ls_errno_desc, errno);

		ls_msg << "Error writing Restart information to Restart File '"
			   << m_szFileName << "'.  " 
			   << ls_errno_desc;

		iu_errors.AddError(KORC_WRITE_TO_OUTPUT_FAIL, ll_return, 
				ls_msg,
				"CRestartFile::WriteRestartRec:2", 
				is_InstanceName.getPointer());

	}

	return ll_return;

}








//---------------------------------------------------------------------+
//  Method:   WriteRestartRec
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Opens Restart File
//            Streams Out Restart Info
//            Closes Restart File
//
//  Args:     ulong	New Total Msg Count
//
//  Returns:  long	KOCC_GOOD if okay
//                  KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CRestartFile::WriteRestartRec(ulong vul_value) {

	m_lDataMsgs = vul_value;

	return WriteRestartRec();

}







//---------------------------------------------------------------------+
//  Method:   show
//  Author:   Daniel R Creager
//  Written:  February 2000
//
//  Desc:     Dumps parameter values.
//
//  Args:     CLog&  CLog Object
//            int    Type of Log entries to write. (Console | File)
//
//  Returns:  Nothing
//  
//  Dev Log:  Consider having this loop through parameter table and
//            use keyword values.  This will simplify maintenance. 
//
//  History: 
//  02/12/00 DRC Created original version from code in the 
//               WriteRestartRec method.
//---------------------------------------------------------------------+
void CRestartFile::show(CLog& a_pcLog, int a_iEntryType){

	CString l_sLogRcrd(100);
	
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "MessageID                : " 
		   << *valid_str(m_szMsgID), a_iEntryType);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "CorrelationID            : " 
		   << m_lCorlID, a_iEntryType);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "ExpectedMessageCount     : " 
		   << m_lExpected, a_iEntryType);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "DataMessagesRead         : " 
		   << m_lDataMsgs, a_iEntryType);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "ControlMessagesProcessed : " 
		   << m_lCtlMsgs, a_iEntryType);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "RestartKey               : " 
		   << *valid_str(m_szRestartKey), a_iEntryType);

}  /* End Method */


