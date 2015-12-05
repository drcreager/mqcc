//---------------------------------------------------------------------+
//  File:     CGetFilesSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the CGetFilesSrvc
//            Class.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
#include "GetFilesSrvc.h"

CGetFilesSrvc::CGetFilesSrvc(CExtSrvcsParamFile* a_pcParms, 
							 CLog*               a_pcLog){
is_InstanceName = "Get Files Service Business Logic Layer";
if (isValidPointer(a_pcParms)) m_pcParms = a_pcParms;
if (isValidPointer(a_pcLog))   m_pcLog = a_pcLog;
m_pcFNameSrvc = new CFileNameSrvc(a_pcParms);
m_bFileSetComplete = false;
} /* End CPutFilesSrvc */




CGetFilesSrvc::~CGetFilesSrvc(){
if (isValidPointer(m_pcFNameSrvc)) delete m_pcFNameSrvc;
if (isValidPointer(m_pcParms)) m_pcParms = NULL;
if (isValidPointer(m_pcLog))   m_pcLog = NULL;
}


//---------------------------------------------------------------------+
//  Method:   CheckParms
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Validate Parameters needed for the GetFiles Service.
//
//            GetFiles
//            - Valid QMgrName
//            - Valid ControlQueueName
//            - Valid DestDir
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
long  CGetFilesSrvc::CheckParms(){

	long ll_return = KOCC_GOOD;
CString	l_szErrText;


//
// Call Ancestor to validate parms needed at "MonitoringSrvc" level.
//
ll_return = CMonitoringSrvc::CheckParms();

if (ll_return == KOCC_GOOD) {
	//
	//  Validate the mandatory parameters for GetFiles
	//
	(m_pcParms->IsValidQueueMgrName())  ? 0 : ll_return = KOCC_FAIL;
	(m_pcParms->IsValidCtlQueueName())  ? 0 : ll_return = KOCC_FAIL;

	if (ll_return == KOCC_FAIL){
		l_szErrText = "Missing Mandatory Parameter(s).";
		m_pcLog->LogError(KORC_INVALID_PARM_COMBINATION, KOCC_FAIL, l_szErrText,
				"CGetFilesSrvc::CheckParms():1",
				is_InstanceName.getPointer());
	} /* End If */
} /* End If */
return ll_return;
} /* End CheckParms */


