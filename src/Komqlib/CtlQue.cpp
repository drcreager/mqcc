/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: %E%	%A%
 */
//---------------------------------------------------------------------+
//  File:     CtlQue.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CCtlQue
//            Class.
//
//            This class represents a Control Queue in the Utilities
//            and knows how to get the Message Count out of a
//            Control Message.
//  
//  Dev Log:  Knowledge of how a control message is formatted is 
//            present here and in CCtlFile.  This info should really
//            be in one place.  Consider a "Control Message" class in
//            the future that's used by both CCtlFile and CCtlQue.
//
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#include "CtlQue.h"






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     Queue Mgr Object Reference
//			  long	Buffer Size for Message Data
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CCtlQue::CCtlQue(CQueMgr* v_pcQM) : CQue(v_pcQM) {

	m_lDataMsgCount			= 0;
	m_lNumDataMsg			= 0;
	m_lCorlID				= 0;
	m_lCtlMsgCount			= 0;
	m_lUncommittedMsgCount	= 0;
	is_InstanceName			= "Control Queue";
}

CCtlQue::CCtlQue() : CQue() {

	m_lDataMsgCount			= 0;
	m_lNumDataMsg			= 0;
	m_lCorlID				= 0;
	m_lCtlMsgCount			= 0;
	m_lUncommittedMsgCount	= 0;
	is_InstanceName			= "Control Queue";
}




//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CCtlQue::~CCtlQue()
{
}






//---------------------------------------------------------------------+
//  Method:   GetDataMsgCount
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  ulong	Data Message Count
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
ulong CCtlQue::GetDataMsgCount() const
{
	return m_lDataMsgCount;
}






//---------------------------------------------------------------------+
//  Method:   DataMsgCountMatches
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  TRUE if the Data Message Count Matches the Expected
//                 Number of Messages
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
BOOL CCtlQue::DataMsgCountMatches() const
{
	return m_lDataMsgCount == m_lNumDataMsg;
}






//---------------------------------------------------------------------+
//  Method:   CountGoodDataMsg
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Increments counters for a successful message
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CCtlQue::CountGoodDataMsg()
{
	++m_lDataMsgCount;
	++m_lCorlID;
	++m_lUncommittedMsgCount;
}






//---------------------------------------------------------------------+
//  Method:   SetRecCount
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Sets the Record Processed Counter
//
//  Args:     ulong Record Count
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CCtlQue::SetRecCount(const ulong lRecCount)
{
	m_lDataMsgCount = lRecCount;
}






//---------------------------------------------------------------------+
//  Method:   GetRecCount
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
ulong CCtlQue::GetRecCount() const
{
	return m_lDataMsgCount;
}






//---------------------------------------------------------------------+
//  Method:   SetCorlID
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
//void CCtlQue::SetCorlID(const ulong lCorlID)
//{
//	m_lCorlID = lCorlID;
//}






//---------------------------------------------------------------------+
//  Method:   GetCorlID
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  ulong	Correlation ID
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
//ulong CCtlQue::GetCorlID() const
//{
//	return GetCorlIDBin();
//}






//---------------------------------------------------------------------+
//  Method:   SetCtlMsgCount
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     ulong lCount
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CCtlQue::SetCtlMsgCount(const ulong lCount)
{
	m_lCtlMsgCount = lCount;
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
ulong CCtlQue::GetCtlMsgCount() const
{
	return m_lCtlMsgCount;
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
long CCtlQue::LoadData(CBuffer& cBuffer)
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
	//if ( GetMsgTextLen() < sizeof(SCtlRecord) )
	//	throw CExcBadCtlMsg(*this);

	if (GetMsgTextLen() < 65) {

		ll_return = KOCC_FAIL;

		ls_msg = "";

		ls_msg << "Control Message is only " 
			   << GetMsgTextLen() 
			   << " bytes.  Message Count is not available.";

		m_pcLog->LogError(KORC_INVALID_MSG_COUNT, ll_return, 
			ls_msg,
			"cCtlQue::LoadData:1",
			is_InstanceName.getPointer());

	}
	else {
	
		//SCtlRecord*	sCtlr = (SCtlRecord*)GetMsgText();
		SCtlRecord*	sCtlr = (SCtlRecord*) cBuffer.getPointer();

		memcpy(szDataMsgCount, sCtlr->rControlCount1, CTL_COUNT_LEN);


		//-------------------------------------------------------------+
		// Throw an exception if the Message Count in the Control 
		// Message is not numeric.
		//-------------------------------------------------------------+
		if (FALSE == isdec(szDataMsgCount)) {

			ll_return = KOCC_FAIL;

			ls_msg = "Control Message Expected Message Count is not numeric.";


			m_pcLog->LogError(KORC_INVALID_MSG_COUNT, ll_return, 
				ls_msg,
				"cCtlQue::LoadData:2",
				is_InstanceName.getPointer());


			//throw CExcBadCtlMsg(*this);
		}
		else {

			m_lNumDataMsg = atol(szDataMsgCount);
		}
	}

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   GetNumDataMsg
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
ulong CCtlQue::GetNumDataMsg() const
{
	return m_lNumDataMsg;
}






//---------------------------------------------------------------------+
//  Method:   ClearUncommittedMsgCount
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
void CCtlQue::ClearUncommittedMsgCount()
{
	m_lUncommittedMsgCount = 0;
}






//---------------------------------------------------------------------+
//  Method:   GetUncommittedMsgCount
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
ulong CCtlQue::GetUncommittedMsgCount() const
{
	return m_lUncommittedMsgCount;
}






//---------------------------------------------------------------------+
//  Method:   IncCtlMsgCount
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
void CCtlQue::IncCtlMsgCount()
{
	++m_lCtlMsgCount;
}







//---------------------------------------------------------------------+
//  Method:   BrowseFirst
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Browse First Message on Queue 
//
//  Args:     char* Message ID
//            ulong	Correlation ID
//
//  Returns:  Nothing
//  
//  Dev Log:  NOTE!! Correlation ID must always be numeric.
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CCtlQue::BrowseFirst(CBuffer& cBuffer, 
					   const char* szMsgID, ulong ulCorlID)
{
	long ll_return = KOCC_GOOD;
	//try
	//{
		// Uses get with value in MQGMO struct set to BrowseFirst
	
	ll_return = CQue::BrowseFirst(cBuffer, szMsgID, ulCorlID);

	if (ll_return == KOCC_GOOD) {
		ll_return = LoadData(cBuffer);
	}

	//}
	//catch (CExcNoMoreMsgs&)
	//{
	//	throw CExcNoMoreCtlMsgs(*this);
	//}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   BrowseNext
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CCtlQue::BrowseNext(CBuffer& cBuffer)
{
	long ll_return = KOCC_GOOD;
	//try
	//{

	ll_return = CQue::BrowseNext(cBuffer);
	// Uses get with value in MQGMO struct set to BrowseNext

	if (ll_return == KOCC_GOOD) {
		ll_return = LoadData(cBuffer);
	}
	//}
	//catch (CExcNoMoreMsgs&)
	//{
	//	throw CExcNoMoreCtlMsgs(*this);
	//}

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   operator <<
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
ostream& operator << (ostream& s, CCtlQue& cq)
{
	s << "Dump of Control Que"                       << endl
	  << "Data Msg Count:    " << cq.m_lDataMsgCount << endl
	  << "Num Data Msg:      " << cq.m_lNumDataMsg   << endl
	  << "Correlation ID:    " << cq.m_lCorlID       << endl
	  << "Control Msg Count: " << cq.m_lCtlMsgCount  << endl;
	return s;
}
