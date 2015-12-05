//---------------------------------------------------------------------+
//  File:     Transfer.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CTransfer
//            Class.
//  
//            This class represents the generic "Transfer" 
//            algorithm.  It is the
//            ancestor of the CPut, CGet, and CDeadLetter
//            algorithms.
//
//            At this level, it is not known if the transfer is 
//            file to queue or queue to file.
//
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//  11/09/00 DRC Added the GetStatus method.
//  04/12/02 DRC Added checking for empty source condition and setting 
//               a user specified return code.
//---------------------------------------------------------------------+
#include "Transfer.h"



//---------------------------------------------------------------------+
//  Method:   CanDoFinalCommit
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Tells Execute method if it should commit queue activity
//            overridden by descendents that work with pairs of queues.
//
//  Args:     None
//
//  Returns:  Always returns TRUE
//  
//  Dev Log:  Later in the development cycle, I realized there were
//            needed "options" I left out of the design.  In these
//            cases, I used member variables that descendents could
//            set to modify ancestor behavior.  This "CanDoFinalCommit"
//            option should probably be a member variable to be 
//            consistent.
//
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CTransfer::CanDoFinalCommit() {
	return TRUE;
}








//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Checks for Valid Parameters needed for all Transfers.
//
//            Asks the Parameter Class to check for the following:
//            - Valid Queue Manager Name
//            - Valid Buffer Size
//            - Valid Data Queue Name (see Dev Log Below)
//
//            m_pcParms errors are reported after the CheckParms call
//            in CTransfer::initSelf()
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  The "Batch Interval == 0 && /r" edit might not be valid.
//
//            Data Queue Name is not required for Dead Letter Mode 2,
//            (and not necessarly for Dead Letter Mode 1).  Its 
//            check should be pushed down to CPut and CGet.
//        
//            Note: Observed on 3/20/2002 that the invocation of 
//                  m_pcLog->LogError in the aforementioned check 
//                  contains a bug.  It triggers an access exception in 
//                  the following call stack:
//                     strtok() line 97 + 3 bytes
//                     CLog::WrapLine() line 783 + 14 bytes
//                     CLog::LogError() line 421
//
//  History: 
//  03/12/99 DMT Created original version.
//  03/20/02 DRC Commented out the Batch Interval check described in the
//               dev note because it is checking values that are set in 
//               descendants after this check is executed. Because of the
//               doubt of its validity expressed in the dev note I chose 
//               to simply remove it for the time being. 
//---------------------------------------------------------------------+
long CTransfer::CheckParms() {

	long ll_return = KOCC_GOOD;


	if (!m_pcParms->IsValidQueueMgrName()) {		
		ll_return = KOCC_FAIL;
	}


	if (!m_pcParms->IsValidBufferSize()) {
		ll_return = KOCC_FAIL;
	}


	if (!m_pcParms->IsValidDataQueueName()) {
		ll_return = KOCC_FAIL;
	}


    /*
     *  03/20/02 DRC Commented out this check because it is checking values 
	 *               that are set in descendants after this check is executed. 
     *               Because of the doubt of its validity expressed in the
     *               dev note I chose to simply remove it for the time being.
	 *
  	if ((m_lBatchInterval == 0) && m_bRestart) {
		// this is an error

		m_pcLog->LogError(KORC_INVLD_BATCH_INTVL_RESTART, KOCC_FAIL,
			"Cannot use Restart Mode when Batch Interval is 0.",
			"CTransfer::CheckParms:1",
			is_InstanceName.getPointer());

		ll_return = KOCC_FAIL;
	}
	*/

	m_bAutoBalance = m_pcParms->DoAutoBalance();

	// At this point, we know we need the following, but
	// don't know if they are "data" or "control"
	//
	// BatchInterval
	// FileName
	// QueueName
	// TotalMsgsProcessed
	// MsgsProcessed	(between batch intervals)
	// InRestartMode
	// MsgID?
	// CorrelID?

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   GetStatus
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Retrieve the data values from CParameter for  a Transfer
//            operation and format a string that is suitable for display.
//
//  Args:     None
//
//  Returns:  char* = address of a status string.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CTransfer::GetStatus() {
	m_cStatus = "";
	m_cStatus << "\n"
	<< "Run Statistics for Batch '" << (char*) m_pcParms->GetMessageID() << "':\n"
	<< "\n"
	<< "Expected Data Msgs Processed:  " << m_pcParms->GetExpectedMsgs()           << "\n"
	<< "       Uncommitted Data Msgs:  " << m_pcParms->GetUncommittedDataMsgs()    << "\n"
	<< "         Data Msgs Processed:  " << m_pcParms->GetDataMsgsProcessed()      << "\n"
	<< "   Total Data Msgs Processed:  " << m_pcParms->GetTotalDataMsgsProcessed() << "\n"
	<< "\n"
	<< "    Uncommitted Control Msgs:  " << m_pcParms->GetUncommittedCtrlMsgs()    << "\n"
	<< "      Control Msgs Processed:  " << m_pcParms->GetCtrlMsgsProcessed()      << "\n"
	<< "Total Control Msgs Processed:  " << m_pcParms->GetTotalCtrlMsgsProcessed() << "\n"
	<< "\n"
	<< "        Reply Msgs Processed:  " << m_pcParms->GetReplyMsgsProcessed()     << "\n"
	<< "        Error Msgs Processed:  " << m_pcParms->GetErrorMsgsProcessed()     << "\n"
    << "\n"
	<< "          Synchpoints Issued:  " << m_pcParms->GetSynchpoints()           << "\n";
	//<< "\n";
	return m_cStatus.getPointer();
}