//---------------------------------------------------------------------+
//  Method:   Execute
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Execute the GetFiles Service.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CGetFilesSrvc::Execute(){

long        l_lResult     = KOCC_GOOD;
long		l_lFinal;

CString     l_szFileName;
CString     l_szNewFileName;
CString     l_szErrText;
long        l_lRcrdCnt    = 0; 
CDataHeader* l_pcDataHdr;
char        l_szSeps[]   = " ,\t\n";
CString		l_szProgram;
CString		l_szArgument;
CString     ls_msg;


//
//  Instantiate a DeblockingSrvc and DataHeader objects
//
m_pcFMSrvc = (CBlockingSrvc*) new CBlockingSrvc(m_pcParms);
l_pcDataHdr = new CDataHeader(m_pcParms);

//
//  Construct a Batch List from the current Control Queue
//
m_pcBatchList = new CBatchList(m_pcLog, m_pcParms, 
                       (char*) m_pcParms->GetMessageID());

if (m_pcBatchList->count() > 0 ) {
	//
	//  Log and tally the results of the Control Queue scan
	//
	ls_msg = "";
	ls_msg << "\nThe " << m_pcBatchList->count() 
		   << " batches found in queues were:";
	m_pcLog->WriteLog(ls_msg,LOGCONSOLE + LOGFILE);

	m_pcLog->WriteLog(*m_pcBatchList->getBatchList(),
		              LOGCONSOLE + LOGFILE);

	m_pcParms->SetFilesLocated(
	           m_pcParms->GetFilesLocated() 
			   + m_pcBatchList->count());
}
else l_lResult = KORC_NO_DATA;

//
//  Process each batch in the list.
//
m_pcBatch = (CString*) m_pcBatchList->top();
while ((!m_pcBatchList->endOfList()) &&  (l_lResult == KOCC_GOOD)){
	m_pcBatchList->pushCursor();
    l_lRcrdCnt = 0;
	
	//
	//  Generate File Names for use as parameters 
	//
	/*
	l_szFileName = m_pcParms->GetDstDir();
	l_szFileName << m_pcFNameSrvc->m_FileDlmtr
	             << m_pcFNameSrvc->getFileName(
				    ((CBlockingSrvc*) m_pcFMSrvc)->setTempFileName());
	*/
	l_szFileName = m_pcFNameSrvc->CreateTempFileName(m_pcParms->GetDstDir());
	//
	//  Allocate sub-directories per specified file path 
	//
	m_pcFMSrvc->MakeDir(m_pcFNameSrvc->getFilePath(&l_szFileName));

	//
	//  Generate a Parameter file
	//
	l_szFileName = ChangeFileExt(&l_szFileName, "dat");
	m_pcParms->SetDataFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "ctl");
	m_pcParms->SetCtlFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "log");
	m_pcParms->SetLogFileName(l_szFileName.getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "rst");
	m_pcParms->SetRestartFileName(l_szFileName.getPointer());

	//  Select the specific batch to "GET"
	m_pcParms->SetMessageID(m_pcBatch->getPointer());

	l_szFileName = ChangeFileExt(&l_szFileName, "prm");
	m_pcParms->WriteParmFile(m_pcParms->GetParmFileName(),
				         	 l_szFileName.getPointer());
	//
	//  Invoke the KOMQGET utility
	//
	if (m_pcParms->GetGetProcessor() != NULL){
		l_lResult  =  m_pcSysCmd->SpawnWait(
			                  m_pcParms->GetGetProcessor(), 
							  l_szFileName.getPointer());
		//
		//  On Success, tally the file
		//
		if (l_lResult == KOCC_GOOD) m_pcParms->SetFilesProcessed(
									m_pcParms->GetFilesProcessed() + 1);
		//
		//  On QueueEmpty, delete all transaction files to prepare for 
		//                 a subsequent retry.
		//
		else if ((l_lResult == KORC_EMPTY_QUEUE) 
			 ||  (l_lResult == KORC_NO_CONTROL_MSG)
			 ||  (l_lResult == KORC_MYSTERY_VALUE)) {
			m_pcFMSrvc->Delete((char*) m_pcParms->GetDataFileName());
			m_pcFMSrvc->Delete((char*) m_pcParms->GetCtlFileName());
			m_pcFMSrvc->Delete((char*) m_pcParms->GetLogFileName());
			m_pcFMSrvc->Delete((char*) m_pcParms->GetRestartFileName());
			m_pcFMSrvc->Delete((char*) l_szFileName.getPointer());
		}
		//
		//  On UnknownError, Log Diagnosis information and leave files
		//
		else {
			l_szErrText = "Processor Error: Get RC(";
			l_szErrText += l_lResult;
			l_szErrText += ")";
			m_pcLog->LogError(l_lResult, KOCC_FAIL, 
			l_szErrText, "CGetFilesSrvc::Execute:1",
			is_InstanceName.getPointer());
			l_lResult = KORC_PROCESSOR_ERROR;
		} // end if
	}
	else {
		l_szErrText = "Parm Error: Get Processor not Specified!";
		m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
		l_szErrText, "CGetFilesSrvc::Execute:2",
		is_InstanceName.getPointer());
		l_lResult = KOCC_FAIL;
	}  /* End If */


	if (l_lResult == KOCC_GOOD) {
		l_lResult = ((CBlockingSrvc*) m_pcFMSrvc)->GetFileAttributes(
		            (char*) m_pcParms->GetDataFileName(), l_pcDataHdr);

		//
		//  Duplicate the Source Host's directory structure and
		//  Rename the received files to the original dir\name spec
		//
		l_szFileName    = (char*) m_pcParms->GetDataFileName();
		l_szNewFileName = m_pcParms->GetDstDir();
		l_szNewFileName << m_pcFNameSrvc->m_FileDlmtr
	                    << l_pcDataHdr->getSrcFileName();
		RenameFileSet(&l_szFileName, &l_szNewFileName);

		//
		//  Allocate a file and process as necessary
		//
		m_pcFile = new CFile;
		m_pcFile->initSelf((char*) m_pcParms->GetDataFileName());
    
		//
		//  Check for File Blocking Specification with the file
		//
		if (l_pcDataHdr->isBlocked()) {
			//
			//l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Deblock(
			//			 (char*) m_pcFile->GetName(), true, true);
			//

			//  ** Diagnostic code **
			//cout << "GetFilesSrvc::Execute" << endl
			//	 << "FileName: " << (char*) m_pcFile->GetName() << endl
			//	 << "BlockLen: " << l_pcDataHdr->getBlkLen() << endl
			//	 << "RcrdLen : " << l_pcDataHdr->getRcrdLen() << endl; 
			
			l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Deblock(
						 (char*) m_pcFile->GetName(), 
								 l_pcDataHdr->getBlkLen(), 
								 l_pcDataHdr->getRcrdLen(),
								 true, true);

			if (l_lResult != KOCC_GOOD) { 
				CopyErrors((CBlockingSrvc*) m_pcFMSrvc);
			} // end if
		} // end if

		//
		//  Deblock files without File Blocking Headers
		//
		else if (m_pcParms->IsDeblockingSpecified()) {
			l_lResult  = ((CBlockingSrvc*) m_pcFMSrvc)->Deblock(
						 (char*) m_pcFile->GetName(), 
								 m_pcParms->GetBufferSize(), 
								 m_pcParms->GetRecordLength(), true, false);
			
			if (l_lResult != KOCC_GOOD) { 
				CopyErrors((CBlockingSrvc*) m_pcFMSrvc);
			} // end if
		} // End If

		//
		//  Warn if Deblocking was not successful?
		//
		if (l_lResult != KOCC_GOOD) {
			l_szErrText = "File Error: File Deblocking failed for ";
			l_szErrText << (char*) m_pcFile->GetName();
			m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
				l_szErrText, "CGetFilesSrvc::Execute:3",
				is_InstanceName.getPointer());
		} /* End If */

		//
		//  Dispose of transaction files (.prm .ctl .log) as indicated by user
		//
		if ((l_lResult == KOCC_GOOD) 
		&&   m_pcParms->IsFileDispDelete()){
			l_lResult  = m_pcFMSrvc->Delete((char*) l_szNewFileName.c_str());
			l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetCtlFileName());
			l_lResult  = m_pcFMSrvc->Delete((char*) m_pcParms->GetLogFileName());
			if (l_lResult != 0){
				l_szErrText = "File Error: File Delete failed!";
				m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
					l_szErrText, "CGetFilesSrvc::Execute:4",
					is_InstanceName.getPointer());
			} /* End If */
		} /* End If */
	}
	else {
		l_lResult = KORC_PROCESSOR_ERROR;
	} // End If

	m_pcBatchList->popCursor();
	m_pcBatch = (CString*) m_pcBatchList->next();
} // End while 

