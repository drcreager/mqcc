//---------------------------------------------------------------------+
//  File:     DeadReport.cpp
//  Author:   David M. Tamkun
//  Written:  April 1999
//  Desc:     This file contains the implementation of the CDeadReport
//            Class.
// 
//            This class represents the Dead Letter Diagnostic
//            Report (Mode 2) algorithm and is derived from
//            CDeadLetter.
// 
//            The model here is that the "Transfer" is Queue to 
//            Collection Class (for Reporting).  Note that the input
//            is a browse and is not destructive.
//
//  Dev Log: 
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "DeadReport.h"






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     None
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadReport::CDeadReport() : CDeadLetter() {
	

	m_chrGetMode		= 'N';	// for Browse Next
	m_bSyncpointEnable	= FALSE;


	is_InstanceName = "Dead Letter Queue Diagnostic Report Layer";
	
}







//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     None
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CDeadReport::~CDeadReport() {
	
//	CGet::termSelf();

}







//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Check Parameters required for Dead Letter Queue 
//            and set member variables used by the Execute Method.
//
//            m_pcParms Errors are reported in CTransfer::initSelf() 
//            after the CheckParms() call.  No need to report errors
//            here.
//
//            Note that Dead Letter Queue Processing does not support
//            Admin Access.
//
//            For Reporting, we are only browsing the Dead Letter Queue.
//            Must force Batch Interval to 0 to prevent unnecessary
//            interim commits.  They don't do any harm, but are 
//            unnecessary and this keeps the synchpoint counter from
//            incrementing.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::CheckParms() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	//-----------------------------------------------------------------+
	// Check Parameters required at ancestor levels.
	//-----------------------------------------------------------------+
	ll_return = CDeadLetter::CheckParms();

		
	//-----------------------------------------------------------------+
	// If we are okay here, all needed parameters are valid, so
	// set "generic" member variables for Execute with specific
	// "Data parameters.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {

		m_lBatchInterval		= m_pcParms->GetBatchInterval();
		m_sQueueName		= (char*) m_pcParms->GetDeadLetterQueName();
		m_lSynchpoints          = 0;
		m_lExpectedMsgs			= 0;
		m_lTotalMsgsProcessed	= 0;
		m_lMsgsProcessed		= 0;
		m_bRestart				= m_pcParms->IsRestart();
		m_bSequence             = FALSE;

		m_lNumDataMsgs			= 0;
		m_lNumCtrlMsgs			= 0;
		m_lNumReplyMsgs			= 0;
		m_lNumErrorMsgs			= 0;

		m_lDeadReason			= 0;
		m_chrGetMode			= 'N';

		if(m_pcParms->HasAdminAccess()) {
			//m_sBatchID			= (char*) m_pcParms->GetMessageID();
			//m_lCorrelID			= m_pcParms->GetCorrelID();

			ls_msg << "-- Dead Letter Processing does not support"
				   << " Administrator access." << "\n"
				   << "-- Message ID and Correlation ID are ignored"
				   << " if provided.\n";

			m_pcLog->WriteLog(ls_msg);
		}


		if (m_lBatchInterval != 0) {

			m_lBatchInterval = 0;

			ls_msg = "";
			ls_msg << "-- Overriding Batch Interval Parameter to zero.\n"
			       << "-- Interim Commits not required while browsing "
				   << "Dead Letter Queue.\n";

			m_pcLog->WriteLog(ls_msg);
		}

	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   OpenInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens Dead Letter Queue in Browse Mode
