//---------------------------------------------------------------------+
//  File:     FileModificationSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the 
//            CFileModificationSrvc Class.  This base class provides a 
//            common ancestor for various classes that modify files in 
//            various ways. (e.g. Blocking, Encrypting, Encoding, etc)
//
//  History: 
//  11/10/00 DRC Created original version. 
//---------------------------------------------------------------------+
#include "FileModificationSrvc.h"






//----------------------------------------------------------------------
// Function: Archive
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will Archive a file by renaming it 
//           from its current location to a sub-directory specifed 
//           by the user as the archive location.
//
// Args:     char*(Fully qualified File name of the subject file)
// Returns:  long(Return Code) 
//           0 = successful 
//           1 = warning
//           2 = error
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::Archive(char* a_szFileName){
CString l_szArchiveName;
CString l_szFileName;

//
// Allocate the Archive Directory 
// -  Ignore errors if it was already allocated
// -  Set the Archive Path Name
//
MakeDir(m_pcParms->GetFileArchive());

l_szArchiveName = m_pcParms->GetFileArchive();
l_szArchiveName << m_pcFNameSrvc->m_FileDlmtr;

//
//  Construct a PathName using the Archive as a Qualifier
//
l_szFileName = a_szFileName;
l_szArchiveName << m_pcFNameSrvc->getArchiveName(&l_szFileName);

//
//  Allocate Archival Sub-directories and 
//  Move the file into the Archive
//
MakeDir(m_pcFNameSrvc->getFilePath(&l_szArchiveName));
return Rename(a_szFileName,l_szArchiveName.getPointer());
} /* End Archive */





//----------------------------------------------------------------------
// Function: CFileModificationSrvc (Default Constructor)
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will initialize this object.
// Args:     None.
// Returns:  Nothing.
// History: 
// 09/19/00 DRC Created original version.
// 11/10/00 DRC Add OS specific initialization of m_FileDlmtr 
//----------------------------------------------------------------------
CFileModificationSrvc::CFileModificationSrvc(CExtSrvcsParamFile* a_pcParms){
if (isValidPointer(a_pcParms))  m_pcParms = a_pcParms;                   

m_pFile1 = new CFile();
m_pFile2 = new CFile();
m_pBuf1  = new CBuffer();
m_pBuf2  = new CBuffer();
m_pcFNameSrvc = new	CFileNameSrvc(a_pcParms);
m_pcSysCmd    = new CSysCmd();
m_lRC    = KOCC_GOOD;
}  /* end CFileModificationSrvc */





//----------------------------------------------------------------------
// Function: ~CFileModificationSrvc (Default Destructor)
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will release this object's resources.
// Args:     None.
// Returns:  Nothing.
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
CFileModificationSrvc::~CFileModificationSrvc(){
if (isValidPointer(m_pFile1)){
	delete m_pFile1;
	m_pFile1 = NULL;
} /* End If */

if (isValidPointer(m_pFile2)){
	delete m_pFile2;
	m_pFile2 = NULL;
} /* End If */

if (isValidPointer(m_pBuf1)){
	delete m_pBuf1;
	m_pBuf1 = NULL;
} /* End If */

if (isValidPointer(m_pBuf2)){
	delete m_pBuf2;
	m_pBuf2 = NULL;
} /* End If */

if (isValidPointer(m_pcFNameSrvc)){
	delete m_pcFNameSrvc;
	m_pcFNameSrvc = NULL;
} /* End If */

if (isValidPointer(m_pcSysCmd)){
	delete m_pcSysCmd;
	m_pcSysCmd = NULL;
} /* End If */
} /* End ~CFileModificationSrvc */