//
//  On Batches_Found, Determine if the FileSet is Complete
//
l_lFinal = l_lResult;
if (m_pcBatchList->count() > 0){
	m_bFileSetComplete = l_pcDataHdr->isFileSetComplete();
	CopyErrors(l_pcDataHdr);
	//
	//  Issue the appropriate post processor command
	//
	if (m_bFileSetComplete) {
		if (m_pcParms->GetOnSuccessCmd() != NULL){
			l_szProgram  = strtok( m_pcParms->GetOnSuccessCmd(), l_szSeps);
			l_szArgument = strtok( NULL, l_szSeps);
			l_lResult  =  m_pcSysCmd->Spawn(
			              l_szProgram.c_str(), 
						  l_szArgument.c_str());

			if (l_lResult == -1){
				l_szErrText = "Error: OnSuccess Command failed!";
				m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
					l_szErrText, "CGetFilesSrvc::Execute:5",
					is_InstanceName.getPointer());
			} /* End If */

		} /* End If */
	}
	else {
		if (m_pcParms->GetOnFailCmd() != NULL){
			l_szProgram  = strtok( m_pcParms->GetOnFailCmd(), l_szSeps);
			l_szArgument = strtok( NULL, l_szSeps);
			l_lResult  =  m_pcSysCmd->Spawn(l_szProgram.c_str(), 
						                    l_szArgument.c_str());
			if (l_lResult == -1){
				l_szErrText = "Error: OnFail Command failed!";
				m_pcLog->LogError(KORC_RESOURCE_ERROR, KOCC_FAIL, 
					l_szErrText, "CGetFilesSrvc::Execute:6",
					is_InstanceName.getPointer());
			} /* End If */
		} /* End If */
	} /* End If */


} /* end if */

//
//  Release the BlockingSrvc and DataHeader objects
//
delete l_pcDataHdr;
delete m_pcFMSrvc;

return l_lFinal;
} /* End Execute */


//---------------------------------------------------------------------+
//  Method:   isFileSetComplete
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Getter function for the FileSetComplete attribute.
//
//  Args:     None
//
//  Returns:  True  = Fileset IS complete
//            False = Fileset IS NOT complete.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CGetFilesSrvc::isFileSetComplete(){
return m_bFileSetComplete;
} /* End isFileSetComplete */





//---------------------------------------------------------------------+
//  Method:   RenameFileSet
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Rename a set of files retrieved from MQ Series using the
//            data file name as a base.
//
//  Args:     CString*    Current file name
//            CString*    New file name
//
//  Returns:  Nothing
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CGetFilesSrvc::RenameFileSet(CString*     l_szFileName,
								  CString*     l_szNewFileName){
//
//  Duplicate the Source Host's directory structure
//
m_pcFMSrvc->MakeDir(m_pcFNameSrvc->getFilePath(l_szNewFileName));

//
//  Rename the received files to the original dir\name spec
//
RenameFile(l_szFileName, l_szNewFileName, "rst");
RenameFile(l_szFileName, l_szNewFileName, "dat");
m_pcParms->SetDataFileName(l_szNewFileName->c_str());
RenameFile(l_szFileName, l_szNewFileName, "ctl");
m_pcParms->SetCtlFileName(l_szNewFileName->c_str());
RenameFile(l_szFileName, l_szNewFileName, "log");
m_pcParms->SetLogFileName(l_szNewFileName->c_str());
RenameFile(l_szFileName, l_szNewFileName, "prm");
}  /* end RenameFileSet */



//---------------------------------------------------------------------+
//  Method:   RenameFile
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Rename a file using a specific file extension.
//
//  Args:     CString*    Current file name
//            CString*    New file name
//            char*       file extension with the dot "." separator
//
//  Returns:  Nothing
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CGetFilesSrvc::RenameFile(CString* l_pcOldFile, 
							   CString* l_pcNewFile,
							   char*    l_szExtension){
//
//  Rename the received files to the original dir\name spec
//
*l_pcOldFile = ChangeFileExt(l_pcOldFile, l_szExtension);
*l_pcNewFile = ChangeFileExt(l_pcNewFile, l_szExtension);
m_pcFMSrvc->Rename(l_pcOldFile, l_pcNewFile);
}  /* end RenameFile */