//---------------------------------------------------------------------+
//  Method:   CommitWork
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Commit Work on Queues if we have uncommitted Queue 
//            Activity
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if commit is successful, or if we don't 
//                      commit because there is no outstanding
//                      Activity.
//
//            > 0       if an error occurs.
//  
//  Dev Log:  Handle MsgProcessed counter here or in Execute?
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+ 
long CTransfer::CommitWork() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	if (m_lUncommittedMsgs > 0) {

		ll_return = m_cQueMgr.Commit();

		if(ll_return == KOCC_GOOD) {


			m_lMsgsProcessed += m_lUncommittedMsgs;
			m_lUncommittedMsgs = 0;
			m_lSynchpoints++;

			if (m_bLogSynchpoint) {

				ls_msg << "-- " << m_lMsgsProcessed 
					   << " messages committed on Queue "
					   << m_sQueueName << ".";

				m_pcLog->WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

			}

		}
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   CTransfer (Constructor)
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CTransfer::CTransfer() : CService() {

	m_lMsgsProcessed		= 0;
	m_lTotalMsgsProcessed	= 0;
	m_lUncommittedMsgs      = 0;
	m_lBatchInterval        = 0;
	m_lExpectedMsgs         = 0;
	m_lSynchpoints          = 0;
	m_lCorrelID             = 0;
	m_sBatchID              = "";
	m_pcParms				= NULL;
	m_pcLog					= NULL;
	m_bSequence				= FALSE;
	m_bLogSynchpoint		= TRUE;
	m_bEnableRestart		= TRUE;

	is_InstanceName			= "Transfer Business Logic Layer";

}







//---------------------------------------------------------------------+
//  Method:   ~CTransfer (Destructor)
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Does Nothing
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CTransfer::~CTransfer() {

//	termSelf();

}






//---------------------------------------------------------------------+
//  Method:   DoBatchInterval
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Checks if we are on a Batch Interval Boundary.
//            If so, calls CommitWork and writes 
//            Restart Info if Commit is successful.
//
//            Note that CommitWork adds m_lUncommittedMsgs msgs to
//            m_lMsgsProcessed and resets m_lUncommittedMsgs to
//            zero.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  03/17/99 DMT Changed order to Commit first and then write
//               Restart info after discussion with Dan.
//---------------------------------------------------------------------+
long CTransfer::DoBatchInterval() {

	long ll_return = KOCC_FAIL;


	if ((m_lBatchInterval > 0) &&
		(m_lUncommittedMsgs == m_lBatchInterval)) {

		ll_return = CommitWork();	

		if ((ll_return == KOCC_GOOD) && m_bEnableRestart) {

			// Set current CorrelID in Restart File
			m_cRestart.SetCorlID(m_lCorrelID);

			// Write Restart Record, indicating total number of msgs
			// processed in batch so far (including those processed in
			// a previous run).
			ll_return = m_cRestart.WriteRestartRec(m_lTotalMsgsProcessed + 
												   m_lMsgsProcessed);

			if(ll_return != KOCC_GOOD) {
				// Write Restart Failed
				m_pcLog->LogErrors(m_cRestart);
			}
		}
		// If CommitWork() Fails, Queue Manager logged error.
		
	}
	else {
		// not at interval
		ll_return = KOCC_GOOD;
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   EstablishRestart
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     If Restarting, opens Restart File and loads its values.
//
//            Otherwise, it opens and initializes the Restart File
//            if Batch Interval is > 0.
//
//            Note that the Restart File can be suppressed if 
//            descendents (ie CDeadLetter) set m_bEnableRestart to
//            FALSE.  
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
long CTransfer::EstablishRestart() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	if ((m_lBatchInterval > 0) && m_bEnableRestart) {

		ll_return = m_cRestart.initSelf((char*)m_pcParms->GetRestartFileName());

		if (m_bRestart && (ll_return == KOCC_GOOD)) {
			ll_return = m_cRestart.Open(ios::in | ios::nocreate);
		

			if (ll_return == KOCC_GOOD) {
				ll_return = m_cRestart.Load();
			}

			if (ll_return == KOCC_GOOD) {

				// Get Restart Values
				m_lTotalMsgsProcessed	= m_cRestart.GetDataMsgsRead();
				m_lExpectedMsgs			= m_cRestart.GetExpectedCount();
				m_lMsgsProcessed		= 0;
				m_lUncommittedMsgs      = 0;
				m_sBatchID				= m_cRestart.GetMsgID();
				m_lCorrelID				= m_cRestart.GetCorlID();
				m_cQue.SetAttrStr(RestartKey, m_cRestart.GetRestartKey());

				ll_return = m_cFile.Advance(m_cBuffer, m_lTotalMsgsProcessed);
			}
		}
		else if(ll_return == KOCC_GOOD) {
			ll_return = m_cRestart.Open(ios::out | ios::noreplace);

			if (ll_return == KOCC_GOOD) {

				m_cRestart.SetDataMsgsRead(m_lTotalMsgsProcessed);
				m_cRestart.SetExpectedCount(m_lExpectedMsgs);
				m_cRestart.SetMsgID(m_sBatchID.getPointer());
				m_cRestart.SetCorlID(m_lCorrelID);
				m_cRestart.SetRestartKey(m_cQue.GetAttrStr(RestartKey));
				
				ll_return = m_cRestart.WriteRestartRec();

			}
		}

		// Log Any Restart File Errors.

		m_pcLog->LogErrors(m_cRestart);

	} // end if batch interval > 0

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Generic Transfer Method.  Calls methods that may
//            be stubs at this level.
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
long CTransfer::Execute() {

	long	ll_return			= KOCC_GOOD;
	long	ll_final_return		= KOCC_GOOD;
	CString	ls_msg;
	CBuffer	cBuffer;


	BOOL	lb_input_open		= FALSE;
	BOOL	lb_output_open		= FALSE;


	//-----------------------------------------------------------------+
	// Initialize Member Classes for this Execute Pass.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = initExecute();
	}

	

	//-----------------------------------------------------------------+
	// Open Input
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = OpenInput(lb_input_open);
	}


	//-----------------------------------------------------------------+
	// Open Output
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = OpenOutput(lb_output_open);
	}



	//-----------------------------------------------------------------+
	// Handle Restart Here?
	//-----------------------------------------------------------------+
	if ((ll_return == KOCC_GOOD) && (m_lBatchInterval > 0)) {
		ll_return = EstablishRestart();
	}


	//-----------------------------------------------------------------+
	// Prime our loop with initial ReceiveData
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = ReceiveData();
	}




	//-----------------------------------------------------------------+
	// If no data available from data source, escalate warning to 
	// error, since this is the priming read.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_WARNING) {

		//
		//  check for empty source condition
		//
		if ((m_pcParms->GetDataMsgsProcessed() == 0)
		&& 	(m_pcParms->GetEmptyQueueRC() != 0)){
			ll_return = m_pcParms->GetEmptyQueueRC();
		} // end if
	
		ls_msg << "No Data available from input source.";
		m_pcLog->LogError(KORC_NO_DATA, ll_return,
						  ls_msg,
						  "CTransfer::Execute:1",
						  is_InstanceName.getPointer());
	} // end if




	//-----------------------------------------------------------------+
	// If we have data, but the Restart File doesn't have a Batch ID
	// yet set it now.
	//
	// (This really only applies to Single Queue Get (ie Get Mode 2).)
	//
	// NOTE!! Make sure m_bEnableRestart is in the "if" before the 
	//        strlen call.  If m_bEnableRestart is FALSE, the rest
	//        of the if won't be evaluated.  If we aren't doing
	//        restart (ie DeadLetter) m_cRestart.GetMsgID might
	//        return NULL and doing a strlen of a NULL pointer causes
	//        an exception.
	//-----------------------------------------------------------------+
	if (ll_return                     == KOCC_GOOD  &&
		m_bEnableRestart                            &&
		m_lBatchInterval              >  0          && 
	      (m_cRestart.GetMsgID() == NULL      ||
	      strlen(m_cRestart.GetMsgID()) == 0)      ) {

			m_cRestart.SetMsgID(m_sBatchID.getPointer());
			//m_cRestart.SetCorlID(m_lCorrelID);

			ll_return = m_cRestart.WriteRestartRec();
	}



	//-----------------------------------------------------------------+
	// Transfer Data Processing Loop
	//-----------------------------------------------------------------+
	while (ll_return == KOCC_GOOD) {

		ll_return = SendData();


		if (ll_return == KOCC_GOOD) {

			m_lUncommittedMsgs += 1;

			// Handle Batch Interval Processing if we are at a 
			// batch interval.
			ll_return = DoBatchInterval();

			if(ll_return == KOCC_GOOD) {
				ll_return = ReceiveData();
			}
		}
	}


	// -- Assuming KOCC_WARNING means at end, time to stop, we're done
	// --                       commence normal shutdown
	//
	// -- Assuming KOCC_FAIL	means we got an error and it's time to
	//                          commence error shutdown.
	if(ll_return == KOCC_WARNING)
		ll_final_return = KOCC_GOOD;

	else if (ll_return == m_pcParms->GetEmptyQueueRC())
		ll_final_return = ll_return;

	else 
		ll_final_return = KOCC_FAIL;


	//-----------------------------------------------------------------+
	// If Can Commit Work and if we have uncommitted activity,
	// do the commit.  
	//
	// Descendents (ie the "xxxBatch" Classes override CanDoFinalCommit
	// and inhibit commiting at this level.
	//
	// However, if we have an error, we want to Rollback here.
	//-----------------------------------------------------------------+
	if ((ll_final_return == KOCC_GOOD) && 
		(CanDoFinalCommit())    /*       &&
		(m_lUncommittedMsgs > 0) */    ) {

		ll_final_return = CommitWork();
	}
	else if(ll_final_return != KOCC_GOOD) {

		m_pcLog->WriteLog("*** Rolling Back Uncommitted Transactions.", 
			LOGCONSOLE + LOGFILE);

		m_cQueMgr.Rollback();
	}


	//-----------------------------------------------------------------+
	// Remove Restart File if we were successful and writing to 
	// a Restart File.
	//
	// Overridden by Batch Descendents.
	//-----------------------------------------------------------------+
	if (ll_final_return == KOCC_GOOD) {
		ll_return = ShutdownRestart();
	}
	



	//-----------------------------------------------------------------+
	// If input was opened, close it.
	//-----------------------------------------------------------------+
	if (lb_input_open) {
		ll_return = CloseInput(lb_input_open);
	}


	//-----------------------------------------------------------------+
	// If output was opened, close it.
	//-----------------------------------------------------------------+
	if (lb_output_open) {
		ll_return = CloseOutput(lb_output_open);
	}



	//-----------------------------------------------------------------+
	// Shutdown Member Classes for this Execute Pass.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = termExecute();
	}


	return ll_final_return;

}







