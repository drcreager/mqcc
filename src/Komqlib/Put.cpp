//---------------------------------------------------------------------+
//  File:     Put.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CPut
//            Class.
//  
//            This class represents the generic "Put" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CPutBatch (Mode 1) and CPutSingle (Mode 2)
//            algorithms.
//
//            The model here is that the Transfer is File to Queue.
//
//  Dev Log: 
//  History: 
//  03/22/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "Put.h"





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
CPut::CPut() : CTransfer() {
	m_SysCmd = new CSysCmd();
	is_InstanceName = "Put Business Logic Layer";	
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
CPut::~CPut() {
//	termSelf();
delete m_SysCmd;
}








//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls ancestor.
//
//            Calls GenerateBatchID to generate the BATCH ID.
//
//            Initializes Queue Class with Batch ID, 
//            ReplyToQueue Name, etc.
//
//
//  Args:     pointer to Parameter Class
//            pointer to Log Class
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {

	long ll_return = KOCC_GOOD;

	ll_return = CTransfer::initSelf(v_pcParms, v_pcLog);


	if (ll_return == KOCC_GOOD) {

		// Everything initialized and Parameters Validated

		// Calculate Batch ID.
		// Set the Following in the Queue Class.
		ll_return = GenerateBatchID(m_sBatchID);
		m_pcParms->SetMessageID(m_sBatchID.getPointer());

		m_cQue.SetMsgID(m_sBatchID.getPointer());

		// ReplyToQueueName
		m_cQue.SetReplyToQueueName((char*)m_pcParms->GetReplyToQName());

		// MsgType
		m_cQue.SetMsgType(KOMT_DATA);

		// MsgFormat
		m_cQue.SetMsgFormat((char*) m_pcParms->GetMessageFormat());

		// Expiration Interval
		m_cQue.SetExpiration(m_pcParms->GetExpirationInterval());

	}

	return ll_return;

}




//---------------------------------------------------------------------+
//  Method:   initExecute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Do Pre-Execute initialization for Queue and File 
//            objects.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs
//  
//  Dev Log:  
//  History: 
//  03/13/99 DMT Created original version.
//  01/08/00 DRC Modified to include Return Receipt Indicators
//---------------------------------------------------------------------+
long CPut::initExecute() {

	long ll_return = KOCC_GOOD;


	ll_return = m_cQue.initSelf(m_pcLog, &m_cQueMgr, 
		                        m_sQueueName.getPointer(),
								m_pcParms->GetFailOnQuiescing(),
#ifdef _ver2_
								m_pcParms->isTranslate(),
								m_pcParms->isLogReceipt(),
                                                                m_pcParms->isDataReceipt());
#else
								m_pcParms->GetTranslate());
#endif

	if (ll_return == KOCC_GOOD) {
		ll_return = m_cFile.initSelf(m_pcLog, m_sFileName.getPointer());
	}

	return ll_return;
}








//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Nothing this level, calls Ancestor.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::termSelf() {

	return CTransfer::termSelf();

}







//---------------------------------------------------------------------+
//  Method:   termExecute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Do Post-Execute shutdown for Queue and File 
//            objects.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::termExecute() {

	long	ll_return = KOCC_GOOD;

	ll_return = m_cFile.termSelf();

	if (ll_return == KOCC_GOOD) {
		m_cQue.termSelf();
	}

	return ll_return;
}








