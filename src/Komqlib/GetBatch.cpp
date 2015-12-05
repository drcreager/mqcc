//---------------------------------------------------------------------+
//  File:     GetBatch.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CGetBatch
//            Class.
//  
//            This class represents the "Get Batch" 
//            algorithm (Mode 1).  It is a descendent of CGet.
//
//            The model here is that the Transfer is Queue to File and
//            involves a Data Queue and a Control Queue.
//
//  Dev Log:  Consider propagating Ancestor Methods down to this level.
//            to simplify maintenance, even if all they do is call
//            the ancestor level.
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "GetBatch.h"






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
CGetBatch::CGetBatch() : CGet() {

	is_InstanceName = "Get Batch Business Logic Layer";
	
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
CGetBatch::~CGetBatch() {
	
//	CGet::termSelf();

}











//---------------------------------------------------------------------+
//  Method:   CanDoFinalCommit
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Overrides Ancestor version so ancestor Execute won't 
//            commit a batch.  We need this classes Exceute method
//            to make sure the batch balances and pop off the 
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
BOOL CGetBatch::CanDoFinalCommit() {
	return FALSE;
}







//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor Method on CGet.
//
//            Checks for Parameters Required at the CGetBatch level.
//
//            - Must have valid Control Queue Name
//            - Must have valid Control File Name
//            - Calls CheckFileExistence to see if Ctrl File already
//              exists.
//
//            Initialzes class Member variables for a Get Batch 
//            operation.
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
long CGetBatch::CheckParms() {

	long	ll_return = KOCC_GOOD;

	//-----------------------------------------------------------------+
	// Check Parameters required at ancestor levels.
	//-----------------------------------------------------------------+
	ll_return = CGet::CheckParms();


	//-----------------------------------------------------------------+
	// Check Parameters required for a "Get Batch"
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
		m_lUncommittedMsgs      = 0;
		m_lTotalMsgsProcessed	= 0;
		m_lMsgsProcessed		= 0;
		m_bRestart				= m_pcParms->IsRestart();

		if(m_pcParms->HasAdminAccess()) {
			m_sBatchID			= (char*) m_pcParms->GetMessageID();
			m_lCorrelID			= m_pcParms->GetCorrelID();
		}

	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   GetControlMsg
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGetBatch::GetControlMsg() {

	long		ll_return		= KOCC_GOOD;
	BOOL		lb_queue_open	= FALSE;
	CCtlQue	    cCtlQue;
	CString		ls_msg;


	ll_return = cCtlQue.initSelf(m_pcLog,
								 &m_cQueMgr,
								 (char*) m_pcParms->GetCtlQueName(),
								 m_pcParms->GetFailOnQuiescing(),
								 m_pcParms->GetTranslate());



	if (ll_return == KOCC_GOOD) {
		ll_return = cCtlQue.QOpen(CQue::Browse);
	}


	if (ll_return == KOCC_GOOD) {

		lb_queue_open = TRUE;
		ll_return = cCtlQue.BrowseFirst(m_cBuffer,
									m_pcParms->GetMessageID(),
									m_pcParms->GetCorrelID());

	}


	if (ll_return == KOCC_GOOD) {

		if (m_pcParms->GetMessageID() == NULL) {
			m_pcParms->SetMessageID((char*) cCtlQue.GetMsgID());
		}


		if (m_pcParms->GetCorrelID() == NULL) {
			m_pcParms->SetCorrelID(cCtlQue.GetCorlIDBin());
		}

		m_pcParms->SetExpectedMsgs(cCtlQue.GetNumDataMsg());
	}
	else if (ll_return == KOCC_WARNING) {
		// no control msg found 

        // let it remain as a warning if EmptyQueueRC not specified

		if (m_pcParms->GetEmptyQueueRC() != 0){
			ll_return = m_pcParms->GetEmptyQueueRC();
		} // end if

		ls_msg << "No control messages found on control queue.  "
			   << "Nothing do to." 
			   << "\n*** MsgId: " << (char*) m_pcParms->GetMessageID() 
			   << "\n*** Corr:  " << (char*) m_pcParms->GetCorrelIDStr();

		m_pcLog->LogError(KORC_NO_CONTROL_MSG, ll_return,
				ls_msg,
				"CGetBatch::GetControlMsg:1",
				is_InstanceName.getPointer());
	}

	
	if (lb_queue_open) {
		cCtlQue.Close();
	}

	cCtlQue.termSelf();


	return ll_return;

}








//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     GetBatch Execute.  Actually calls Ancestor Execute
//            twice, once to transfer the Data Queue and if no
//            errors and everything balances, again for the
//            control queue.
//
//            - Browses Control Queue to get Expected Message Count
//              and establish batch ID if not in restart mode.
//
//            - Transfer Data Queue.
//
//            - Check Balancing if AutoBalancing
//
//            - Reset Member Variables for Control Execute
//
//            - Transfer Control Queue
//
//            - If OK, Commit
//
//            - Shutdown Restart, because we didn't want ancestor to
//              do it.
//
//            - Store run statistics from Member variables in 
//              Parameter Class so CCmdInterface can report them.
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
long CGetBatch::Execute() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	//-----------------------------------------------------------------+	
	// If not Restarting, Browse Control Queue to Get Batch ID and
	// Expected Msg Count.
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
		ll_return = CGet::Execute();
	}


	//-----------------------------------------------------------------+
	// Copy Data Queue Run Stats to Parameter Class
	//-----------------------------------------------------------------+
	m_pcParms->SetUncommittedDataMsgs(m_lUncommittedMsgs);
	m_pcParms->SetDataMsgsProcessed(m_lMsgsProcessed);
	m_pcParms->SetTotalDataMsgsProcessed(m_lTotalMsgsProcessed +
		                                 m_lMsgsProcessed);
	m_pcParms->SetSynchpoints(m_lSynchpoints);

	m_pcParms->SetMessageID(m_sBatchID.getPointer());
	m_pcParms->SetExpectedMsgs(m_lExpectedMsgs);



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
		m_lExpectedMsgs			= 1;	// for control queue
		m_lUncommittedMsgs      = 0;
		m_lTotalMsgsProcessed	= 0;
		m_lMsgsProcessed		= 0;
		m_bRestart				= 0;	// for control queue

		//Batch ID should already be set, no need to reset it.
		//m_sBatchID				= m_pcParms->GetMsgID();

		//-------------------------------------------------------------+
		// The old version of KOMQPUT used the Correllation ID of the
		// last data message when putting the Control Message.  The
		// new version of KOMQPUT continues incrementing the 
		// the Correllation ID to be compatible with Dan's version.
		//
		// If the batch was Put with the new version of KOMQPUT, we
		// could  leave Correlation ID alone and let it continue to
		// sequence.  By doing so, we wouldn't be able to complete
		// a batch Put with the old Put utility.
		//
		// In order to get a batch from the old or new Put utility,
		// restore the Correlation ID we got when we first browsed
		// the control message and turn sequencing off.
		//-------------------------------------------------------------+

		// Restore previously "browsed" Correllation ID so we are
		// sure we get the same Control Message.
		m_lCorrelID				= m_pcParms->GetCorrelID();

		// Turn off Get Sequencing;
		m_bSequence				= FALSE;


		//-------------------------------------------------------------+
		// Execute Control Queue Transfer
		//-------------------------------------------------------------+
		ll_return = CGet::Execute();


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
				m_pcParms->SetTotalCtrlMsgsProcessed(m_lTotalMsgsProcessed +
					                                 m_lMsgsProcessed);
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
								"CGetBatch::Execute::1",
								is_InstanceName.getPointer());

				// Assuming there's no point in Rolling back if the 
				// final commit failed.

			}
		}
		else {
			// Error on Control Execute 
			// Ancestor Should've Rolled Back
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
				   << ". Rolling back uncommitted transactions.";

			ll_return = KOCC_FAIL;

			m_pcLog->LogError(KORC_BATCH_OUT_OF_BALANCE, ll_return,
							ls_msg,
							"GetBatch::Execute:1",
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
//  Args:     TBD
//
//  Returns:  KOCC_GOOD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGetBatch::ShutdownRestart() {

	return KOCC_GOOD;
}






//---------------------------------------------------------------------+
//  Method:   CheckFileExistence
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     The existence of the Data and Restart file is checked 
//            When the first Excecute pass begins.  This method
//            explicitly checks for the Control File.  We want to make 
//            sure Control File doesn't exist before we begin 
//            processing the Data Queue.
//
//            We try to open the Control File for output with the 
//            no-replace option.  If it's there, the open fails.
//
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if we can open the Control file and it doesn't
//                      exist.
//
//            KOCC_FAIL if we cannot open the control file, or if the
//                      control file already exists.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGetBatch::CheckFileExistence() {

	long	 ll_return = KOCC_GOOD;
	CString	 ls_msg;
	CMsgFile cFile;

	ll_return = cFile.initSelf(m_pcLog, 
		                      (char*) m_pcParms->GetCtlFileName());

	if (ll_return == KOCC_GOOD) {

		// Open fails if the file exists
		ll_return = cFile.Open(ios::out | ios::noreplace);
	}

	// close file
	cFile.termSelf();

	return ll_return;

}