//----------------------------------------------------------------------
// Function: Copy
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will copy an input file into an output file.
// Args:     None.
// Returns:  long(Return Code) 
//           0 = successful 
//           1 = warning
//           2 = error
// History: 
// 09/19/00 DRC Created original version. 
// 05/08/02 DRC Added conditional logic to avoid fall thru when errors
//              occur and reflect the lastCC value on the log.
//----------------------------------------------------------------------
long CFileModificationSrvc::Copy(){

//
// Prime the I/O Loop
//
m_lRC = m_pFile1->Read(*m_pBuf1);
if ((m_lRC != KOCC_GOOD) && (!m_pFile1->eof())) {
	iu_errors.AddError(m_pFile1->getLastCC(), m_lRC, "Error from Read",
	"CFileModificationSrvc::Copy:1",
	is_InstanceName.getPointer());
}
else {
	//
	// I/O Loop
	//
	while (m_lRC == KOCC_GOOD){
		*m_pBuf2 = m_pBuf1;
		m_lRC = m_pFile2->Write(*m_pBuf2);
		if (m_lRC == KOCC_GOOD){
			m_lRC = m_pFile1->Read(*m_pBuf1);
			if (m_lRC != KOCC_GOOD){
				iu_errors.AddError(m_pFile1->getLastCC(), m_lRC, "Error from Read",
				"CFileModificationSrvc::Copy:3",
				is_InstanceName.getPointer());
			} // End If
		}
		else{
			iu_errors.AddError(m_pFile2->getLastCC(), m_lRC, "Error from Write",
			"CFileModificationSrvc::Copy:2",
			is_InstanceName.getPointer());
		} // End If
	} // End While 

	//
	//  on single record files, write it out
	//
	if (m_pFile1->eof()){
		if (m_lRC == KOCC_WARNING) m_lRC = KOCC_GOOD;
		if (m_pBuf1->getDataLen() > 0){
			*m_pBuf2 = m_pBuf1;
			m_lRC = m_pFile2->Write(*m_pBuf2);
			if (m_lRC != KOCC_GOOD){
				iu_errors.AddError(m_pFile2->getLastCC(), m_lRC, "Error from Write",
				"CFileModificationSrvc::Copy:4",
				is_InstanceName.getPointer());
			} // End If 
		} // End If 
	} // End If 
} // End If 

return m_lRC;
}  /* End of Copy */



//----------------------------------------------------------------------
// Function: CopyToMQS
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will copy a file inserting a data header to 
//           prepare it for transfer through MQ Series. 
//
// Args:     CDataHeader*    Address of a DataHeader Object that contains
//                           the information to be inserted.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::CopyToMQS(CDataHeader* a_pcFileHdr){

m_lRC = InsertHdr(a_pcFileHdr);
if (m_lRC == KOCC_GOOD) m_lRC = Copy();
return m_lRC;
} /* End CopyToMQS */





/*
//----------------------------------------------------------------------
// Function: CopyToHost
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will copy a file that has been transfered 
//           using MQ Series.  It will extract the data header to 
//           prepare it for use as a host file.
// 
// Args:     CDataHeader*    Address of a DataHeader Object that will 
//                           hold the extracted information.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::CopyToHost(CDataHeader* a_pcFileHdr){

m_lRC = ExtractHdr(a_pcFileHdr);
if (m_lRC == KOCC_GOOD) m_lRC = Copy();
return m_lRC;
} /* End CopyToHost */








//----------------------------------------------------------------------
// Function: ExtractHdr
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function extract the data header from a file. 
// Args:     CDataHeader*    Address of a DataHeader Object to receive
//                           the extracted information.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::ExtractHdr(CDataHeader* a_pcFileHdr){
//
//  Clear the DataHeader Buffer
//
a_pcFileHdr->clear();
	
//
// Prime the I/O Loop
//
m_lRC = m_pFile1->Read(*m_pBuf1);

//
// I/O Loop
//
while ((m_lRC == KOCC_GOOD) && (!a_pcFileHdr->isHeaderComplete())){
	*m_pBuf2 = m_pBuf1;
	m_lRC = a_pcFileHdr->Write(*m_pBuf2);
	if (!a_pcFileHdr->isHeaderComplete()) 
		m_lRC = m_pFile1->Read(*m_pBuf1);
} /* End While */

//
//  on single record files, write it out
//
if (m_pFile1->eof()){
	if (m_lRC == KOCC_WARNING) m_lRC = KOCC_GOOD;
	if (m_pBuf1->getDataLen() > 0){
		*m_pBuf2 = m_pBuf1;
		m_lRC = a_pcFileHdr->Write(*m_pBuf2);
	} /* End If */
} /* End If */
return m_lRC;
} /* End ExtractHdr */