//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Asks ParmFile class to validate parameters required for
//            any PUT.
//
//            - Must have Restart File Name if Batch Interval > 0.
//            - Must have Data File Name.
//            - Must have good Batch Interval.
//            - Must have valid Message ID.
//            - Must have valid ReplyToQueue Name.
//            - Must have valid Expiration Interval.
//            - Must have valid Message Format.
//
//            m_pcParms Errors are logged after CheckParms call in
//            CTransfer::initSelf(), so no need to report here.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  01)       - On Return_Receipts_specified,set KeyTranslate=On.
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::CheckParms() {

	long	ll_return = KOCC_GOOD;

	// Call Ancestor to validate parms needed at "Transfer" level.
	ll_return = CTransfer::CheckParms();

	if (ll_return == KOCC_GOOD) {

		// Validate Parms Needed for any GET or PUT.

		if ((m_pcParms->GetBatchInterval() > 0   )   &&
			(!m_pcParms->IsValidRestartFileName())   ) {		
			ll_return = KOCC_FAIL;
		}

		if (!m_pcParms->IsValidDataFileName()) {		
			ll_return = KOCC_FAIL;
		}

		if (!m_pcParms->IsValidBatchInterval()) {
			ll_return = KOCC_FAIL;
		}

		// Validate Parms Needed for any PUT
		// NULL is not allowed for MessageID with PUTs
		if (!m_pcParms->IsValidMsgID(FALSE)) {
			ll_return = KOCC_FAIL;
		}

		if (!m_pcParms->IsValidReplyToQueueName()) {
			ll_return = KOCC_FAIL;
		}

		if (!m_pcParms->IsValidExpirationInterval()) {
			ll_return = KOCC_FAIL;
		}

		if (!m_pcParms->IsValidMessageFormat()) {
			ll_return = KOCC_FAIL;
		}

             #ifdef _ver2_
		if (!m_pcParms->IsValidReceiptRequest()) {
			ll_return = KOCC_FAIL;
		}
             #endif
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   OpenInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens File for Input.  File must exist.
//
//  Args:     BOOL ref to Input is Open flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::OpenInput(BOOL& rb_input_open) {

	long	ll_return = KOCC_FAIL;
	CString	ls_msg;

	ll_return = m_cFile.Open((ios::open_mode) ios::in | ios::nocreate);

	rb_input_open = (ll_return == KOCC_GOOD);

	return ll_return;

}










//---------------------------------------------------------------------+
//  Method:   OpenOutput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens Queue for Output
//
//  Args:     BOOL - ref to Output is Open flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::OpenOutput(BOOL& rb_output_open) {

	long ll_return = KOCC_FAIL;

	ll_return = m_cQue.QOpen(CQue::Output);



	rb_output_open = (ll_return == KOCC_GOOD);

	return ll_return;
}





//---------------------------------------------------------------------+
//  Method:   EstablishRestart
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Extends EstablishRestart on CTransfer.  Put processing
//            establishes a BatchID during initSelf processing and
//            stores the calculated batch id in the parameter class 
//            and in the Queue class.
//
//            If we are in restart, we need to take the Batch ID
//            from the restart file and update the Batch ID in 
//            the parameter class and the Queue class.
//
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/23/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::EstablishRestart() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	// call ancestor script and let it do it's thing
	ll_return = CTransfer::EstablishRestart();

	// if ancestor was good
	if (ll_return == KOCC_GOOD) {

		// if We need Restart 'cause we're doing interim commits and
		//    Restart is enabled (which it would be for Puts) and
		//    we were run with the restart switch, the ancestor
		//    updated m_sBatchID with the value from the restart
		//    file.  Now, we need to update the Parmameter class
		//    and the Queue class so we continue processing the
		//    same batch id.  Otherwise, this run would use a new
		//    ID.
		if (m_lBatchInterval > 0 &&
			m_bEnableRestart     &&
			m_bRestart              ) {


			m_pcParms->SetMessageID(m_sBatchID.getPointer());

			m_cQue.SetMsgID(m_sBatchID.getPointer());

			m_pcParms->SetExpectedMsgs(m_lExpectedMsgs);

		}

	}

	return ll_return;

}





//---------------------------------------------------------------------+
//  Method:   ReceiveData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Reads a line from the input file and stores in in 
//            the Buffer.
//
//  Args:     vbSequence	see above
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::ReceiveData() {

	long ll_return		= KOCC_GOOD;

	ll_return = m_cFile.Read(m_cBuffer);

	return ll_return;

}










