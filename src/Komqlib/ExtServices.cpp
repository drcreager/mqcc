//---------------------------------------------------------------------+
//  File:     ExtServices.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the CExtServices
//            Class.
//  
//
//  Dev Log: 
//  01)  With a BufferSize of 2048 Block/Deblock is dropping the final
//       record. (Note it is correct with 256)
//
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
#include "ExtServices.h"

//---------------------------------------------------------------------+
//  Method:   CExtServices (Default Constructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     tbd.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
CExtServices::CExtServices(){
is_InstanceName = "Extended Services Business Logic Layer";
}  /* End CExtServices */





//---------------------------------------------------------------------+
//  Method:   ~CExtServices (Destructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
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
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
CExtServices::~CExtServices(){

}  /* End ~CExtServices */





//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Checks for Valid Parameters needed for ExtServices.
//
//            Asks the Parameter Class to check for the following:
//            - Valid Operation
//            - Valid LogFileName
//            - Valid BufferSize
//
//            UpdateFiles
//            - Valid Blocking OR Deblocking
//            - Valid DataFileName
//
//            m_pcParms errors are reported after the CheckParms call
//            in CService::initSelf()
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtServices::CheckParms() {

long ll_return = KOCC_GOOD;
CString	ls_ErrText;

//
//  Validate the Operation Specification
//
(m_pcParms->GetOperation() != NoOpr)? 0 : ll_return = KOCC_FAIL;
if (ll_return == KOCC_FAIL){
	ls_ErrText = "Missing Operation= Parameter!";
	m_pcLog->LogError(KORC_INVALID_OPERATION, KOCC_FAIL, ls_ErrText,
			"CExtServices::CheckParms():1",
			is_InstanceName.getPointer());
} /* End If */

//
//  Validate the universally mandatory parameters
//
(m_pcParms->IsValidLogFileName())   ? 0 : ll_return = KOCC_FAIL;
		
if (!m_pcParms->IsValidBufferSize()) {		
	ll_return = KOCC_FAIL;
	goto exitPt;
} /* End If */
		

//
//  Check for invalid combinations of type
//
//  Encode  AND Decode  should never happen in the same process
//  Encrypt AND Decrypt should never happen in the same process
//  Block   AND Deblock should never happen in the same process
//  Encode  AND Decrypt should never happen in the same process
//  Decode  AND Encrypt should never happen in the same process
//
if ((m_pcParms->IsEncodingSpecified()
&&  m_pcParms->IsDecodingSpecified())
||  (m_pcParms->IsEncryptionSpecified()
&&  m_pcParms->IsDecryptionSpecified())
||  (m_pcParms->IsBlockingSpecified()
&&  m_pcParms->IsDeblockingSpecified())
||  (m_pcParms->IsEncodingSpecified()
&&  m_pcParms->IsDecryptionSpecified())
||  (m_pcParms->IsDecodingSpecified()
&&  m_pcParms->IsEncryptionSpecified())){
	ls_ErrText = "Mutually exclusive types of Operation specified!";
	m_pcLog->LogError(KORC_INVALID_OPERATION, KOCC_FAIL, ls_ErrText,
			"CExtServices::CheckParms():2",
			is_InstanceName.getPointer());
	ll_return = KOCC_FAIL;
} /* End If */

//
//  Check for Cipher Key when required
//
//
if ((m_pcParms->IsEncryptionSpecified()
	||  m_pcParms->IsDecryptionSpecified())
	&&  !m_pcParms->IsValidCipherKey()){
	ls_ErrText = "Encryption / Decryption Key is invalid or missing!";
	m_pcLog->LogError(KORC_INVALID_PARM_COMBINATION, KOCC_FAIL, ls_ErrText,
			"CExtServices::CheckParms():3",
			is_InstanceName.getPointer());
	ll_return = KOCC_FAIL;
} /* End If */

//
//  Validate the mandatory parameters for UpdateFile
//
if (m_pcParms->GetOperation() == UpdateFile){
	//
	//  A TYPE of Operation MUST be specified
	//
	if (m_pcParms->IsEncodingSpecified()
	||  m_pcParms->IsDecodingSpecified()
	||  m_pcParms->IsEncryptionSpecified()
	||  m_pcParms->IsDecryptionSpecified()
	||  m_pcParms->IsBlockingSpecified()
	||  m_pcParms->IsDeblockingSpecified());
	else {
		ls_ErrText = "The type of Operation is not specified!";
		m_pcLog->LogError(KORC_INVALID_OPERATION, KOCC_FAIL, ls_ErrText,
				"CExtServices::CheckParms():4",
				is_InstanceName.getPointer());
		ll_return = KOCC_FAIL;
	} /* End If */

} /* End If */

exitPt:
	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     tbd.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtServices::Execute(){
	long    l_lResult = KOCC_GOOD;
	CString	ls_ErrText;
	char*   l_szTempFile = NULL;
	long    l_lRcrdLen   = 0;

	//
	//  Create a temporary file name
	//
   if((l_szTempFile = tmpnam(NULL)) == NULL) 
	  strcpy(l_szTempFile, "~ESUCpy1.tmp");

	//
	//  Invoke the Operation specified in the Parameter settings
	//
	switch (m_pcParms->GetOperation()){
	case PutFiles:
		//
		// Instantiate and Initialize the Service
		//
		m_pcService = (CPutFilesSrvc*) new CPutFilesSrvc(m_pcParms, m_pcLog);

		//-----------------------------------------------------------------+
		// Validate the Input Parameters and 
		// Transfer any Parameter errors found to the Log
		//-----------------------------------------------------------------+
		l_lResult = m_pcService->initSelf(m_pcLog);
		m_pcLog->LogErrors(*m_pcParms);

		//
		// Execute the Service
		//
        if (l_lResult == KOCC_GOOD){
			l_lResult  = ((CPutFilesSrvc*) m_pcService)->Execute();
			switch (l_lResult){
			case KORC_NO_DATA:
				ls_ErrText = "Put Files request located NO source files!";
				m_pcLog->LogError(KORC_NO_DATA, KOCC_WARNING, ls_ErrText,
								  "CExtServices::Execute():1",
								  is_InstanceName.getPointer());
				l_lResult = KOCC_WARNING;
				break;

			case KORC_PROCESSOR_ERROR:
				ls_ErrText = "Abnormal termination in the Put Files Processor!";
				m_pcLog->LogError(KORC_PROCESSOR_ERROR, KOCC_WARNING, 
					              ls_ErrText, "CExtServices::Execute():2",
								  is_InstanceName.getPointer());
				l_lResult = KOCC_WARNING;
				break;

			default: 
				m_pcLog->LogErrors(*m_pcService);

			} /* End Switch*/
		} /* End If */
		//
		// Release the Service
		//
		delete[] m_pcService;
		break;

	case GetFiles:
		//
		// Instantiate and Initialize the Service
		//
		m_pcService = (CGetFilesSrvc*) new CGetFilesSrvc(m_pcParms, m_pcLog);

		//-----------------------------------------------------------------+
		// Validate the Input Parameters and 
		// Transfer any Parameter errors found to the Log
		//-----------------------------------------------------------------+
		l_lResult = m_pcService->initSelf(m_pcLog);
		m_pcLog->LogErrors(*m_pcParms);

		//
		// Execute the Service
		//
        if (l_lResult == KOCC_GOOD){
			l_lResult  = ((CGetFilesSrvc*) m_pcService)->Execute();
			switch (l_lResult){
			case KORC_EMPTY_QUEUE:
				ls_ErrText = "Get Files: Queue is Empty!";
				m_pcLog->LogError(KORC_EMPTY_QUEUE, KOCC_WARNING, ls_ErrText,
								  "CExtServices::Execute():3",
								  is_InstanceName.getPointer());
				l_lResult = KOCC_WARNING;
				break;
			
			case KORC_NO_DATA:
				ls_ErrText = "Get Files request located NO source files!";
				m_pcLog->LogError(KORC_NO_DATA, KOCC_WARNING, ls_ErrText,
								  "CExtServices::Execute():4",
								  is_InstanceName.getPointer());
				l_lResult = KOCC_WARNING;
				break;

			case KORC_PROCESSOR_ERROR:
				ls_ErrText = "Abnormal termination in the Get Files Processor!";
				m_pcLog->LogError(KORC_PROCESSOR_ERROR, KOCC_WARNING, 
					              ls_ErrText, "CExtServices::Execute():5",
								  is_InstanceName.getPointer());
				l_lResult = KOCC_FAIL;
				break;

			default: 
				m_pcLog->LogErrors(*m_pcService);
				
			} /* End Switch*/
		} /* End If */

		//
		//  Warn user when the file set in in-complete.
		//
		if (!((CGetFilesSrvc*) m_pcService)->isFileSetComplete()){
			CopyErrors((CAny2*) m_pcService);
			m_pcLog->LogErrors(*this);
			l_lResult = KOCC_FAIL;
		} /* End If */

		//
		// Release the Service
		//
		delete m_pcService;
		break;

	case UpdateFile:
		if (m_pcParms->IsBlockingSpecified()) {
			m_pcFileMod = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);

			//-----------------------------------------------------------------+
			// Initialize Service and Transfer any errors to the Log
			//-----------------------------------------------------------------+
		    ((CService*) m_pcFileMod)->initSelf(m_pcLog);
			m_pcLog->LogErrors(*m_pcParms);

            l_lResult  = ((CBlockingSrvc*) m_pcFileMod)->Block(
				                           (char*) m_pcParms->GetDataFileName(), 
						                   m_pcParms->GetBufferSize(),
										   m_pcParms->GetRecordLength(), true);
			m_pcLog->LogErrors(*m_pcParms);

			if (l_lResult == KOCC_GOOD){
				m_pcParms->SetDataFilesProcessed(m_pcParms->GetDataFilesProcessed() + 1);
			}
			else {
				ls_ErrText = "Block Files request failed!";
				m_pcLog->LogError(KORC_FILE_OPEN_ERROR, KOCC_FAIL, ls_ErrText,
				                  "CExtServices::Execute():6",
				                  is_InstanceName.getPointer());
				l_lResult = KOCC_FAIL;
			} /* End If */
			delete[] m_pcFileMod;
		} /* End If */

		if (m_pcParms->IsDeblockingSpecified()) {
			m_pcFileMod = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);
			
			//-----------------------------------------------------------------+
			// Initialize Service and Transfer any errors to the Log
			//-----------------------------------------------------------------+
		    ((CService*) m_pcFileMod)->initSelf(m_pcLog);
			m_pcLog->LogErrors(*m_pcParms);

            l_lResult  = ((CBlockingSrvc*) m_pcFileMod)->Deblock(
				                           (char*) m_pcParms->GetDataFileName(), 
						                   m_pcParms->GetBufferSize(), 
										   m_pcParms->GetRecordLength(), 
										   true, true);
			m_pcLog->LogErrors(*m_pcParms);

			if (l_lResult == KOCC_GOOD){
				m_pcParms->SetDataFilesProcessed(m_pcParms->GetDataFilesProcessed() + 1);
			}
			else {
				ls_ErrText = "Deblock Files request failed!";
				m_pcLog->LogError(KORC_FILE_OPEN_ERROR, KOCC_FAIL, ls_ErrText,
				                  "CExtServices::Execute():7",
				                  is_InstanceName.getPointer());
			} /* End If */
			delete[] m_pcFileMod;
		} /* End If */
		break;

	case NoOpr:
		l_lResult = KORC_INVALID_OPERATION;
		break;

	default:
		l_lResult = KORC_INVALID_OPERATION;

	} /* End Switch */

	return l_lResult;
}  /* End Execute */





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
char* CExtServices::GetStatus() {
	int l_iTypeCnt = 0;

	m_cStatus = "";
	m_cStatus << "\n" 
	<< "Run Statistics for Extended Services:\n\n"
	<< "                   Requested Service:  ";

	switch (m_pcParms->GetOperation()){
	case PutFiles:
		m_cStatus << "Put";
		l_iTypeCnt++;
		GetTypeStatus(&l_iTypeCnt, ": ");
		if (l_iTypeCnt) m_cStatus << " File(s).\n";
		break;

	case GetFiles:
		m_cStatus << "Get File(s).\n";
		break;

	case UpdateFile:
		GetTypeStatus(&l_iTypeCnt, ", ");
		if (l_iTypeCnt) m_cStatus << " File(s).\n";
		else            m_cStatus << "Unspecified Update Operation\n";
		break;
	
	case NoOpr:
		m_cStatus << "No Operation"  << "\n";
		break;
	} /* End Switch */

	m_cStatus 
	<< "                     Files   Located:  " << m_pcParms->GetDataFilesProcessed()  << "\n"
	<< "                     Files Processed:  " << m_pcParms->GetCtrlFilesProcessed()  << "\n";
    //<< "\n";
	return m_cStatus.getPointer();
}  /* End GetStatus */