//----------------------------------------------------------------------
// Function: Delete
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will delete a file.
//
//           This function returns 0 if the file is successfully deleted. 
//           Otherwise, it returns -1 and sets errno either to EACCES to 
//           indicate that the path specifies a read-only file, or to 
//           ENOENT to indicate that the filename or path was not found 
//           or that the path specifies a directory. 
//           This function fails and returns -1 if the file is open.
//
//EACCES 13  Permission denied The file's permission setting does not allow 
//           the specified access. This error signifies that an attempt was 
//           made to access a file (or, in some cases, a directory) in a way 
//           that is incompatible with the file's attributes.
//
//ENOENT  2  No such file or directory The specified file or directory does 
//           not exist or cannot be found. This message can occur whenever 
//           a specified file does not exist or a component of a path does 
//           not specify an existing directory.
//
// Args:     char*    Name of the File
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::Delete(char* a_szFileName){
return (long) remove(a_szFileName);
} /* End Delete */




//----------------------------------------------------------------------
// Function: getBlkCount
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Getter function fo rthe file block count.
// Args:     Address of the file object.
// Returns:  long(Block Count) 
//           0 = successful 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::getBlkCount(CFile* a_pcFile){
return 	a_pcFile->getBlkCount();
} /* end getBlkCount */




//----------------------------------------------------------------------
// Function: getRcrdCount
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Getter function for the file record count.
// Args:     Address of the file object.
// Returns:  long(Record Count) 
//           0 = successful 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::getRcrdCount(CFile* a_pcFile){
return a_pcFile->getBlkCount();
} /* end getRcrdCount */




//----------------------------------------------------------------------
// Function: initSrcFile
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Initialize this instance of this class.
// Args:     char*    Source File Name
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::initSrcFile( char* a_szFileName){
m_lRC = KOCC_GOOD;
CString l_ErrMsg("File Open failed for ");

m_pFile1->initSelf(a_szFileName);

if (m_pcParms->GetRecordLength() <=0)
	m_pFile1->getMaxRcrdLen();

m_pFile1->Open(ios::in | ios::nocreate);
if (!m_pFile1->is_open()){
	m_lRC = KOCC_FAIL;
	l_ErrMsg += a_szFileName;
	iu_errors.AddError(2, m_lRC, l_ErrMsg.c_str(),
	"CFileModificationSrvc::initSrcFile:1",
	is_InstanceName.getPointer());
} /* End If */
return m_lRC;
} /* End initSrcFile */




//----------------------------------------------------------------------
// Function: initDstFile
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Initialize this instance of this class.
// Args:     char*    Destination File Name
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::initDstFile( char* a_szFileName){
m_lRC = KOCC_GOOD;
CString l_ErrMsg("File Open failed for ");

m_pFile2->initSelf(a_szFileName);
m_pFile2->Open(ios::out);
if (!m_pFile2->is_open()){
	m_lRC = KOCC_FAIL;
	l_ErrMsg += a_szFileName;
	iu_errors.AddError(2, m_lRC, l_ErrMsg.c_str(),
	"CFileModificationSrvc::initDstFile:1",
	is_InstanceName.getPointer());
} /* End If */

return m_lRC;
} /* End initDstFile */






//----------------------------------------------------------------------
// Function: initSelf
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Initialize this instance of this class.
// Args:     char*    Source File Name
//           char*    Destination File Name
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::initSelf( char* a_szSrcFileName,   
									  char* a_szDstFileName){
m_lRC = KOCC_GOOD;
CString l_ErrMsg("Unable to Init ");

m_lRC = initSrcFile(a_szSrcFileName);
if (m_lRC == KOCC_GOOD){ 
	m_lRC = initDstFile(a_szDstFileName);
	if (m_lRC != KOCC_GOOD){
		l_ErrMsg += "Destination File: ";
		l_ErrMsg += a_szDstFileName;
		iu_errors.AddError(2, m_lRC, l_ErrMsg.c_str(),
		"CFileModificationSrvc::initSelf:1",
		is_InstanceName.getPointer());
	} // end if
}
else {
	l_ErrMsg += "Source File: ";
	l_ErrMsg += a_szSrcFileName;
	iu_errors.AddError(2, m_lRC, l_ErrMsg.c_str(),
		"CFileModificationSrvc::initSelf:1",
		is_InstanceName.getPointer());
} // end if

return m_lRC;
} /* End initSelf */






//----------------------------------------------------------------------
// Function: InsertHdr
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will insert a data header into a file. 
// Args:     CDataHeader*    Address of a DataHeader Object that contains
//                           the information to be inserted.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::InsertHdr(CDataHeader* a_pcFileHdr){
//
// Prime the I/O Loop
//
m_lRC = a_pcFileHdr->Read(*m_pBuf1);