//---------------------------------------------------------------------+
//  Method:   SendData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Adds 1 to Correl ID, updates Queue Correl ID property,
//            Asks Queue to Put Message whose data is in Buffer.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::SendData() {

	long	ll_return		= KOCC_FAIL;


	if (m_bSequence) {
		m_lCorrelID++;
	}
	
	m_cQue.SetCorlID(m_lCorrelID);

	ll_return = m_cQue.Put(m_cBuffer);

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   CloseInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes Input file.
//
//  Args:     BOOL - ref to Input is Open flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::CloseInput(BOOL& rb_input_open) {

	long ll_return = KOCC_GOOD;

	if (rb_input_open) {
		ll_return = m_cFile.Close();

		rb_input_open = FALSE;
	}

	
	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   CloseOutput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes output Queue.
//
//  Args:     BOOL - ref to Output is Open flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPut::CloseOutput(BOOL& rb_output_open) {

	long ll_return = KOCC_GOOD;

	if (rb_output_open) {
		ll_return = m_cQue.Close();

		rb_output_open = FALSE;
	}

	return ll_return;

}





//---------------------------------------------------------------------+
//  Method:   GenerateBatchID
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     If user provided Msg ID is <= 10 characters, append
//            timestamp in YYYYMMDDHHMMSS format.
//
//  Args:     CString - variable to receive new Batch ID
//
//  Returns:  KOCC_GOOD if successful; KOCC_FAIL if time failed
//  
//  Dev Log:  In Visual Studio 97, there is an "April 2001" issue
//            which will cause "localtime" to be off by 1 hour for 
//            one week in the month of April, 2001.  This bug is fixed
//            in a Service Pack to Visual Studio 98.
//
//            For our uses, this should not cause a problem.
//
//  History: 
//  03/23/99  DMT Created original version.
//  04/18/02  DRC Forced prefix len to 10 characters when appending 
//                the date/time stamp for consistency with COBOL 
//                implementation.
//  05/08/02  DRC Force a 1 second wait after generating each BatchID
//                to prevent duplicate ID being generated on very fast
//                hosts.
//---------------------------------------------------------------------+
long CPut::GenerateBatchID(CString& v_cBatchID) {

	long	ll_return	= KOCC_GOOD;
	CString	ls_msg;
	CString l_Value;
	time_t	lt_time;
	tm*		lstr_time	= NULL;
	char	lpc_string[15];			// YYYYMMDDHHMMSS string
									// If you change this size, 
									// make sure you change the
									// size passed to strftime
									// below

	if(strlen(m_pcParms->GetMessageID()) > 10) {

		ls_msg << "-- Message ID '"
			   << (char*) m_pcParms->GetMessageID()
			   << "' has more than 10 characters.\n"
		       << "-- Batch ID will NOT have a timestamp added.";

		m_pcLog->WriteLog(ls_msg, LOGCONSOLE + LOGFILE);
		
		v_cBatchID = (char*) m_pcParms->GetMessageID();
	}
	else {
		//
		//  right fill MessageID with zeroes to 10 characters
		//
		l_Value = (char*) m_pcParms->GetMessageID();
		while (l_Value.vlen() < 10) l_Value += "0";
        //
		//  append the date/time stamp
		//
		time(&lt_time);
		lstr_time = localtime(&lt_time);

		if (lstr_time != NULL) {

			// formats string to "YYYYMMDDHHMMSS"

			if(strftime(lpc_string, 15, "%Y" "%m%d%H" "%M%S",
				lstr_time) == 0) {

				ll_return = KOCC_FAIL;
				ls_msg = "Error Formatting Timestamp.";

				m_pcLog->LogError(KORC_ERROR_FORMATTING_TIMESTMP, ll_return,
							ls_msg,
							"CPut::GenerateBatchID:1",
							is_InstanceName.getPointer());
			}
		}
		else {

			ll_return = KOCC_FAIL;
			ls_msg = "Error determining current time.";

			m_pcLog->LogError(KORC_ERROR_FORMATTING_TIMESTMP, ll_return,
							ls_msg,
							"CPut::GenerateBatchID:2",
							is_InstanceName.getPointer());
		}

		if (ll_return == KOCC_GOOD) {
			//v_cBatchID =  (char*) m_pcParms->GetMessageID();
			v_cBatchID = l_Value;
			v_cBatchID += lpc_string;

		}
	}
	//  Prevent duplicate Batch Ids on fast processors
	m_SysCmd->Sleep(1000L);
	return ll_return;
}
