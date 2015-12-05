//---------------------------------------------------------------------+
//  File:     DeadLetter.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CDeadLetter
//            Class.
//
//            This class represents the generic "Dead Letter" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CDeadSingle (Mode 1) and CDeadReport (Mode 2)
//            algorithms.
//
//            The model here is that the Transfer is Queue to Queue, 
//            from the Dead Letter Queue to an appropriate 
//            Destination Queue, depending on the source messages type.
//  
//  Dev Log: 
//  History: 
//  03/24/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "DeadLetter.h"





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
CDeadLetter::CDeadLetter() : CTransfer() {

	m_lNumDataMsgs		= 0;
	m_lNumCtrlMsgs		= 0;
	m_lNumReplyMsgs		= 0;
	m_lNumErrorMsgs		= 0;

	m_bLogSynchpoint	= FALSE;
	m_bEnableRestart	= FALSE;
	m_bSyncpointEnable	= TRUE;

	m_lDeadReason		= 0;

	m_chrGetMode		= 'G';	// G = Get, N = BrowseNext

	is_InstanceName = "Dead Letter Business Logic Layer";
	
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
CDeadLetter::~CDeadLetter() {
//	termSelf();
}








//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Additional Queues will be initialized as part of the 
//            "Open" process.  User may not have specified names 
//            for all output queues and this is okay if a message
//            of the corresponding type is not found on the message
//            queue.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/20/00 DRC Eliminated the Log parm on initSelf.
//---------------------------------------------------------------------+
long CDeadLetter::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {

	long ll_return = KOCC_GOOD;

	ll_return = CTransfer::initSelf(v_pcParms, v_pcLog);

	//-----------------------------------------------------------------+
	// Allocate Message Buffer for Receiving Msg Data without Dead
	// Letter Header.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = m_cOutputBuffer.initSelf(m_pcParms->GetBufferSize() + 1);
	}

	return ll_return;

}




//---------------------------------------------------------------------+
//  Method:   initExecute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Pre-Execute initialization.  This where we usually 
//            initialize the Queue and File objects used in the 
//            transfer.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::initExecute() {

	long ll_return = KOCC_GOOD;


	ll_return = m_cQue.initSelf(m_pcLog, &m_cQueMgr,
		                        m_sQueueName.getPointer(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());


	return ll_return;
}



//---------------------------------------------------------------------+
//  Method:   termSelf
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
long CDeadLetter::termSelf() {

	long	ll_return = KOCC_GOOD;


	ll_return = m_cOutputBuffer.termSelf();

	if (ll_return == KOCC_GOOD) {
		ll_return = CTransfer::termSelf();

	}

	return ll_return;
}




//---------------------------------------------------------------------+
//  Method:   termExecute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Pre-Execute shutdown.  This where we usually 
//            shutdown the Queue and File objects used in the 
//            transfer.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::termExecute() {

	long	ll_return = KOCC_GOOD;

	ll_return = m_cQue.termSelf();

	return ll_return;
}




