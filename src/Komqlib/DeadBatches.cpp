//---------------------------------------------------------------------+
//  File:     DeadBatches.cpp
//  Author:   David Tamkun
//  Written:  April 1999
//  Desc:     This file contains the implementation of the CDeadBatches
//            Class.
//  
//            This class represents a "Collection" of CDeadBatch
//            objects that's implemented as a linked list.  This
//            collection is used by the DeadReport algorithm
//            (aka Dead Letter Mode 2).
//
//  Dev Log: 
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "DeadBatches.h"






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
CDeadBatches::CDeadBatches() : CAny2() {

	m_pcFirstBatch	= NULL;
	m_pcLastBatch	= NULL;

	m_lNumBatches	= 0;

	m_pcLog			= NULL;

	is_InstanceName = "Batch Collection";

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
CDeadBatches::~CDeadBatches() {

	termSelf();

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
long CDeadBatches::initSelf(CLog* v_pcLog) {


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
long CDeadBatches::termSelf() {


	return RemoveAll();
}









//---------------------------------------------------------------------+
//  Method:   AddBatch
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Adds a batch with the specified properties to the 
//            collection.
//
//  Args:     ref CString	Batch ID
//            ref CString	Dead Reason Name (ie MQRC_...).
//            ref CString   Destination Queue Manager Name
//            ref CString   Destination Queue Name
//            ref CString   ReplyTo Queue Name
//
//  Returns:  Pointer to Added Batch.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch* CDeadBatches::AddBatch(CString& rs_BatchID, 
						CString& rs_Reason, 
						CString& rs_DestQueueMgr, CString& rs_DestQueue, 
						CString& rs_ReplyToQ) {

	CString	ls_msg;
	long	ll_return = KOCC_GOOD;


	CDeadBatch* l_pcNew    = NULL;
	CDeadBatch* l_pcOldEnd = NULL;

	try {
		l_pcNew = new CDeadBatch;
	}

	catch (...) {
		l_pcNew = NULL;
	}

	if (l_pcNew == NULL) {

		ll_return = KOCC_FAIL;

		ls_msg = "Unable to Allocate New Batch Class.";

		m_pcLog->LogError(KORC_MEMORY_ALLOC_FAIL, ll_return, ls_msg,
						"CDeadBatches::AddBatch:1",
						is_InstanceName.getPointer());
	}
	else {

		m_lNumBatches++;

		l_pcNew->initSelf(m_pcLog);

		l_pcOldEnd = m_pcLastBatch;

		if (m_pcFirstBatch == NULL) {
			m_pcFirstBatch = l_pcNew;
		} 
		else {
			l_pcOldEnd->SetNext(l_pcNew);
		}

		m_pcLastBatch = l_pcNew;


		l_pcNew->SetPrevious(l_pcOldEnd);

		l_pcNew->m_sBatchID          = rs_BatchID;
		l_pcNew->m_szDeadReason      = rs_Reason;
		l_pcNew->m_sDestQueueMgrName = rs_DestQueueMgr;
		l_pcNew->m_sDestQueueName    = rs_DestQueue;
		l_pcNew->m_sReplyQueueName   = rs_ReplyToQ;

	}

	return l_pcNew;

}









//---------------------------------------------------------------------+
//  Method:   GetBatch
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Locates the DeadBatch Class with the specified batch ID
//            in the collection.
//
//  Args:     ref CString	Batch ID to "look up".
//
//  Returns:  Pointer to DeadBatch class or NULL if a node with
//            the specified ID doesn't exist in the collection.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadBatch* CDeadBatches::GetBatch(CString& rs_BatchID) {

	CDeadBatch* l_pcBatch   = NULL;
	BOOL		lb_continue = TRUE;

	l_pcBatch = m_pcFirstBatch;

	while (l_pcBatch != NULL && lb_continue) {

		if (l_pcBatch->m_sBatchID == rs_BatchID.getPointer()) {
			lb_continue = FALSE;
		}
		else {
			l_pcBatch = l_pcBatch->GetNext();
		}
	}

	return l_pcBatch;
}







//---------------------------------------------------------------------+
//  Method:   SetStatus
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Traverses the collection and asks each Batch to 
//            set its Batch Status.
//
//  Args:     None
//
//  Returns:  Always returns KOCC_GOOD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatches::SetStatus() {

	long	ll_return   = KOCC_GOOD;
	BOOL	lb_continue = TRUE;

	CDeadBatch* l_pcBatch   = NULL;


	l_pcBatch = m_pcFirstBatch;

	while (l_pcBatch != NULL && lb_continue) {

		l_pcBatch->SetStatus();
		
		l_pcBatch = l_pcBatch->GetNext();
	}

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   RemoveAll
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     If the collection is not empty, removes all list
//            elements and reset the Collection Classes first and 
//            last pointers to NULL.
//
//  Args:     None
//
//  Returns:  Always returns KOCC_GOOD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatches::RemoveAll() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	CDeadBatch*	l_pcLast	= NULL;
	CDeadBatch* l_pc2ndLast	= NULL;


	l_pcLast = m_pcLastBatch;

	while (l_pcLast != NULL) {

		l_pc2ndLast = l_pcLast->GetPrevious();


		l_pcLast->termSelf();

		delete l_pcLast;

		m_lNumBatches--;

		m_pcLastBatch = l_pc2ndLast;

		if (l_pc2ndLast != NULL) {
			l_pc2ndLast->SetNext(NULL);
		}

		l_pcLast = l_pc2ndLast;

	}

	m_pcFirstBatch = NULL;

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   ProcessBatch
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Looks to see if the provided batch is in the Collection.
//            If so, calls the elements "TallyBatch" method to update
//            the counts.  
//
//            If the batch doesn't exist, calls AddBatch to Add it 
//            to the collection.
//
//            There should be no more than 1 control msg for a batch,
//            but if for some reason, there's more than one, any 
//            control msg with the same Batch ID updates the Expected
//            Message Count.
//
//            The Reason Code of the First Message in a Batch Sets
//            the reason code for the whole batch.  Likewise, the 
//            first message in a batch sets the Destination Queue and
//            Queue Manager, as well as the ReplyToQ.
//            
//
//  Args:     ref CString	Batch ID
//            long	Message Type
//            long  Number of Messages - ignored if Message Type is not
//                                       1, (ie not a Control Msg).
//            ref CString	Dead Reason Name (ie MQRC_...).
//            ref CString   Destination Queue Manager Name
//            ref CString   Destination Queue Name
//            ref CString   ReplyTo Queue Name
//
//  Returns:  KOCC_GOOD if successful, KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatches::ProcessBatch(CString& rs_BatchID,
		                        long     vl_MsgType,
							    long     vl_ExpectedCount,
							    CString& rs_Reason,
							    CString& rs_DestQueueMgr,
							    CString& rs_DestQueue,
							    CString& rs_ReplyToQ) {

	CString		ls_msg;
	long		ll_return	= KOCC_FAIL;
	CDeadBatch*	lpc_Batch	= NULL;


	lpc_Batch = GetBatch(rs_BatchID);


	if (lpc_Batch == NULL) {
		lpc_Batch = AddBatch(rs_BatchID, rs_Reason, 
							rs_DestQueueMgr, rs_DestQueue, rs_ReplyToQ);
	}


	if (lpc_Batch != NULL) {

		ll_return = lpc_Batch->TallyBatch(vl_MsgType, vl_ExpectedCount);

	}

	return ll_return;
}