//---------------------------------------------------------------------+
//  Method:   GetTypeStatus
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
void CExtServices::GetTypeStatus(int* a_iTypeCnt, char* a_szDlmtr) {

	if (m_pcParms->IsEncryptionSpecified()) {
		if (*a_iTypeCnt) m_cStatus << a_szDlmtr;
		m_cStatus << "Encrypt, Encode";
		(*a_iTypeCnt)++;
	}
	else if (m_pcParms->IsEncodingSpecified()) {
		if (*a_iTypeCnt) m_cStatus << a_szDlmtr;
		m_cStatus   << "Encode";
		(*a_iTypeCnt)++;
	}

	if (m_pcParms->IsDecryptionSpecified()) {
		if (*a_iTypeCnt) m_cStatus << a_szDlmtr;
		m_cStatus << "Decode, Decrypt";
		(*a_iTypeCnt)++;
	}
	else if (m_pcParms->IsDecodingSpecified()) {
		if (*a_iTypeCnt) m_cStatus << a_szDlmtr;
		m_cStatus   << "Decode";
		(*a_iTypeCnt)++;
	}

	if (m_pcParms->IsBlockingSpecified()) {
		if (*a_iTypeCnt) m_cStatus << " and ";
		m_cStatus   << "Block";
		(*a_iTypeCnt)++;
	}
	if (m_pcParms->IsDeblockingSpecified()) {
		if (*a_iTypeCnt) m_cStatus << " and ";
		m_cStatus << "Deblock"; 
		(*a_iTypeCnt)++;
	}
}





//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   Daniel R Creager
//  Written:  November 2000
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
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtServices::initSelf(CExtSrvcsParamFile* v_pcParms, CLog* v_pcLog) {

	long	ll_return = KOCC_GOOD;

	//-----------------------------------------------------------------+
	// Initialize Parameter and Log Object Pointers
	//-----------------------------------------------------------------+
	m_pcParms	= v_pcParms;
	ll_return = CService::initSelf(v_pcLog);


	//-----------------------------------------------------------------+
	// Transfer any Parameter errors found in initSelf to the Log
	//-----------------------------------------------------------------+
	m_pcLog->LogErrors(*m_pcParms);

	return ll_return;

}  /* End initSelf */





//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   Daniel R Creager
//  Written:  November 2000
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
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtServices::termSelf() {

	m_pcParms	= NULL;

	return CService::termSelf();

}  /* End termSelf */














