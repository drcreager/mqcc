//---------------------------------------------------------------------+
//  File:     DeadBatch.cpp
//  Author:   David Tamkun
//  Written:  April 1999
//  Desc:     This file contains the implementation of the CDeadBatch
//            Class.
// 
//            This class represents a Batch of Messages found on 
//            the Dead Letter Queue and represents a node of a linked-
//            list of batches generated during DeadLetter Queue
//            Reporting (execution of the DeadReport Algorithm).
// 
//  Dev Log: 
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "DeadBatch.h"





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch::CDeadBatch() : CAny2() {

	m_lDeadReason = 0;

	m_lExpectedMsgs = -1;
	
	m_lDataCount = 0;
	m_lCtrlCount = 0;
	m_lReplyCount = 0;
	m_lErrorCount = 0;

	m_pcLog			= NULL;

	m_pcNext		= NULL;
	m_pcPrevious	= NULL;


}






//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch::~CDeadBatch() {


}









//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatch::initSelf(CLog* v_pcLog) {

	if (v_pcLog    == NULL) {

		return KOCC_FAIL;
	}
	else {

		m_pcLog    = v_pcLog;
		
		return KOCC_GOOD;
	}

}






//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatch::termSelf() {

	m_pcLog    = NULL;

	return KOCC_GOOD;

}






//---------------------------------------------------------------------+
//  Method:   SetStatus
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Sets the Batch's Status to Full, Partial, Data Only,
//            or Control Only by examining the message counts and
//            determining if the Message Count was ever set.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatch::SetStatus() {


	if (m_lExpectedMsgs > -1) {

		if(m_lExpectedMsgs == m_lDataCount) {
							
			m_szStatus = "FULL_BATCH";
		}
		else if (m_lDataCount > 0) {

			m_szStatus = "PARTIAL_BATCH";
		}
		else {
			m_szStatus = "CTRL_ONLY";
		}
	}
	else {
		m_szStatus = "DATA_ONLY";
	}


	return KOCC_GOOD;

}








//---------------------------------------------------------------------+
//  Method:   GetStatus
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Returns the batches Status
//
//  Args:     None
//
//  Returns:  char* pointer to Status String.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
char* CDeadBatch::GetStatus() {

	return m_szStatus.getPointer();

}






//---------------------------------------------------------------------+
//  Method:   GetNext
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Returns a pointer to the next CDeadBatch object in the
//            linked list.  Returns NULL if this element is the last
//            (or only) element in the list.
//
//  Args:     None
//
//  Returns:  see above.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch*	CDeadBatch::GetNext() {
	return m_pcNext;
}







//---------------------------------------------------------------------+
//  Method:   GetPrevious
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Returns a pointer to the previous CDeadBatch object in the
//            linked list.  Returns NULL if this element is the first
//            (or only) element in the list.
//
//  Args:     None
//
//  Returns:  See above.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch* CDeadBatch::GetPrevious() {
	return m_pcPrevious;
}







//---------------------------------------------------------------------+
//  Method:   SetNext
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Makes this linked list element point to the specified 
//            element as the next node.
//
//  Args:     Pointer to Next Element.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
void CDeadBatch::SetNext(CDeadBatch* v_pcNext) {
	m_pcNext = v_pcNext;
}






//---------------------------------------------------------------------+
//  Method:   SetPrevious
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Makes this linked list element point to the specified 
//            element as the next node.
//
//  Args:     Pointer to Previous Node.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
void CDeadBatch::SetPrevious(CDeadBatch* v_pcPrevious) {
	m_pcPrevious = v_pcPrevious;
}





//---------------------------------------------------------------------+
//  Method:   TallyBatch
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Increments Message Type Counters for the Batch.
//
//            If the Message is a Control Message, the Batch's 
//            Expected Message Count is updated with the value
//            of vl_NumMsgs if vl_numMsgs hasn't been set yet 
//            (ie still initialized to -1)..
//
//            Although a Batch should only have 1 control message, if
//            more than one control message has the same batch id,
//            The first control message processed for the batch is
//            the one who's Message Count will be reflected in
//            the report.
//
//  Args:     long	Message Type
//            long  Number of Messages - ignored if Message Type is not
//                                       1, (ie not a Control Msg).
//
//  Returns:  KOCC_GOOD if successful, KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatch::TallyBatch(long vl_MsgType, long vl_NumMsgs) {

	CString	ls_msg;
	long	ll_return = KOCC_GOOD;


	switch (vl_MsgType) {

	case MQMT_DATAGRAM:
		// Data Queue

		m_lDataCount++;
		break;
	
	case MQMT_REQUEST:
		// Control Queue

		m_lCtrlCount++;

		if (m_lExpectedMsgs == -1) {
			m_lExpectedMsgs = vl_NumMsgs;
		}

		break;


	case MQMT_REPLY:
		// reply queue

		m_lReplyCount++;
		break;


	case MQMT_REPORT:
		// error queue

		m_lErrorCount++;
		break;


	default:
		// error 
		ll_return = KOCC_FAIL;

		ls_msg << "Unexpected Message Type "
			   << vl_MsgType
			   << ".";

		m_pcLog->LogError(KORC_INVALID_MSG_TYPE, ll_return,
				ls_msg,
				"CDeadBatch::TallyBatch:1",
				is_InstanceName.getPointer());

		break;

	}	// end switch


	return ll_return;


}