//---------------------------------------------------------------------+
//  Method:   LogReport
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Writes Dead Letter Report to Log File.
//
//  Args:     None
//
//  Returns:  Always returns KOCC_GOOD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadBatches::LogReport() {

	long		ll_return = KOCC_GOOD;
	CString		ls_msg;
	CString		ls_line;
	CString		ls_wrapped;
	CString		ls_title;

	CDeadBatch*	lpc_Batch = NULL;
	
	long		ll_report_width = 80;


	ls_title << "\n"
		     << "                          DEAD LETTER CONTENT SUMMARY"
			 << "\n\n";

	m_pcLog->WriteLog(ls_title);


	lpc_Batch = m_pcFirstBatch;

	while (lpc_Batch != NULL) {

		ls_line    = "";
		ls_wrapped = "";


		// Change the Fill Character from NULL to a Space so
		// the trim function will work correctly.
		//
		lpc_Batch->m_sBatchID.ls_fillChar = ' ';
		lpc_Batch->m_szDeadReason.ls_fillChar = ' ';
		lpc_Batch->m_szStatus.ls_fillChar = ' ';
		lpc_Batch->m_sDestQueueMgrName.ls_fillChar = ' ';
		lpc_Batch->m_sDestQueueName.ls_fillChar = ' ';
		lpc_Batch->m_sReplyQueueName.ls_fillChar = ' ';
		

		// Want to trim off leading or trailing spaces for cosmetics.
		lpc_Batch->m_sBatchID = lpc_Batch->m_sBatchID.trim();

		lpc_Batch->m_szDeadReason = 
							lpc_Batch->m_szDeadReason.trim();

		lpc_Batch->m_szStatus = lpc_Batch->m_szStatus.trim();

		lpc_Batch->m_sDestQueueMgrName = 
							lpc_Batch->m_sDestQueueMgrName.trim();

		lpc_Batch->m_sDestQueueName = 
							lpc_Batch->m_sDestQueueName.trim();

		lpc_Batch->m_sReplyQueueName = 
							lpc_Batch->m_sReplyQueueName.trim();


		// Build the Report Line
		ls_line << "ID("     << lpc_Batch->m_sBatchID          << ") "
			    << "REASON(" << lpc_Batch->m_szDeadReason      << ") "
				<< "STATUS(" << lpc_Batch->m_szStatus          << ") "
				<< "QMGR("   << lpc_Batch->m_sDestQueueMgrName << ") "
				<< "DEST("   << lpc_Batch->m_sDestQueueName    << ") "
				<< "RESP("   << lpc_Batch->m_sReplyQueueName   << ") ";

		// Wrap the Report Line to 80 columns without any line prefix.
		m_pcLog->WrapLine(ls_wrapped, ls_line.getPointer(), 
						  ll_report_width, "");

		// Write Line(s) to Log File.
		m_pcLog->WriteLog(ls_wrapped);

		lpc_Batch = lpc_Batch->GetNext();

	}

	ls_wrapped = "";

	ls_line  = "\nTHIS DATA RELECTS A SNAPSHOT THAT MAY CHANGE OVER ";
	ls_line += "RELATIVELY SHORT PERIODS OF TIME.";

	m_pcLog->WrapLine(ls_wrapped, ls_line.getPointer(), 
						          ll_report_width, "");

	m_pcLog->WriteLog(ls_wrapped);


	return ll_return;
}