//
// I/O Loop
//
while (m_lRC == KOCC_GOOD){
	*m_pBuf2 = m_pBuf1;
	m_lRC = m_pFile2->Write(*m_pBuf2);
	m_lRC = a_pcFileHdr->Read(*m_pBuf1);
} /* End While */

//
//  on single record files, write it out
//
if (a_pcFileHdr->eof()){
	if (m_lRC == KOCC_WARNING) m_lRC = KOCC_GOOD;
	if (m_pBuf1->getDataLen() > 0){
		*m_pBuf2 = m_pBuf1;
		m_lRC = m_pFile2->Write(*m_pBuf2);
	} /* End If */
} /* End If */

return m_lRC;
} /* End InsertHdr */





//----------------------------------------------------------------------
// Function: MakeDir
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will create a (sub-)directory by wrappering
//           the c library function _mkdir.  Only the final node of a 
//           multi level path name can be new.
//
// Args:     char*(Path name of the sub-directory or directory)
// Returns:  long(Return Code) 
//           0 = successful 
//          -1 = error with errno set as follows: 
//
//               EEXIST = Directory was not created because dirname 
//                        is the name of an existing file, directory, 
//                        or device
//
//               ENOENT = Path was not found
//
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::MakeDir(char* a_szPathName){
long l_RC = 0;

l_RC = m_pcSysCmd->MakeDir(a_szPathName);
//
//  ON ENOENT, Build the higher level(s) of directory.
//
if ((l_RC == -1) && (errno == ENOENT)) {
	//
	//  Build the Higher level Node(s)
	//
	CString l_szPathName(a_szPathName);
    l_szPathName = l_szPathName.mid(1, m_pcFNameSrvc->getDlmtrPos(&l_szPathName) - 1);
	if (l_szPathName.vlen() > 2) l_RC = MakeDir(l_szPathName.getPointer());
	//
	//  Retry the build of this level's node
	//
	l_RC = m_pcSysCmd->MakeDir(a_szPathName);
} /* End MakeDir */

return l_RC;
} /* end MakeDir */




//----------------------------------------------------------------------
// Function: Rename
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will rename a file.
// Args:     char*    Old Name of the Existing File
//           char*    New, unused Name for the File
// Returns:  long(Return Code) 
//           0 = successful
//           errno = unsuccessful  
// History: 
// 09/19/00 DRC Created original version.
// 03/08/01 DRC Added returning errno in non-zero RC conditions. 
//----------------------------------------------------------------------
long CFileModificationSrvc::Rename(char* a_szOldFileName, 
								   char* a_szNewFileName){
if (rename(a_szOldFileName, a_szNewFileName) == 0)
    return 0L;
else
	return (long) errno;
} /* End Rename */





//----------------------------------------------------------------------
// Function: Rename
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will rename a file.
// Args:     char*    Old Name of the Existing File
//           char*    New, unused Name for the File
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::Rename(CString* a_szOldFileName, 
								   CString* a_szNewFileName){
return (long) Rename(a_szOldFileName->getPointer(), 
					 a_szNewFileName->getPointer());
} /* End Rename */





//----------------------------------------------------------------------
// Function: termSrcFile
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Terminate this instance of this class.
// Args:     None.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::termSrcFile(){
m_lRC = m_pFile1->Close();
if (m_lRC == KOCC_GOOD) 
	m_lRC = m_pFile1->termSelf();
return m_lRC;
} /* End termSrcFile */




//----------------------------------------------------------------------
// Function: termDstFile
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Terminate this instance of this class.
// Args:     None.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::termDstFile(){
m_lRC = m_pFile2->Close();
if (m_lRC == KOCC_GOOD)	
	m_lRC = m_pFile2->termSelf();

return m_lRC;
} /* End termDstFile */




//----------------------------------------------------------------------
// Function: termSelf
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Terminate this instance of this class.
// Args:     None.
// Returns:  long(Return Code) 
//           0 = successful 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileModificationSrvc::termSelf(){
m_lRC = termDstFile();

if (m_lRC == KOCC_GOOD) 
	m_lRC = termSrcFile();

return m_lRC;
} /* End termSelf */



