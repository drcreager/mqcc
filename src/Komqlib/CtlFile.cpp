//---------------------------------------------------------------------+
//  File:     CtlFile.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CCtlFile
//            Class.
//
//            This class represents a Control File in the Utilities
//            and knows how to get the Message Count out of a
//            Control Message.
//  
//  Dev Log:  Knowledge of how a control message is formatted is 
//            present here and in CCtlQue.  This info should really
//            be in one place.  Consider a "Control Message" class in
//            the future that's used by both CCtlFile and CCtlQue.
//
//  History: 
//  03/23/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "CtlFile.h"



//---------------------------------------------------------------------+
//  Method:   Constructor
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
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CCtlFile::CCtlFile() : CMsgFile() {

	m_lNumMsgs		= 0;
	is_InstanceName	= "Control File";

}







//---------------------------------------------------------------------+
//  Method:   Destructor
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
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CCtlFile::~CCtlFile() {
//	termSelf();
}

	



//---------------------------------------------------------------------+
//  Method:   Export
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Generate a control file containing the number of messages
//            that are supposed to be in the current batch.
//
//  Args:     TBD
//
//  Returns:  long - Return Code 0 == Successful
//  
//  Dev Log:  
//                 
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CCtlFile::Export(long a_lMsgCnt) {
CString			ls_msg;
CBuffer			l_Buf;
long			ll_return = KOCC_GOOD;

//  Initialize 
l_Buf.initSelf(sizeof *m_pcCtlRcrd);

//  Format and Write out the Control Record
ll_return = Open(ios::out);
if (ll_return == KOCC_GOOD){
	SetNumMsgs(a_lMsgCnt);
	UnloadData(l_Buf);
	ll_return = Write(l_Buf);
	Close();
} /* End If */

if (ll_return == KOCC_GOOD) {
} /* end If */

//  Terminate
l_Buf.termSelf();
return ll_return;
} /* end Export */




//---------------------------------------------------------------------+
//  Method:   GetNumMsgs
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Returns the number of messages that are supposed to be
//            in the batch as specified in the control record that
//            was just read.
//
//  Args:     TBD
//
//  Returns:  long - The number of messages in the batch specified in
//                   the control message record.
//  
//  Dev Log:  
//                 
//  History: 
//  03/23/99 DMT Created original version.
//---------------------------------------------------------------------+
long CCtlFile::GetNumMsgs() {
	return m_lNumMsgs;
}







//---------------------------------------------------------------------+
//  Method:   Read
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Reads a record from the Control File, stores the
//            contents in the Buffer class, and loads the control
//            info into member variables.
//
//  Args:     TBD
//
//  Returns:  KOCC_WARNING if EOF
//            KOCC_FAIL if error
//            KOCC_GOOD if read line of data.
//  
//  Dev Log:  
//                 
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CCtlFile::Read(CBuffer& cBuffer) {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	ll_return = CFile::Read(cBuffer);

	if (ll_return == KOCC_GOOD) {
		ll_return = LoadData(cBuffer);
	}

	return ll_return;
}





//---------------------------------------------------------------------+
//  Method:   LoadData
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     This is a common routine to load member variables from 
//            a control que data msg if necessary.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CCtlFile::LoadData(CBuffer& cBuffer)
{
	CString	ls_msg;
	long	ll_return = KOCC_GOOD;

	char	szDataMsgCount[CTL_COUNT_LEN+1];

	memset(szDataMsgCount,'\0', sizeof(szDataMsgCount));

	//-----------------------------------------------------------------+
	// Uncomment the lines below to enforce tighter validation on
	// the control message.  The lines below throw an exception if
	// the length of the control message does not exactly match the
	// expected length.
	//-----------------------------------------------------------------+
	//if ( GetMsgTextLen() < sizeof(SCtlRecord2) ){
	//	Set Error Info Here;
	//}
	
	//SCtlRecord*	sCtlr = (SCtlRecord*)GetMsgText();
	m_pcCtlRcrd = (SCtlRecord2*) cBuffer.getPointer();

	memcpy(szDataMsgCount, m_pcCtlRcrd->rControlCount1, CTL_COUNT_LEN);


	//-----------------------------------------------------------------+
	// Throw an exception if the Message Count in the Control Message
	// is not numeric or the data is < 65 bytes (might not have the 
	// full message count).
	//-----------------------------------------------------------------+
	if (strlen(cBuffer.getPointer()) < 65) {

		ll_return = KOCC_FAIL;

		ls_msg << "Control Message is only " 
			   << strlen(cBuffer.getPointer())
			   << " bytes.  Message Count is not available.";

		m_pcLog->LogError(KORC_INVALID_MSG_COUNT, ll_return, 
			ls_msg,
			"CCtlFile::LoadData:2",
			is_InstanceName.getPointer());

	}
	else if (FALSE == isdec(szDataMsgCount)) {

		ll_return = KOCC_FAIL;

		ls_msg = "Control Message Expected Message Count is not numeric.";


		m_pcLog->LogError(KORC_INVALID_MSG_COUNT, ll_return, 
			ls_msg,
			"CCtlFile::LoadData:1",
			is_InstanceName.getPointer());

	}
	else {

		m_lNumMsgs = atol(szDataMsgCount);
	}

	return ll_return;
}



void CCtlFile::SetNumMsgs(long a_lArg){
m_lNumMsgs = a_lArg;
}





//---------------------------------------------------------------------+
//  Method:   UnloadData
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This is a common routine to unload member variables 
//            into a control msg.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CCtlFile::UnloadData(CBuffer& cBuffer) {
	long		ll_return = KOCC_GOOD;

	if (cBuffer.getSize() < sizeof m_pcCtlRcrd)
		ll_return = KOCC_FAIL;
	else {
		memset(cBuffer.getPointer(), ' ', cBuffer.getSize());
		m_pcCtlRcrd = (SCtlRecord2*) cBuffer.getPointer();
   		memcpy(m_pcCtlRcrd->rPartnerProfile,  "KOMQUTLC", 8);
		memcpy(m_pcCtlRcrd->rMessageType,     "FileXfr ", 8);
		sprintf(m_pcCtlRcrd->rControlCount1,  "%011d",    m_lNumMsgs );
		sprintf(m_pcCtlRcrd->rControlCount2,  "%011d",    0);
		sprintf(m_pcCtlRcrd->rControlCount3,  "%011d",    0);
		sprintf(m_pcCtlRcrd->rControlCount4,  "%011d",    0);
		sprintf(m_pcCtlRcrd->rControlCount5,  "%011d",    0);
		sprintf(m_pcCtlRcrd->rControlAmount1, "%023d",    0);
		m_pcCtlRcrd->rCntlAmtSign1 = '+';
		sprintf(m_pcCtlRcrd->rControlAmount2, "%023d",    0);
		m_pcCtlRcrd->rCntlAmtSign2 = '+';
		sprintf(m_pcCtlRcrd->rControlAmount3, "%023d",    0);
		m_pcCtlRcrd->rCntlAmtSign3 = '+';
		sprintf(m_pcCtlRcrd->rControlAmount4, "%023d",    0);
		m_pcCtlRcrd->rCntlAmtSign4 = '+';
		sprintf(m_pcCtlRcrd->rControlAmount5, "%023d",    0);
		m_pcCtlRcrd->rCntlAmtSign5 = '+';
	} /* End If */
	
	return ll_return;
}



