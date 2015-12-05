//---------------------------------------------------------------------+
//  File:     PutSingle.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CPutSingle
//            Class.
//
//            This class represents the "Put Single Queue" 
//            algorithm (Mode 1).  It is a descendent of CPut.
//
//            The model here is that the Transfer is File to Queue and
//            involves only a Data Queue.
//  
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
#include "PutSingle.h"






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
CPutSingle::CPutSingle() : CPut() {
	
	is_InstanceName = "Put Single Queue Business Logic Layer";
	
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
CPutSingle::~CPutSingle() {
	
//	CPut::termSelf();

}







//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Sets Parm Class and Log Class Pointers
//
//  Args:     ptr to CStartParamFile (Parameter Class)
//            ptr to CLog            (Log Class)
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  Not sure why I had this here, when I don't have the
//            termSelf here.  Does nothing other than calling
//            the ancestor.
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CPutSingle::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {


	long	ll_return = KOCC_GOOD;

	ll_return = CPut::initSelf(v_pcParms, v_pcLog);


	return ll_return;
}





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
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     - Calls Ancestor
//
//            - Checks Parms needed for a "Put Single".  (Which are the
//              same as those needed for any Put, so all handled at
//              the CPut level).
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
long CPutSingle::CheckParms() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;


	//-----------------------------------------------------------------+
	// Check Parameters required at ancestor levels.
	//-----------------------------------------------------------------+
	ll_return = CPut::CheckParms();

		
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
		m_bSequence				= TRUE;

		// User must specify MsgID for a PUT, We append timestamp
		// if user provided value <= 10 chars.
		m_sBatchID				= (char*) m_pcParms->GetMessageID();

		// User option of Correl ID for PUT not valid
		//m_lCorrelID				= m_pcParms->GetCorrelID();
	}

	return ll_return;

}









//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor to execute data transfer.
//            Stores Run statistics from member variables and stores
//            in Parameter Class so CCmdInterface can report them.
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
long CPutSingle::Execute() {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;



	ll_return = CPut::Execute();

	//
	// Copy RunStats to Specific Ones
	//


	m_pcParms->SetUncommittedDataMsgs(m_lUncommittedMsgs);
	m_pcParms->SetDataMsgsProcessed(m_lMsgsProcessed);
	m_pcParms->SetTotalDataMsgsProcessed(m_lTotalMsgsProcessed +
		                                 m_lMsgsProcessed);
	m_pcParms->SetSynchpoints(m_lSynchpoints);
	m_pcParms->SetMessageID(m_sBatchID.getPointer());

	return ll_return;
}

