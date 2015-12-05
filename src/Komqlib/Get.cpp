//---------------------------------------------------------------------+
//  File:     Get.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CGet
//            Class.
//  
//            This class represents the generic "Get" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CGetBatch (Mode 1) and CGetSingle (Mode 2)
//            algorithms.
//
//            The model here is that the Transfer is Queue to File.
//
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//  03/02/00 DRC Added extension of DoBatchInterval
//---------------------------------------------------------------------+
#include "Get.h"





//---------------------------------------------------------------------+
//  Method:   CGet (Constructor)
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
CGet::CGet() : CTransfer() {

	is_InstanceName = "Get Business Logic Layer";
	
}







//---------------------------------------------------------------------+
//  Method:   ~CGet (Destructor)
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
CGet::~CGet() {
//	termSelf();
}








//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor and if Ancestor is okay, asks the 
//            parameter class to validate parameters required
//            for any GET operation.
//
//            - If Batch Interval is > 0, must have a valid Restart File
//                 Name.
//
//            - Must have valid Data File.
//
//            - Must have valid Batch Interval.
//
//            m_pcParms Errors are logged after CheckParms call in
//            CTransfer::initSelf(), so no need to report here.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::CheckParms() {

	long	ll_return = KOCC_GOOD;

	// Call Ancestor to validate parms needed at "Transfer" level.
	ll_return = CTransfer::CheckParms();

	if (ll_return == KOCC_GOOD) {

		// Validate Parms Needed for any GET.

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

	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   CloseInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     For Get Processing, closes the input Queue and sets
//            input open flag to FALSE.
//
//  Args:     ref to BOOL, Input is Open Flag
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  THIS IS BACKWARDS, method currently closing 
//            Output File.  TODO
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::CloseInput(BOOL& rb_input_open) {

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
//  Desc:     For Get Processing, closes Output File and sets Output
//            Open flag to FALSE.
//
//  Args:     ref to BOOL, Output is Open Flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  THIS IS BACKWARDS!!, Currently this is closing the 
//            input Queue. TODO
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::CloseOutput(BOOL& rb_output_open) {

	long ll_return = KOCC_GOOD;

	if (rb_output_open) {
		ll_return = m_cQue.Close();

		rb_output_open = FALSE;
	}

	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   DoBatchInterval
//  Author:   Daniel R Creger
//  Written:  March 2000
//
//  Desc:     Extend CTransfer::DoBatchInterval to provide GET specific
//            processing for this method.  
//
//            For GET processing: we need to obtain the RESTART_KEY 
//            value from the API and include it in the restart record. 
//
//  Args:     None
//
//  Returns:  KOCC_GOOD or KOCC_FAIL as set by the ancestor method.
//  
//  Dev Log:  
//  History: 
//  03/02/00 DRC Created original version.
//---------------------------------------------------------------------+
long CGet::DoBatchInterval() {
//
// Transfer the RestartKey Attribute from the API into the Restart File
//
m_cRestart.SetRestartKey(m_cQue.GetAttrStr(RestartKey));

//
//  Invoke the ancestor's method
//
return CTransfer::DoBatchInterval();
}  /* End Method*/







//---------------------------------------------------------------------+
//  Method:   initExecute
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Do Pre-Execute initialization for Queue and File 
//            objects.
//
//  Args:     TBD
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::initExecute() {

	long ll_return = KOCC_GOOD;


	ll_return = m_cQue.initSelf(m_pcLog, &m_cQueMgr, 
		                        m_sQueueName.getPointer(),
								m_pcParms->GetFailOnQuiescing(),
								m_pcParms->GetTranslate());

	if (ll_return == KOCC_GOOD) {
		ll_return = m_cFile.initSelf(m_pcLog, m_sFileName.getPointer());
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Get Initialization.  Does nothing other than calling
//            ancestor on CTransfer.
//
//  Args:     pointer to CStartParamFile
//            pointer to CLog
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog) {

	long ll_return = KOCC_GOOD;

	ll_return = CTransfer::initSelf(v_pcParms, v_pcLog);

	return ll_return;

}







//---------------------------------------------------------------------+
//  Method:   OpenInput
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens a Queue for Exclusive Input
//
//  Args:     ref to BOOL - Input Open Indicator
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::OpenInput(BOOL& rb_input_open) {

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
//  Desc:     Opens a File for Input and Output if in Restart Mode,
//
//            Opens a File for Output if not in Restart Mode and file,
//            must not already exist.
//
//  Args:     ref to BOOL, Output Open flag.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::OpenOutput(BOOL& rb_output_open) {

	long ll_return = KOCC_FAIL;

	if (m_bRestart) {
		ll_return = m_cFile.Open((ios::open_mode) ios::in | ios::out);
	}

	else {
		ll_return = m_cFile.Open((ios::open_mode) ios::out | ios::noreplace);
	}
	
	rb_output_open = (ll_return == KOCC_GOOD);

	return ll_return;
}











//---------------------------------------------------------------------+
//  Method:   ReceiveData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     For Get Processing, Gets data from a Queue.
//
//            If m_bSequence is True, the value of m_lCorrelID is 
//            incremented before calling GET.
//
//            If m_bSequence is False, m_lCorrelID is not incremented
//            and its current value (if not zero) is used to match
//            the next message retrieved.
//
//            If m_lMsgID is non-NULL, it's value is used to match
//            the GET.
//
//            Note that m_lUncommittedMsgs is incremented in 
//            CTransfer::Execute after a good SendData() call and
//            m_lMsgsProcessed is updated in 
//            CTransfer::CommitWork().
//
//  Args:     None, but member variable m_bSequence controls Get 
//                  sequencing.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::ReceiveData() {

	long ll_return		= KOCC_GOOD;


	if (m_bSequence && (m_lCorrelID > 0)) {
		m_lCorrelID++;
	}
	
	ll_return = m_cQue.GetNext(m_cBuffer, 
		                       m_sBatchID.getPointer(), 
		                       m_lCorrelID);

	if (ll_return == KOCC_GOOD) {

		if (m_sBatchID.vlen() == 0) {

			// if msg ID was null before, set it now.
			m_sBatchID = (char*) m_cQue.GetMsgID();
		}

		if (m_lCorrelID == 0) {
			m_lCorrelID = m_cQue.GetCorlIDBin();
		}

	}
	else {
		if (m_bSequence && (m_lCorrelID > 0)) {
			// decrement the Correlation ID if we are sequencing
			// and didn't get the last message
			m_lCorrelID--;
		}
	}

	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   SendData
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     For Get Processing, writes message data to a File.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::SendData() {

	long	ll_return = KOCC_FAIL;

	ll_return = m_cFile.Write(m_cBuffer);

	return ll_return;

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
long CGet::termExecute() {

	long	ll_return = KOCC_GOOD;

	ll_return = m_cFile.termSelf();

	if (ll_return == KOCC_GOOD) {
		m_cQue.termSelf();
	}

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor.  Nothing special at this level.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL in an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CGet::termSelf() {

	return CTransfer::termSelf();

}