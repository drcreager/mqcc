//---------------------------------------------------------------------+
//  File:     CMonitoringSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the 
//            CMonitoringSrvc Class.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "MonitoringSrvc.h"

//---------------------------------------------------------------------+
//  Method:   ChangeFileExt
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Changes the extension of a specified file name.
//
//  Args:     CString*  FileName to be altered
//            char*     New file extension to be appended 
//
//  Returns:  char*     Altered file name
//  
//  History: 
//  11/10/00 DRC Created original version.
//  03/02/01 DRC Adjusted to handle filenames with no extension.
//---------------------------------------------------------------------+
char* CMonitoringSrvc::ChangeFileExt(char* a_szFileName,
									 char* a_szExtension){
long l_Pos  = 1;
long l_Prev = 0;
//
//  Locate the last dot "." in the file name.
//
*m_szWork = a_szFileName;
while (l_Pos > 0){
	l_Prev = l_Pos;
	l_Pos = m_szWork->pos(++l_Pos,".");
}
//
//  On File Extension Found, truncate the existing file extension 
//  Append a new extension on the result 
//
if (l_Prev > 1) *m_szWork =  m_szWork->mid(1, l_Prev - 1);
*m_szWork << "." << a_szExtension;

return m_szWork->getPointer();
}  /* end ChangeFileExt */





char* CMonitoringSrvc::ChangeFileExt(CString* a_szFileName,
								  	 char*    a_szExtension){
return ChangeFileExt(a_szFileName->getPointer(), a_szExtension);
}  /* end ChangeFileExt */





//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Checks for Valid Parameters needed for Monitoring.
//
//            Asks the Parameter Class to check for the following:
//            - Valid Operation
//            - Valid LogFileName
//            - Valid BufferSize
//     =>>    - Valid Mode
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
long  CMonitoringSrvc::CheckParms(){

	long ll_return = KOCC_GOOD;
	//
	// Call Ancestor to validate parms needed at "Service" level.
	//
	ll_return = CService::CheckParms();


	if (!m_pcParms->IsValidLogFileName()) {		
		ll_return = KOCC_FAIL;
		m_pcLog->LogError(KORC_INVALID_FILE_NAME, KOCC_FAIL,
			"Missing or invalid 'LogFileName=' specification.",
			"CMonitoringSrvc::CheckParms:3",
			is_InstanceName.getPointer());
	} /* End If */


	if (!m_pcParms->IsValidBufferSize()) {		
		ll_return = KOCC_FAIL;
		m_pcLog->LogError(KORC_INVALID_BUFFER_SIZE, KOCC_FAIL,
			"Missing or invalid 'BufferSize='.  Value Range is 1 to 2093152",
			"CMonitoringSrvc::CheckParms:4",
			is_InstanceName.getPointer());
	} /* End If */

	return ll_return;
} /* End CheckParms */




CMonitoringSrvc::CMonitoringSrvc(){
is_InstanceName = "Monitoring Services Business Logic Layer";
m_pcCtlFile     = new CCtlFile();
m_pcSysCmd      = new CSysCmd();
m_szWork        = new CString();
m_pcFNameSrvc   = new CFileNameSrvc(m_pcParms);
} /* End CMonitoringSrvc */



CMonitoringSrvc::CMonitoringSrvc(CExtSrvcsParamFile* a_pcParms, 
					             CLog* a_pcLog){
if (isValidPointer(a_pcParms)) m_pcParms = a_pcParms;
if (isValidPointer(a_pcLog))   m_pcLog = a_pcLog;
CMonitoringSrvc();
} /* End CMonitoringSrvc */



CMonitoringSrvc::~CMonitoringSrvc(){
if (isValidPointer(m_pcFNameSrvc)) delete m_pcFNameSrvc;
if (isValidPointer(m_szWork))      delete m_szWork;
if (isValidPointer(m_pcCtlFile))   delete m_pcCtlFile;
if (isValidPointer(m_pcSysCmd))    delete m_pcSysCmd;
} /* End ~CMonitoringSrvc */



long CMonitoringSrvc::count(){
	return m_pcFileList->count();
} /* End count */






long CMonitoringSrvc::Execute(){

long     l_lResult     = KOCC_GOOD;
CString  l_szFileName;
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
		m_pcFMSrvc = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);
        l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Block(
			         (char*) m_pcFile->GetName(), m_pcFileList, true);
		l_lRcrdCnt = ((CBlockingSrvc*) m_pcFMSrvc)->getRcrdCount();
		delete m_pcFMSrvc;
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
	l_szFileName = (char*) m_pcFile->GetName();
	m_pcParms->SetDataFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "log");
	m_pcParms->SetLogFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "rst");
	m_pcParms->SetRestartFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "prm");
	m_pcParms->WriteParmFile(m_pcParms->GetParmFileName(),
				         	 l_szFileName.getPointer());

	//
	//  Invoke the PUT utility (KOMQPUTC)
	//
	char l_PgmName[] = "C:\\mqclient\\bin\\komqputc.exe";
	l_lResult  =  m_pcSysCmd->SpawnWait(l_PgmName, 
							  l_szFileName.getPointer());

	if (l_lResult == KOCC_GOOD) m_pcParms->SetFilesProcessed(
	                            m_pcParms->GetFilesProcessed() + 1);
	else l_lResult = KORC_PROCESSOR_ERROR;
    
/*
	if (m_pcParms->IsDeblockingSpecified()) {
		m_pcFMSrvc = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);
        l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Deblock(
				     (char*) m_pcFile->GetName(), 
						     m_pcParms->GetBufferSize(), 
							 m_pcParms->GetRecordLength());
		delete[] m_pcFMSrvc;
	} // End If
*/
	m_pcFMSrvc = new CFileModificationSrvc(m_pcParms);

	if ((l_lResult == KOCC_GOOD) 
	&&   m_pcParms->IsFileDispArchive()){
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcParms->GetDataFileName());
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcParms->GetCtlFileName());
		l_lResult  = m_pcFMSrvc->Archive((char*) m_pcParms->GetLogFileName());
		l_lResult  = m_pcFMSrvc->Archive(l_szFileName.getPointer());
	} /* End If */

	if ((l_lResult == KOCC_GOOD) 
	&&   m_pcParms->IsFileDispDelete()){
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetDataFileName());
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetCtlFileName());
		l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetLogFileName());
		l_lResult  = m_pcFMSrvc->Delete(l_szFileName.getPointer());
	} /* End If */
	delete m_pcFMSrvc;

	m_pcFileList->popCursor();
	m_pcFile = (CFile*) m_pcFileList->next();
} // End while 

return l_lResult;
} /* End Execute */


char* CMonitoringSrvc::GetStatus(){
	return "OK!";
} /* End GetStatus */





/*
void CMonitoringSrvc::initSelf(CExtSrvcsParamFile* a_pcParms){

//-----------------------------------------------------------------+
// Initialize Parameter and Log Object Pointers
//-----------------------------------------------------------------+
m_pcParms = a_pcParms;
CService::initSelf(m_pcLog);


//-----------------------------------------------------------------+
// Transfer any Parameter errors found in initSelf to the Log
//-----------------------------------------------------------------+
m_pcLog->LogErrors(*m_pcParms);
} /* End CMonitoringSrvc */