//
//  Args:     ref BOOL flag to indicate if input was opened successfully
//                     (and therefore that it should be closed later).
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::OpenInput(BOOL& rb_input_open) {

	long	ll_return = KOCC_FAIL;
	CString	ls_msg;

	ll_return = m_cQue.QOpen(CQue::Browse);

	rb_input_open = (ll_return == KOCC_GOOD);

	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Processing Logic.  Dead Letter Messages are "transferred"
//            into the Dead Batch collection for later reporting.
//
//            Input is "browsed" so nothing is removed from the 
//            Dead Letter Queue.
//
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::Execute() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;



	ll_return = CDeadLetter::Execute();

	//
	// Copy RunStats to Specific Ones
	//

	m_pcParms->SetDataMsgsProcessed(m_lNumDataMsgs);
	m_pcParms->SetTotalDataMsgsProcessed(m_lNumDataMsgs);

	m_pcParms->SetCtrlMsgsProcessed(m_lNumCtrlMsgs);
	m_pcParms->SetTotalCtrlMsgsProcessed(m_lNumCtrlMsgs);


	m_pcParms->SetReplyMsgsProcessed(m_lNumReplyMsgs);
	m_pcParms->SetTotalReplyMsgsProcessed(m_lNumReplyMsgs);


	m_pcParms->SetErrorMsgsProcessed(m_lNumErrorMsgs);
	m_pcParms->SetTotalErrorMsgsProcessed(m_lNumErrorMsgs);


	m_pcParms->SetSynchpoints(m_lSynchpoints);
	m_pcParms->SetMessageID(m_sBatchID.getPointer());

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   initExecute
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Pre-execute initialization
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::initExecute() {

	long	ll_return =	KOCC_GOOD;


	ll_return = CDeadLetter::initExecute();


	if (ll_return == KOCC_GOOD) {

		ll_return = m_cDeadBatches.initSelf(m_pcLog);

	}

	

	if (ll_return == KOCC_GOOD) {

		ll_return = m_cCtlFile.initSelf(m_pcLog, "bogus");

	}


	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   OpenOutput
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Overrides Ancestor methods.  "Output" goes into 
//            collection for tallying and later reporting.  Nothing
//            is really opened.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::OpenOutput() {

	// Override - not needed here.


	return KOCC_GOOD;
}








//---------------------------------------------------------------------+
//  Method:   SendData
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     "Sends" retrieved message to Batch Collection for 
//            tallying.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::SendData() {

	long	ll_return =	KOCC_GOOD;

	long	ll_msg_type = -1;
	CString	ls_msg;
	long*	plCounter	= NULL;
	long	ll_num_msgs = 0;

	CString	ls_Reason;
	CString	ls_BatchID;
	CString	ls_ReplyToQ;

	
	ll_msg_type = m_cQue.GetMsgType();

	//-----------------------------------------------------------------+
	// Set Pointer to appropriate Destination Queue Member Variable.
	// This keeps code cleaner, otherwise would've had to replicate
	// member setting code for each message/queue type.
	//-----------------------------------------------------------------+
	switch(ll_msg_type) {

	case MQMT_DATAGRAM:
		// Data Queue

		plCounter	= &m_lNumDataMsgs;
		break;
	
	case MQMT_REQUEST:
		// Control Queue

		plCounter	= &m_lNumCtrlMsgs;

		ll_return = m_cCtlFile.LoadData(m_cOutputBuffer);

		if (ll_return == KOCC_GOOD) {
			ll_num_msgs = m_cCtlFile.GetNumMsgs();
		}


		break;


	case MQMT_REPLY:
		// reply queue

		plCounter	= &m_lNumReplyMsgs;
		break;

	case MQMT_REPORT:
		// error queue

		plCounter	= &m_lNumErrorMsgs;
		break;

	default:
		// error 
		ll_return = KOCC_FAIL;

		ls_msg << "Unexpected Message Type "
			   << ll_msg_type
			   << ".";

		m_pcLog->LogError(KORC_INVALID_MSG_TYPE, ll_return,
				ls_msg,
				"CDeadReport::SendData:1",
				is_InstanceName.getPointer());

		break;

	}	// end switch


	if (ll_return == KOCC_GOOD) {

		ll_return = m_cQueMgr.XlateReasonCode(m_lDeadReason, 
			                                  ls_Reason);
	}


	if (ll_return == KOCC_GOOD) {
		// Increment Counter for Appropriate Msg Type.
		(*plCounter)++;


		ls_BatchID  = (char*) m_cQue.GetMsgID();

		ls_ReplyToQ = m_cQue.GetReplyToQueueName();

		ll_return = m_cDeadBatches.ProcessBatch(ls_BatchID,
												ll_msg_type,
												ll_num_msgs,
												ls_Reason,
												m_sDestQueueMgrName,
												m_sDestQueueName,
												ls_ReplyToQ);
	}

	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   CloseOutput
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Would close output.  This isn't needed for Dead Letter
//            reporting, so this is a simple override.
//
//  Args:     takes ref to a BOOL
//
//  Returns:  always returns KOCC_GOOD.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::CloseOutput(BOOL &val) {

	// Override, not needed here

	val = FALSE;

	return KOCC_GOOD;
}







//---------------------------------------------------------------------+
//  Method:   termExecute
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Post Execute Logic.  Tells Collection to set Status
//            attributes and then tells collection to write it's 
//            report to the log file.
//
//  Args:     None.
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/15/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadReport::termExecute() {

	long	ll_return =	KOCC_GOOD;
	

	m_cDeadBatches.SetStatus();

	m_cDeadBatches.LogReport();


	m_cCtlFile.termSelf();
	m_cDeadBatches.termSelf();


	return CDeadLetter::termExecute();

}





