//---------------------------------------------------------------------+
//  File:     DeadSingle.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CGetSingle
//            Class.
//
//            This class represents the Dead Letter Queue Transfer
//            (Mode 1) algorithm and is derived from CDeadLetter.
//  
//            The model here is that the Transfer is Queue to Queue, 
//            from the Dead Letter Queue to an appropriate 
//            Destination Queue, depending on the source messages type.
//
//  Dev Log: 
//  History: 
//  03/24/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "DeadSingle.h"






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
CDeadSingle::CDeadSingle() : CDeadLetter() {
	
	is_InstanceName = "Single Dead Letter Queue Business Logic Layer";
	
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
CDeadSingle::~CDeadSingle() {
	
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
//            Note that Dead Letter Queue Processing Required Batch
//            Interval to be set to 1.  If it's not 1, whatever the
//            user provided is overridden and the user is notified
//            of the override.
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
long CDeadSingle::CheckParms() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;
	CString ls_wrapped;


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

		if(m_pcParms->HasAdminAccess()) {
			//m_sBatchID			= (char*) m_pcParms->GetMessageID();
			//m_lCorrelID			= m_pcParms->GetCorrelID();

			ls_msg << "-- Dead Letter Processing does not support"
				   << " Administrator access." << "\n"
				   << "-- Message ID and Correlation ID are ignored"
				   << " if provided.\n";

			m_pcLog->WriteLog(ls_msg);
		}


		if (m_lBatchInterval != 1) {

			m_lBatchInterval = 1;

			ls_msg     = "";
			ls_wrapped = "";

			ls_msg << "Overriding Batch Interval Parameter to 1."
			       << "  Interim Commits are required after tranferring"
				   << " each message when processing "
				   << "the Dead Letter Queue.";

			m_pcLog->WrapLine(ls_wrapped, ls_msg.getPointer(), 
										72, "-- ");

			m_pcLog->WriteLog(ls_wrapped);
		}

	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Dead Letter Queue Transfer Logic. - calls ancestor 
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
long CDeadSingle::Execute() {

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

