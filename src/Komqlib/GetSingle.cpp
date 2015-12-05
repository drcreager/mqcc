//---------------------------------------------------------------------+
//  File:     GetSingle.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CGetSingle
//            Class.
//  
//            This class represents the "Get Single Queue" 
//            algorithm (Mode 2).  It is a descendent of CGet.
//
//            The model here is that the Transfer is Queue to File and
//            involves only a Data Queue.
//
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "GetSingle.h"






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
CGetSingle::CGetSingle() : CGet() {
	
	is_InstanceName = "Get Single Queue Business Logic Layer";
	
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
CGetSingle::~CGetSingle() {
	
//	CGet::termSelf();

}






//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor CheckParms
//
//            Initializes Member Variables for the Execute.
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
long CGetSingle::CheckParms() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	//-----------------------------------------------------------------+
	// Check Parameters required at ancestor levels.
	//-----------------------------------------------------------------+
	ll_return = CGet::CheckParms();

		
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
		m_lUncommittedMsgs      = 0;
		m_lExpectedMsgs			= 0;
		m_lTotalMsgsProcessed	= 0;
		m_lMsgsProcessed		= 0;
		m_bRestart				= m_pcParms->IsRestart();
		m_bSequence				= TRUE;

		if(m_pcParms->HasAdminAccess()) {
			m_sBatchID			= (char*) m_pcParms->GetMessageID();
			m_lCorrelID			= m_pcParms->GetCorrelID();

			// If CorrelID is non-zero, we Get using "CorrelID + 1"
			// if the user wants to start with Msg 3, we need to 
			// decrement this by 1, so 2 + 1 = 3 which is where
			// user wanted to start.
			if (m_lCorrelID > 0) {
				m_lCorrelID --;
			}
		}

	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor Execute Method and stores Run Statistics
//            from Member Variables in the Parameter Class so 
//            CCmdInterface can report them.
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
long CGetSingle::Execute() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;



	ll_return = CGet::Execute();

	//
	// Copy RunStats to Specific Ones
	//
	m_pcParms->SetUncommittedDataMsgs(m_lUncommittedMsgs);
	m_pcParms->SetDataMsgsProcessed(m_lMsgsProcessed);
	m_pcParms->SetTotalDataMsgsProcessed(m_lTotalMsgsProcessed +
		                                 m_lMsgsProcessed);
	m_pcParms->SetMessageID(m_sBatchID.getPointer());
	m_pcParms->SetExpectedMsgs(m_lExpectedMsgs);
	m_pcParms->SetSynchpoints(m_lSynchpoints);

	return ll_return;
}