//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Check parameters required for Dead Letter Queue 
//            processing.
//
//            m_pcParms Errors are logged after CheckParms call in
//            CTransfer::initSelf(), so no need to report here.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::CheckParms() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	// Call Ancestor to validate parms needed at "Transfer" level.
	ll_return = CTransfer::CheckParms();

	if (ll_return == KOCC_GOOD) {

		// Validate Parms Needed for any Dead Letter Queue Processing

		// need DLQ Name, target queue names validated only if 
		// appropriate message type is encountered.

		// BI must be 1.  Commit after every message.

		if (! m_pcParms->IsValidDeadLetterQueueName()) {		
			ll_return = KOCC_FAIL;
		}
		

		if (! m_pcParms->IsValidBatchInterval()) {
			ll_return = KOCC_FAIL;
		}

	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   OpenInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens Dead Letter Queue
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::OpenInput(BOOL& rb_input_open) {

	long	ll_return = KOCC_FAIL;
	CString	ls_msg;

	ll_return = m_cQue.QOpen(CQue::Exclusive);

	rb_input_open = (ll_return == KOCC_GOOD);

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   OpenOutput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Stub to override Ancestor.  For DL Processing, 
//            The SendData method calls a local OpenOutput() which
//            opens the appropriate output queue if needed.
//
//  Args:     BOOL 
//
//  Returns:  KOCC_GOOD - this is a stub.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::OpenOutput(BOOL& rb_output_open) {

	long	ll_return = KOCC_GOOD;

	rb_output_open = TRUE;

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   OpenOutput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     The "Real" version for Dead Letter Processing.
//
//            Called by local SendData method.
//
//            Calls m_cQue.GetMsgType() to determine the Message Type
//            of the message we just got from the dead letter queue.
//
//            If Queue for msg type is not open:
//
//                Validates Queue Name
//                calls initSelf for correct queue.
//                calls QOpen for correct queue.
//
//  Args:     None, although this checks m_cQue to get Msg Type of
//                  Received Msg.
//
//  Returns:  KOCC_GOOD if the queue opens successfully, or if the 
//                      queue is already open.
//
//            KOCC_FAIL	if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::OpenOutput() {

	long	ll_return = KOCC_FAIL;
	CString	ls_msg;


	switch (m_cQue.GetMsgType()) {

	case MQMT_DATAGRAM:
		//-------------------------------------------------------------+
		// If Data Queue is Not Open:
		//     make sure we have a valid Queue Name
		//     initialize it
		//     open it.
		//-------------------------------------------------------------+
		if(!m_cDataQue.IsOpen()) {

			if(m_pcParms->IsValidDataQueueName()) {

				ll_return = m_cDataQue.initSelf(m_pcLog, &m_cQueMgr,
								(char*) m_pcParms->GetDataQueName(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());

				if (ll_return == KOCC_GOOD) {

					ll_return = m_cDataQue.QOpen(CQue::Output);
				}
			}
			else {
				ll_return = KOCC_FAIL;
				m_pcLog->LogErrors(*m_pcParms);
			}
		}
		else {
			// Queue already open
			ll_return = KOCC_GOOD;
		}

		break;


	
	
	case MQMT_REQUEST:
		//-------------------------------------------------------------+
		// If Control Queue is Not Open:
		//     make sure we have a valid Queue Name
		//     initialize it
		//     open it.
		//-------------------------------------------------------------+

		if(!m_cCtrlQue.IsOpen()) {

			if(m_pcParms->IsValidCtrlQueueName()) {

				ll_return = m_cCtrlQue.initSelf(m_pcLog, &m_cQueMgr,
								(char*) m_pcParms->GetCtlQueName(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());

				if (ll_return == KOCC_GOOD) {

					ll_return = m_cCtrlQue.QOpen(CQue::Output);
				}
			}
			else {
				ll_return = KOCC_FAIL;
				m_pcLog->LogErrors(*m_pcParms);
			}
		}
		else {
			// Queue already open
			ll_return = KOCC_GOOD;
		}

		break;



	case MQMT_REPLY:
		//-------------------------------------------------------------+
		// If Data Queue is Not Open:
		//     make sure we have a valid Queue Name
		//     initialize it
		//     open it.
		//-------------------------------------------------------------+

		if(!m_cReplyQue.IsOpen()) {

			if(m_pcParms->IsValidReplyToQueueName()) {

				ll_return = m_cReplyQue.initSelf(m_pcLog, &m_cQueMgr,
								(char*) m_pcParms->GetReplyToQName(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());

				if (ll_return == KOCC_GOOD) {

					ll_return = m_cReplyQue.QOpen(CQue::Output);
				}
			}
			else {
				ll_return = KOCC_FAIL;
				m_pcLog->LogErrors(*m_pcParms);
			}
		}
		else {
			// Queue already open
			ll_return = KOCC_GOOD;
		}

		break;



	case MQMT_REPORT:
		//-------------------------------------------------------------+
		// If Data Queue is Not Open:
		//     make sure we have a valid Queue Name
		//     initialize it
		//     open it.
		//-------------------------------------------------------------+

		if(!m_cErrorQue.IsOpen()) {

			if(m_pcParms->IsValidErrorQueueName()) {

				ll_return = m_cErrorQue.initSelf(m_pcLog, &m_cQueMgr,
									(char*) m_pcParms->GetErrQueName(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());

				if (ll_return == KOCC_GOOD) {

					ll_return = m_cErrorQue.QOpen(CQue::Output);
				}
			}
			else {
				ll_return = KOCC_FAIL;
				m_pcLog->LogErrors(*m_pcParms);
			}
		}
		else {
			// Queue already open
			ll_return = KOCC_GOOD;
		}

		break;

	default:

		ll_return = KOCC_FAIL;

		ls_msg << "Unexpected Message Type "
			   << m_cQue.GetMsgType()
			   << ".";

		m_pcLog->LogError(KORC_UNEXPECTED_MSG_TYPE, ll_return,
				ls_msg,
				"CDeadLetter::OpenOutput:1",
				is_InstanceName.getPointer());


	}	// end switch
	

	return ll_return;
}











//---------------------------------------------------------------------+
//  Method:   ReceiveData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Does GET on Dead Letter Queue, using the GetMode 
//            specified in m_chrGetMode. (ie Get vs Browse)
//
//            Always passes NULL and 0 for MsgID and CorrelID, so
//            we won't match on these.
//
//            If Msg Format is "MQDEAD  ", Calls RemoveDeadLetterHeader
//            to remove DLH.  Also resets Msg Format in m_cQue to the
//            original Msg Format in the DeadLetter Header returned by
//            RemoveDeadLetterHeader.
//
//            If Msg Format is not "MQDEAD  ", copies msg data from
//            input to output buffer without modification.
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
long CDeadLetter::ReceiveData() {

	long	ll_return		= KOCC_GOOD;
	

	// Do Administrative Get (allows DLQ Msgs)
	//   Get Mode (Get vs Browse) depends on value of m_chrGetMode
	// Never Match Msg ID or Correl ID.
	// Make sure Synchpoint Option turned on.
	m_cQue.SetSyncpointEnable(m_bSyncpointEnable);
	ll_return = m_cQue.GetMsgA(m_cBuffer, m_chrGetMode);


	if (ll_return == KOCC_GOOD) {

		if (strcmp(m_cQue.GetMsgFormat(), "MQDEAD  ") == 0) {

			ll_return = RemoveDeadLetterHeader(m_cOutputBuffer, 
				                               m_cBuffer);

			// Replace "MQDEAD" msg format with the original format
			// extracted from the Dead Letter Header
			if (ll_return == KOCC_GOOD) {

				m_cQue.SetMsgFormat(m_sFormat.getPointer());
			}
		}
		else {

			// Somehow, a Non Dead-Letter Msg is on the Dead
			// Letter Queue.
			//
			// Clear out Member Variables that would've been set
			// by RemoveDeadLetterHeader and do a straight
			// copy of message data to output buffer.
			//
			// There's no dead letter header to remove.
			m_sDestQueueName	= "Not Available";
			m_sDestQueueMgrName = "Not Available";
			m_sFormat			= m_cQue.GetMsgFormat();
			m_lDeadReason		= 0;

			memcpy(m_cOutputBuffer.getPointer(), 
				   m_cBuffer.getPointer(),
				   m_cOutputBuffer.getSize());

		}

	}

	return ll_return;

}










//---------------------------------------------------------------------+
//  Method:   SendData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls OpenOutput() to open output queue if needed
//
//            puts msg on appropriate queue.
//
//            Datagram Messages get put to the Data Queue
//            Request Messages get put to the Reply Queue
//            Reply Messages get put to the Reply Queue
//            Error Messages get put to the Error Queue
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
long CDeadLetter::SendData() {

	long	ll_return	= KOCC_GOOD;
	long	ll_msg_type = -1;
	CString	ls_msg;
	CQue*	pcQue		= NULL;
	long*	plCounter	= NULL;



	ll_msg_type = m_cQue.GetMsgType();

	//-----------------------------------------------------------------+
	// Set Pointer to appropriate Destination Queue Member Variable.
	// This keeps code cleaner, otherwise would've had to replicate
	// member setting code for each message/queue type.
	//-----------------------------------------------------------------+
	switch(ll_msg_type) {

	case MQMT_DATAGRAM:
		// Data Queue

		pcQue		= &m_cDataQue;
		plCounter	= &m_lNumDataMsgs;
		break;
	
	case MQMT_REQUEST:
		// Control Queue

		pcQue		= &m_cCtrlQue;
		plCounter	= &m_lNumCtrlMsgs;
		break;


	case MQMT_REPLY:
		// reply queue

		pcQue		= &m_cReplyQue;
		plCounter	= &m_lNumReplyMsgs;
		break;

	case MQMT_REPORT:
		// error queue

		pcQue		= &m_cErrorQue;
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
				"CDeadLetter::SendData:1",
				is_InstanceName.getPointer());

		break;

	}	// end switch


	if (ll_return == KOCC_GOOD) {

		ll_return = OpenOutput();

		// Copy Message Attributes from DLQ to Destination Queue
		if (ll_return == KOCC_GOOD) {

			pcQue->SetMsgID(m_cQue.GetMsgID());

			pcQue->SetCorlID(m_cQue.GetCorlIDBin());

			pcQue->SetExpiration(m_cQue.GetExpiration());

			pcQue->SetReplyToQueueName(m_cQue.GetReplyToQueueName());

			pcQue->SetMsgFormat(m_cQue.GetMsgFormat());

			pcQue->SetMsgType(ll_msg_type);

			pcQue->SetPersistence(m_cQue.GetPersistence());

			pcQue->SetFeedbackCode(m_cQue.GetFeedbackCode());

			pcQue->SetReportOptions(m_cQue.GetReportOptions());
			
			pcQue->SetPriority(m_cQue.GetPriority());


			// Synchpoint Option forced on for PUT, so no need
			// to set it.
			ll_return = pcQue->Put(m_cOutputBuffer);

			if(ll_return == KOCC_GOOD) {
				// Increment Counter for Appropriate Msg Type.
				(*plCounter)++;
			}
		}

	}

	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   CloseInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes Dead Letter Queue
//
//  Args:     ref BOOL flag indicating if Input was Opened successfully.
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::CloseInput(BOOL& rb_input_open) {

	long ll_return = KOCC_GOOD;

	if (rb_input_open) {
		ll_return = m_cQue.Close();

		rb_input_open = FALSE;
	}

	
	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   CloseOutput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes each output queue and shuts it down if it was
//            opened while processing the dead letter queue.
//
//  Args:     ref BOOL flag indicating if output was opened 
//                     successfully. (ignored)
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::CloseOutput(BOOL& rb_open_output) {

	long ll_return = KOCC_GOOD;

	if(m_cDataQue.IsOpen()) {
		m_cDataQue.Close();
		m_cDataQue.termSelf();
	}

	if(m_cCtrlQue.IsOpen()) {
		m_cCtrlQue.Close();
		m_cCtrlQue.termSelf();
	}

	if(m_cReplyQue.IsOpen()) {
		m_cReplyQue.Close();
		m_cReplyQue.termSelf();
	}

	if(m_cErrorQue.IsOpen()) {
		m_cErrorQue.Close();
		m_cErrorQue.termSelf();
	}

	rb_open_output = FALSE;

	return ll_return;

}





//---------------------------------------------------------------------+
//  Method:   RemoveDeadLetterHeader
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Copies Message Contents from the Input Buffer to the
//            output buffer, removing the Dead Letter Header in the
//            process.
//
//            Values that were in the Dead Letter Header are stored
//            in Member Variables for use by Descendents.
//
//            It is assumed the Buffers are the same size since they
//            are initialized with the same input buffer size from the
//            user.
//
//  Args:     ref CBuffer	Output Buffer
//            ref CBuffer	Input Buffer
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//
//  History: 
//  03/24/99  DMT Created original version.
//---------------------------------------------------------------------+
long CDeadLetter::RemoveDeadLetterHeader(CBuffer& cOutput, 
										 CBuffer& cInput) {


	char    ls_format[MQ_FORMAT_LENGTH + 1];
	char    ls_dest_queue[MQ_Q_NAME_LENGTH + 1];
	char    ls_dest_queue_mgr[MQ_Q_MGR_NAME_LENGTH + 1];

	long	ll_return			= KOCC_GOOD;

	MQDLH*	lpmqdlh_dead_letter = NULL;


	memset(ls_format,         '\0', MQ_FORMAT_LENGTH     + 1);
	memset(ls_dest_queue,     '\0', MQ_Q_NAME_LENGTH     + 1);
	memset(ls_dest_queue_mgr, '\0', MQ_Q_MGR_NAME_LENGTH + 1);


	//-----------------------------------------------------------------+
	// Note:  memcpy does not support overlapping regions.  Therefore,
	//        we could not use memcpy to copy the Message data after
	//        the Dead Letter Header to the top of the buffer.
	//
	//        memmove does support overlapping regions, but for some
	//        reason it doesn't work on AIX.  
	//
	//        By having two separate buffers, we do not have overlapping
	//        regions and can use memcpy.
	//
	// Also:  It is assumed the Buffers are the same size since they
	//        are initialized with the same input buffer size from the
	//        user.
	//-----------------------------------------------------------------+

	// assume msg was not truncated and does have a Dead Letter Header

	lpmqdlh_dead_letter = (PMQDLH) cInput.getPointer();


	//-----------------------------------------------------------------+
	// Return Dead Reason Code
	//-----------------------------------------------------------------+
	m_lDeadReason = lpmqdlh_dead_letter->Reason;


	//-----------------------------------------------------------------+
	// Copy Format from Dead Letter Header to 
	// member variable.  The Dead Letter Header contains
	// the format of the original message.
	//-----------------------------------------------------------------+
	memcpy(ls_format, lpmqdlh_dead_letter->Format, MQ_FORMAT_LENGTH);
	m_sFormat = ls_format;


	//-----------------------------------------------------------------+
	// Copy Destination Queue Name from Dead Letter Header to 
	// member variable.  
	//-----------------------------------------------------------------+
	memcpy(ls_dest_queue, lpmqdlh_dead_letter->DestQName, 
			MQ_Q_NAME_LENGTH);

	m_sDestQueueName = ls_dest_queue;


	//-----------------------------------------------------------------+
	// Copy Destination Queue Mgr Name from Dead Letter Header to 
	// member variable.  
	//-----------------------------------------------------------------+
	memcpy(ls_dest_queue_mgr, lpmqdlh_dead_letter->DestQMgrName, 
			MQ_Q_MGR_NAME_LENGTH);

	m_sDestQueueMgrName = ls_dest_queue_mgr;


	//-----------------------------------------------------------------+
	// Copy Message Data after Dead Letter Header to beginning of
	// output buffer.
	//-----------------------------------------------------------------+
	memcpy(cOutput.getPointer(), 
	        cInput.getPointer() + sizeof(MQDLH),
			cOutput.getSize() - sizeof(MQDLH));

	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   CanDoFinalCommit
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Overrides Ancestor version so ancestor Execute won't 
//            commit a batch.  This final commit is not necessary 
//            since we force Batch Interval to 1 when processing
//            Dead Letter Queues.
//
//  Args:     None
//
//  Returns:  FALSE
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CDeadLetter::CanDoFinalCommit() {
	return FALSE;
}