//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Performs class initialization and calls CheckParms to
//            check required parameters at different layers.
//
//            Also allocates Message Buffer and Opens Queue Mgr.
//
//  Args:     ptr to Parms Class
//            ptr to Log Class
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/20/00 DRC Eliminated the Log ref from the CMsgFile.initSelf().
//---------------------------------------------------------------------+
long CTransfer::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {

	long	ll_return = KOCC_GOOD;
	//-----------------------------------------------------------------+
	// Initialize Parameter and Log Object Pointers
	//-----------------------------------------------------------------+
	m_pcParms	= v_pcParms;
	/*
	m_pcLog		= v_pcLog;
    */

	ll_return = CService::initSelf(v_pcLog);


	/*
	//-----------------------------------------------------------------+
	// Check Parameters - Make sure we have valid parameters for this
	//                    operation.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = CheckParms();
		m_pcLog->LogErrors(*m_pcParms);
	}
	*/


	//-----------------------------------------------------------------+
	// Transfer any Parameter errors found in initSelf to the Log
	//-----------------------------------------------------------------+
	m_pcLog->LogErrors(*m_pcParms);


	//-----------------------------------------------------------------+
	// Allocate Message Buffer for Sending/Receiving Messages
	//
	// NOTE:  If you change this so that we don't add an extra byte
	//        to the Buffer Size, be sure to check the code in
	//        CFile::Read which compensates for the extra byte.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = m_cBuffer.initSelf(m_pcParms->GetBufferSize() + 1);
	}


	//-----------------------------------------------------------------+
	// Open Queue Manger
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = m_cQueMgr.initSelf(m_pcLog,
								(char*) m_pcParms->GetQueMgrName());
	}


	return ll_return;

}







//---------------------------------------------------------------------+
//  Method:   ShutdownRestart
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Removes the Restart File.  Called if the Transfer 
//            Completes Successfully.  
//
//            Overridden by GetBatch and PutBatch so they can
//            remove restart file only after BOTH transfers
//            are successful.
//
//  Args:     None
//
//  Returns:  Always KOCC_GOOD 
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CTransfer::ShutdownRestart() {

	if (m_bEnableRestart) {
		m_cRestart.Remove();
	}

	return KOCC_GOOD;
}







//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Shuts down Queue Manager and Buffer classes.
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
long CTransfer::termSelf() {

	m_pcParms	= NULL;

	//-----------------------------------------------------------------+
	// Close Queue Manager
	//-----------------------------------------------------------------+
	m_cQueMgr.termSelf();
	

	//-----------------------------------------------------------------+
	// Free Buffer
	//-----------------------------------------------------------------+
	m_cBuffer.termSelf();


	return CService::termSelf();

}






