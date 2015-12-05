//---------------------------------------------------------------------+
//  File:     CPutFilesSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the CPutFilesSrvc
//            Class.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
#include "PutFilesSrvc.h"
CPutFilesSrvc::CPutFilesSrvc(CExtSrvcsParamFile* a_pcParms, 
							 CLog*               a_pcLog){
is_InstanceName = "Put Files Service Business Logic Layer";
if (isValidPointer(a_pcParms)) m_pcParms = a_pcParms;
if (isValidPointer(a_pcLog))   m_pcLog = a_pcLog;
} /* End CPutFilesSrvc */






CPutFilesSrvc::~CPutFilesSrvc(){
if (isValidPointer(m_pcParms)) m_pcParms = NULL;
if (isValidPointer(m_pcLog))   m_pcLog = NULL;
}

//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Validate Parameters needed for the PutFiles Service.
//
//            PutFiles
//            - Valid ParmFile (Refers to the Parameters Template)
//            - Valid SourceFiles
//
//            On FileDisp=Archive
//            - Valid FileArchive
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
long  CPutFilesSrvc::CheckParms(){
long ll_return = KOCC_GOOD;
CString	ls_ErrText;


//
// Call Ancestor to validate parms needed at "MonitoringSrvc" level.
//
ll_return = CMonitoringSrvc::CheckParms();

if (ll_return == KOCC_GOOD) {
	//
	//  Validate the mandatory parameters for PutFiles
	//
	(m_pcParms->IsValidSrcFileMask())   ? 0 : ll_return = KOCC_FAIL;
	if (ll_return == KOCC_FAIL){
		ls_ErrText = "Missing Mandatory Parameter(s).";
		m_pcLog->LogError(KORC_INVALID_PARM_COMBINATION, KOCC_FAIL, ls_ErrText,
				"CPutFilesSrvc::CheckParms():1",
				is_InstanceName.getPointer());
	} /* End If */

	if (!m_pcParms->IsValidParmFile()) {		
		ll_return = KOCC_FAIL;
	} /* End If */

	if (!m_pcParms->IsValidDisp()) {		
		ls_ErrText = "Missing or Invalid 'FileDisp=' OR 'FileDisp=' and 'FileArchive=' are inconsistent!";
		m_pcLog->LogError(KORC_INVALID_PARM_COMBINATION, KOCC_FAIL, ls_ErrText,
				"CPutFilesSrvc::CheckParms():2",
				is_InstanceName.getPointer());
		ll_return = KOCC_FAIL;
	} /* End If */

} /* End If */

return ll_return;
} /* End CheckParms */




//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Execute the PutFiles Service.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  History: 
//  11/10/00 DRC Created original version.
//  01/10/01 DRC Do not replace original files with blocked versions.
//  03/27/02 DRC Modified setting the DATA file name to support Blocking=Off
//---------------------------------------------------------------------+
long CPutFilesSrvc::Execute(){

long     l_lResult     = KOCC_GOOD;
CString  l_szFileName;
CString  l_szTempFileName;
CString  ls_ErrText;
long     l_lRcrdCnt    = 0;

//
//  Construct a file list per caller specifications
//
m_pcFileList = new CFileList((char*) m_pcParms->GetSrcFileMask(), 
							 m_pcParms->IsSubDirSrchSpecified());
if (m_pcFileList->count() > 0 ) 
	m_pcParms->SetFilesLocated(
	           m_pcParms->GetFilesLocated() 
			   + m_pcFileList->count());
else l_lResult = KORC_NO_DATA;

//
//  Instantiate a BlockingSrvc object
//
m_pcFMSrvc = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);

