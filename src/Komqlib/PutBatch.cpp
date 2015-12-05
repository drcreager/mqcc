//---------------------------------------------------------------------+
//  File:     PutBatch.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CPutBatch
//            Class.
//  
//            This class represents the "Put Batch" 
//            algorithm (Mode 1).  It is a descendent of CPut.
//
//            The model here is that the Transfer is File to Queue and
//            involves a Data Queue and a Control Queue.
//
//  Dev Log:  Consider propagating Ancestor Methods down to this level.
//            to simplify maintenance, even if all they do is call
//            the ancestor level.
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "PutBatch.h"






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
CPutBatch::CPutBatch() : CPut() {

	is_InstanceName = "Put Batch Business Logic Layer";
	
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
CPutBatch::~CPutBatch() {
	
//	CPut::termSelf();

}








//long CPutSingle::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {
//
//
//	CTransfer::initSelf(v_pcParms, v_pcLog);
//	m_cQue.initSelf();
//	m_cFile.initSelf();
//
//	// Don't forget to set return codes
//	return 0;
//
//}





//long CPutSingle::termSelf() {
//
//	m_cFile.termSelf();
//	m_cQue.termSelf();
//
//	CTransfer::termSelf();
//
//	// Don't forget to set return codes
//	return 0;
//}








//---------------------------------------------------------------------+
//  Method:   CanDoFinalCommit
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Overrides Ancestor version so ancestor Execute won't 
//            commit a batch.  We need this classes Exceute method
//            to make sure the batch balances before we Put the 
//            Control Message.
//
//  Args:     None
//
//  Returns:  FALSE
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CPutBatch::CanDoFinalCommit() {
	return FALSE;
}







//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Asks ParmFile class to validate parameters required for
//            a "Put Batch".
//
//            - Must have Control Queue and File Names.
//
//            - Call CheckFileExistence to make sure Control File
//              exists before processing Data Queue.
//
//            - Initializes Member Variables.
//
//            m_pcParms Errors are reported in CTransfer::initSelf() 
//            after the CheckParms() call.  No need to report errors
//            here.
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
long CPutBatch::CheckParms() {

	long	ll_return = KOCC_GOOD;

	//-----------------------------------------------------------------+
	// Check Parameters required at ancestor levels.
	//-----------------------------------------------------------------+
	ll_return = CPut::CheckParms();


	//-----------------------------------------------------------------+
	// Check Parameters required for a "Put Batch"
	//-----------------------------------------------------------------+
	if (!(m_pcParms->IsValidCtrlQueueName() &&
	      m_pcParms->IsValidCtrlFileName())    ) {

		ll_return = KOCC_FAIL;
	}

	

	//-----------------------------------------------------------------+
	// If Okay, Check Filenames provided to make sure they don't 
	// exist if they aren't supposed to.  Don't want to process data
	// and then fail.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = CheckFileExistence();
	}


	//-----------------------------------------------------------------+
	// If we are okay here, all needed parameters are valid, so
	// set "generic" member variables for Execute with specific
	// "Data parameters.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {

		m_lBatchInterval		= m_pcParms->GetBatchInterval();
		m_sFileName				= (char*) m_pcParms->GetDataFileName();
		m_sQueueName			= (char*) m_pcParms->GetDataQueName();
		m_lSynchpoints          = 0;
		m_lExpectedMsgs			= 0;
		m_lTotalMsgsProcessed	= 0;
		m_lUncommittedMsgs      = 0;
		m_lMsgsProcessed		= 0;
		m_bRestart				= m_pcParms->IsRestart();

		// User must specify MsgID for a PUT, We append timestamp
		// if user provided value <= 10 chars.
		m_sBatchID				= (char*) m_pcParms->GetMessageID();

		// User option of Correl ID for PUT not valid
		//m_lCorrelID				= m_pcParms->GetCorrelID();

	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   GetControlMsg
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Gets Message Count from Control File and Stores in 
//            in the Parameter Class.
//
//            If the Control File is 0 bytes, a Message Count of zero
//            is assumed.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful or if the file contains 0 bytes.
//
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPutBatch::GetControlMsg() {

	long		ll_return		= KOCC_GOOD;
	BOOL		lb_file_open	= FALSE;
	CCtlFile	cCtlFile;
	CString		ls_msg;


	ll_return = cCtlFile.initSelf(m_pcLog,
							   (char*) m_pcParms->GetCtlFileName());



	if (ll_return == KOCC_GOOD) {
		ll_return = cCtlFile.Open(ios::in | ios::nocreate);
	}


	if (ll_return == KOCC_GOOD) {

		lb_file_open = TRUE;
		ll_return = cCtlFile.Read(m_cBuffer);

	}


	if (ll_return == KOCC_GOOD) {

		m_pcParms->SetExpectedMsgs(cCtlFile.GetNumMsgs());

	}
	else if (ll_return == KOCC_WARNING) {

		m_pcParms->SetExpectedMsgs(0);
		
		ll_return = KOCC_GOOD;

		ls_msg << "No control messages found on control file.  "
			   << "Nothing do to.";

		m_pcLog->LogError(KORC_NO_CONTROL_MSG, ll_return,
				ls_msg,
				"CPutBatch::GetControlMsg:1",
				is_InstanceName.getPointer());
	}

	
	if (lb_file_open) {
		cCtlFile.Close();
	}

	cCtlFile.termSelf();


	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   SendData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Before Calling CPut::SendData, this checks to see if
//            the expected count is 0.  If so, this sets an error if
//            AutoBalance is ON.
//
//            If Control File is 0 bytes, or if a Control Message
//            exists with a count of zero bytes, this lets us
//            detect if we have data to send.  If so, we'll
//            detect the error after the data has been "received" 
//            but before it's been sent.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD normally
//            KOCC_FAIL if m_lExpectedMsgs = 0 or if 
//            CPut::SendData returned an error.
//  
//  Dev Log:  
//  History: 
//  04/24/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPutBatch::SendData() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	if ((m_lExpectedMsgs == 0) && m_bAutoBalance) {

		ll_return = KOCC_FAIL;

		ls_msg << "Batch Out of Balance!  Expected number of"
			   << " messages is 0, but"
			   << " Data Messages exist and AutoBalance is turned on.";

		m_pcLog->LogError(KORC_BATCH_OUT_OF_BALANCE, ll_return,
						ls_msg,
						"CPutBatch::SendData::1",
						is_InstanceName.getPointer());

	}
	else {
		ll_return = CPut::SendData();
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Executes batch Transfer from Files to Queues.
//
//            - Reads first Control Message to get Expected Count by
//              calling GetControlMsg().
//
//            - Calls Ancestor Execute to transfer Data File to Data
//              Queue.
//
//            - Checks Balancing
//
//            - Executes Control Queue Transfer
//
//            - Commits
//
//            - Gets Run Statistics from member variables and stores
//              in Parameter Class so CCmdInterface can report on 
//              them.
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
long CPutBatch::Execute() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	//-----------------------------------------------------------------+	
	// If not Restarting, Browse Control File to get Expected Msg Count.
	//-----------------------------------------------------------------+	
	if (!m_bRestart) {
		ll_return = GetControlMsg();
	}



	if (ll_return == KOCC_GOOD) {
		//-------------------------------------------------------------+
		// Set Member Variables that Execute Method uses, these are 
		// "generic" and aren't data or control queue specific.
		//-------------------------------------------------------------+	
		m_lExpectedMsgs = m_pcParms->GetExpectedMsgs();
		m_sBatchID		= (char*) m_pcParms->GetMessageID();
		m_bSequence		= TRUE;


		//-------------------------------------------------------------+
		// Transfer Data Queue
		//-------------------------------------------------------------+	
		ll_return = CPut::Execute();
	}


	//-----------------------------------------------------------------+
	// Copy Data Queue Run Stats to Parameter Class
	//-----------------------------------------------------------------+
	m_pcParms->SetUncommittedDataMsgs(m_lUncommittedMsgs);
	m_pcParms->SetDataMsgsProcessed(m_lMsgsProcessed);
	m_pcParms->SetTotalDataMsgsProcessed(m_lTotalMsgsProcessed + 
										 m_lMsgsProcessed);
	m_pcParms->SetSynchpoints(m_lSynchpoints);


	//-----------------------------------------------------------------+
	// Are we Error Free and in Balance or 
	//        Error Free and AutoBalance is turned off?
	//-----------------------------------------------------------------+
	if ((ll_return == KOCC_GOOD) &&
		((m_lTotalMsgsProcessed + m_lMsgsProcessed + 
		  m_lUncommittedMsgs == m_lExpectedMsgs) ||
		 !m_bAutoBalance)) {



		if (!m_bAutoBalance) {

			ls_msg = "-- AutoBalance is Off.  Out of balance batch ";
		    ls_msg += "will still be processed.";

			m_pcLog->WriteLog(ls_msg);
		}


		//-------------------------------------------------------------+
		// Prepare to Process Control Queue, Reset Member Variables
		// for Control Queue.  
		//
		// Since "Restart" doesn't apply for a control queue, set 
		// m_bRestart to FALSE, Batch Interval to zero, and Expected
		// count to 1.
		//-------------------------------------------------------------+
		m_lBatchInterval		= 0;	// for control queue
		m_sFileName				= (char*) m_pcParms->GetCtlFileName();
		m_sQueueName			= (char*) m_pcParms->GetCtlQueName();

		m_cQue.SetMsgType(KOMT_CONTROL);

//		m_lExpectedMsgs			= 1;	// for control queue
		m_lTotalMsgsProcessed	= 0;
		m_lMsgsProcessed		= 0;
		m_lUncommittedMsgs      = 0;
		m_bRestart				= 0;	// for control queue

		// Batch ID should already be set, no need to reset it.
		// want to continue putting msgs with the same 
		// batch ID.

		
		// We will leave Corellation ID alone because we want
		// to continue sequencing with the control message.
		// We'll leave m_bSequence set to true, as well.

		
		//-------------------------------------------------------------+
		// Execute Control Queue Transfer if we have at least 1 msg
		// in the batch.
		//-------------------------------------------------------------+
		if (m_lExpectedMsgs > 0) {
			ll_return = CPut::Execute();
		}


		//-------------------------------------------------------------+
		// Get Interim Control Counts 
		// In case we had an error, we can at least report these.
		//-------------------------------------------------------------+
		m_pcParms->SetUncommittedCtrlMsgs(m_lUncommittedMsgs);
		m_pcParms->SetCtrlMsgsProcessed(m_lMsgsProcessed);
		m_pcParms->SetTotalCtrlMsgsProcessed(m_lTotalMsgsProcessed +
			                                 m_lMsgsProcessed);
		m_pcParms->SetSynchpoints(m_lSynchpoints);


		if (ll_return == KOCC_GOOD ) {

			//---------------------------------------------------------+
			// If OK, do Final Commit.
			//---------------------------------------------------------+
			ll_return = CommitWork();

			if (ll_return == KOCC_GOOD) {

				//-----------------------------------------------------+
				// Get Good Final Ctrl Queue Run Stats
				//-----------------------------------------------------+
				m_pcParms->SetUncommittedCtrlMsgs(m_lUncommittedMsgs);
				m_pcParms->SetCtrlMsgsProcessed(m_lMsgsProcessed);
				m_pcParms->SetTotalCtrlMsgsProcessed(
					m_lTotalMsgsProcessed + m_lMsgsProcessed);
				m_pcParms->SetSynchpoints(m_lSynchpoints);

				//-----------------------------------------------------+
				// Since we know we had a good final commit, We can 
				// Add uncommitted data messages to committed data
				// msgs.
				//-----------------------------------------------------+
				m_pcParms->SetDataMsgsProcessed(
					m_pcParms->GetDataMsgsProcessed() +
					m_pcParms->GetUncommittedDataMsgs());

				m_pcParms->SetTotalDataMsgsProcessed(
					m_pcParms->GetTotalDataMsgsProcessed() + 
					m_pcParms->GetUncommittedDataMsgs());

				m_pcParms->SetUncommittedDataMsgs(0);


				//-----------------------------------------------------+
				// Call Ancestor to remove Restart File from Data Queue
				// processing.
				//-----------------------------------------------------+
				CTransfer::ShutdownRestart();

			}
			else {
				//-----------------------------------------------------+
				// Final Commit Failed
				//-----------------------------------------------------+
				ls_msg = "Error on Final Commit!";
				m_pcLog->LogError(KORC_COMMIT_FAILED, ll_return,
								ls_msg,
								"CPutBatch::Execute::1",
								is_InstanceName.getPointer());

				// Assuming Rollback here is pointless if Commit Fails.

			}
		}
		else {
			// Error on Control Execute 
			// Ancestor Should've Rolled Back

			//ls_msg = "Error on Control Execute!";
			//m_pcLog->LogError(KORC_UNSPECIFIED, ll_return,
			//				ls_msg,
			//				"CPutBatch::Execute::2",
			//				is_InstanceName.getPointer());

		}
	}
	else {

		//-------------------------------------------------------------+
		// Error on Data Execute or Data Queue Out of Balance.
		//-------------------------------------------------------------+
		if (ll_return == KOCC_GOOD) {

			//---------------------------------------------------------+
			// Out of Balance
			//---------------------------------------------------------+
			ls_msg << "Batch Out of Balance!.  Expected "
				   << m_lExpectedMsgs
				   << " messages but processed "
				   << m_lTotalMsgsProcessed + m_lMsgsProcessed + 
				      m_lUncommittedMsgs
				   << ".  Rolling back uncommitted transactions.";

			ll_return = KOCC_FAIL;

			m_pcLog->LogError(KORC_BATCH_OUT_OF_BALANCE, ll_return,
							ls_msg,
							"PutBatch::Execute:1",
							is_InstanceName.getPointer());

			//---------------------------------------------------------+
			// rollback
			//---------------------------------------------------------+
			m_cQueMgr.Rollback();
		}
		else {
			//---------------------------------------------------------+
			// Error on Data Execute
			// Ancestor would've rolled back
			//---------------------------------------------------------+
			
			//ls_msg = "Error on Data Execute!";
			//m_pcLog->LogError(KORC_UNSPECIFIED, ll_return,
			//				ls_msg,
			//				"CPutBatch::Execute::3",
			//				is_InstanceName.getPointer());
		}
	}

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   ShutdownRestart
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Dummy to override ancestor.  Don't want to remove 
//            Restart file until after we've balanced.
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
long CPutBatch::ShutdownRestart() {

	return KOCC_GOOD;
}







//---------------------------------------------------------------------+
//  Method:   CheckFileExistence
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Want to make sure Control File does exist before we begin 
//            processing Data Queue.
//
//            This is the opposite of our Check on the Get, when we want
//            to make sure the file does NOT exist.
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
long CPutBatch::CheckFileExistence() {

	long	 ll_return = KOCC_GOOD;
	CString	 ls_msg;
	CMsgFile cFile;

	ll_return = cFile.initSelf(m_pcLog, 
		                      (char*) m_pcParms->GetCtlFileName());

	if (ll_return == KOCC_GOOD) {

		// Open fails if the file does NOT exist
		ll_return = cFile.Open(ios::in | ios::nocreate);
	}

	// close file
	cFile.termSelf();

	return ll_return;

}