//
//  Process the files in the list using the File Modification 
//  services specified in the Parameter settings.
//
m_pcFile = (CFile*) m_pcFileList->top();
while ((!m_pcFileList->endOfList()) &&  (l_lResult == KOCC_GOOD)){
	m_pcFileList->pushCursor();
    l_lRcrdCnt = 0;

	//
	//  Block a file
	//
	if (m_pcParms->IsBlockingSpecified()) {
        l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Block(
			         (char*) m_pcFile->GetName(), m_pcFileList, false);
		
		if (l_lResult != KOCC_GOOD) CopyErrors((CBlockingSrvc*) m_pcFMSrvc);
		
		l_szTempFileName = ((CBlockingSrvc*) m_pcFMSrvc)->getTempFileName();
		l_lRcrdCnt = ((CBlockingSrvc*) m_pcFMSrvc)->getRcrdCount();
	} // End If
	
	//
	//  Generate a Control file
	//
	if (l_lRcrdCnt == 0){
		l_lRcrdCnt = m_pcFile->setRcrdCount();
	} /* End If */
	l_szFileName = (char*) m_pcFile->GetName();
    l_szFileName = ChangeFileExt(&l_szFileName, "ctl");
	m_pcParms->SetCtlFileName(l_szFileName.getPointer());

	m_pcCtlFile->initSelf(m_pcLog, l_szFileName.getPointer());
	m_pcCtlFile->Export(l_lRcrdCnt);
	m_pcCtlFile->termSelf();

	//
	//  Generate a Parameter file
	//
	//  Point to the temp file for placement into MQ
	//
	if (m_pcParms->IsBlockingSpecified()) {
		m_pcParms->SetDataFileName(l_szTempFileName.getPointer());
	}
	else {
		l_szFileName = (char*) m_pcFile->GetName();
		m_pcParms->SetDataFileName(l_szFileName.getPointer());
	} // End If

	l_szFileName = ChangeFileExt(&l_szFileName, "log");
	m_pcParms->SetLogFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "rst");
	m_pcParms->SetRestartFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "prm");
	m_pcParms->WriteParmFile(m_pcParms->GetParmFileName(),
				         	 l_szFileName.getPointer());

	//
	//  Invoke the KOMQPUT utility
	//
	if (m_pcParms->GetPutProcessor() != NULL){ 
		l_lResult  =  m_pcSysCmd->SpawnWait(
			                  m_pcParms->GetPutProcessor(), 
							  l_szFileName.getPointer());


		if (l_lResult == KOCC_GOOD) m_pcParms->SetFilesProcessed(
									m_pcParms->GetFilesProcessed() + 1);
		else {
			ls_ErrText = "Processor Error: Put RC(";
			ls_ErrText += l_lResult;
			ls_ErrText += ")";
			m_pcLog->LogError(l_lResult, KOCC_FAIL, 
			ls_ErrText, "CPutFilesSrvc::Execute:1",
			is_InstanceName.getPointer());
			l_lResult = KORC_PROCESSOR_ERROR;
		} // end if	
	} /* End If */

	
	//
	//  Dispose of temp file or allocate a new temp file name
	//
	if (l_lResult == KOCC_GOOD) { 
		m_pcFMSrvc->Delete(l_szTempFileName.getPointer());
	}
	else {
		//
		//  Allocate a new temp file name
		//
		((CBlockingSrvc*) m_pcFMSrvc)->setTempFileName(
			m_pcFNameSrvc->getFilePath((char*) m_pcFile->GetName()));
	}  /* End If */

	//
	//  Dispose of files as indicated by user
	//
	if ((l_lResult == KOCC_GOOD) 
	&&   m_pcParms->IsFileDispArchive()){
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcFile->GetName());
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcParms->GetCtlFileName());
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcParms->GetLogFileName());
		l_lResult  = m_pcFMSrvc->Archive(l_szFileName.getPointer());
		if (l_lResult == -1){
			ls_ErrText = "Resource Error: Duplicate Name Detected in Archive!";
			m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
				ls_ErrText, "CPutFilesSrvc::Execute:2",
				is_InstanceName.getPointer());
		}
		else if (l_lResult != 0){
			ls_ErrText = "Resource Error: Unable to Archive file!";
			m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
				ls_ErrText, "CPutFilesSrvc::Execute:3",
				is_InstanceName.getPointer());
		} /* End If */
	} /* End If */

	if ((l_lResult == KOCC_GOOD) 
	&&   m_pcParms->IsFileDispDelete()){
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcFile->GetName());
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetCtlFileName());
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetLogFileName());
		l_lResult  = m_pcFMSrvc->Delete(l_szFileName.getPointer());
		if (l_lResult != 0){
			ls_ErrText = "File Error: File Delete failed!";
			m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
				ls_ErrText, "CPutFilesSrvc::Execute:4",
				is_InstanceName.getPointer());
		} /* End If */
	} /* End If */
	//delete m_pcFMSrvc;

	m_pcFileList->popCursor();
	m_pcFile = (CFile*) m_pcFileList->next();
} // End while 

//
//  Release the BlockingSrvc object
//
delete m_pcFMSrvc;

return l_lResult;
} /* End Execute */