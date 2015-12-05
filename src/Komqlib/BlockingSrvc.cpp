//---------------------------------------------------------------------+
//  File:     CBlockingSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation of the CBlockingSrvc class. 
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "BlockingSrvc.h"

//----------------------------------------------------------------------
// Function: CBlockingSrvc (constructor)
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     tbd
// Args:     ExtSrvcParmFile*    Address of the Parm file object
//
// Returns: 
//
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
CBlockingSrvc::CBlockingSrvc(CExtSrvcsParamFile* a_pcParms)
              :CFileModificationSrvc(a_pcParms) {

m_pcDataHdr   = new CDataHeader(a_pcParms);
is_InstanceName = "BlockingSrvc";
} /* end CBlockingSrvc */

CBlockingSrvc::~CBlockingSrvc(){
if (isValidPointer(m_pcDataHdr))   delete m_pcDataHdr;
m_pcDataHdr = NULL;
}


//----------------------------------------------------------------------
// Function: Block
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will convert a line feed delimited (PC style)
//           sequential file into a fixed length blocked (Mainframe 
//           style) sequential file.
// Args:     char*    Name of the File
//           long     Block length of the Output file.
//           int      Logical record length of the Output file.
// Returns: 
// History: 
// 09/19/00 DRC Created original version. 
// 11/10/00 DRC Added a Record Length parameter and triggered browsing
//              the file for record length only when the parm = 0.
//              Added the FileList to the parameters.
//----------------------------------------------------------------------
long CBlockingSrvc::Block(char* a_szFileName, long       a_lBlkLen, 
						  long  a_lRcrdLen,   CFileList* a_pcFileList,
						  BOOL  a_bReplace){

//CString   l_szTempFileName;
CString   l_szSrcFileName;

//
//  Move the temp file into the original sub-directory
//
m_pcFNameSrvc->CreateTempFileName(m_pcFNameSrvc->getFilePath(a_szFileName));

/*
l_szTempFileName = m_pcFNameSrvc->getFilePath(a_szFileName);
l_szTempFileName << m_pcFNameSrvc->m_FileDlmtr 
                 << m_pcFNameSrvc->getFileName(&m_szTempFileName);
m_szTempFileName = l_szTempFileName;
*/

//
// init file before the buffer to determine the max rcrd len for the 
// buffer allocation
//
m_lRC = this->initSelf(a_szFileName, m_pcFNameSrvc->getTempFileName());
if (m_lRC == KOCC_GOOD) {
	if (a_lRcrdLen <=0) a_lRcrdLen = m_pFile1->getMaxRcrdLen();
	m_pBuf1->initSelf(a_lBlkLen, true, a_lRcrdLen);
	m_pBuf2->initSelf(a_lBlkLen);
	//
	//  Construct a Data Header for this file.
	//
	#ifdef _WIN32
	 m_pcDataHdr->setOS("Windows");
	 m_pcDataHdr->setProgram("KOMQUTLC");
	#else
	 m_pcDataHdr->setOS("Unix");
	 m_pcDataHdr->setProgram("KOMQUTL");
	#endif
	m_pcDataHdr->setVersion("1.0.0.1");
	if (m_pcParms->IsFileSetSpecified()){
		m_pcDataHdr->setSetName(m_pcParms->GetSetName());
		m_pcDataHdr->setSetClass(m_pcParms->GetSetClsName());
		m_pcDataHdr->setSetList(a_pcFileList);
	} /* End If */
	m_pcDataHdr->setSrcFileClass(m_pcParms->GetDataClsName());
	//m_pcDataHdr->setRcrdCnt(0);
	//
	//  Strip the high level qualifier from the Source file 
	//  name and store it into the DataHeader 
	//
	l_szSrcFileName = a_szFileName;
	l_szSrcFileName = m_pcFNameSrvc->getArchiveName(&l_szSrcFileName);
	m_pcDataHdr->setSrcFileName(l_szSrcFileName.c_str());

	m_pcDataHdr->setFileFormat("FixedBlocked");
	m_pcDataHdr->setBlkLen(a_lBlkLen);
	m_pcDataHdr->setRcrdLen(a_lRcrdLen);
	m_pcDataHdr->LoadXML();

	//
	//
	//
	m_lRC = this->CopyToMQS(m_pcDataHdr);
	m_lRC = this->termSelf();
}
else {
	iu_errors.AddError(2, m_lRC,
			"Unable to Initialize Blocking",
			"CBlockingSrvc::Block:1",
			is_InstanceName.getPointer());
} /* End If */

//
//  On specified, replace the original file with its blocked counterpart
//
if (a_bReplace) {
	if (m_lRC == KOCC_GOOD) 
		m_lRC = this->Delete(a_szFileName);
	if (m_lRC == KOCC_GOOD) 
		m_lRC = this->Rename(m_pcFNameSrvc->getTempFileName(), a_szFileName);
	else
		iu_errors.AddError(errno, m_lRC,
			"Unable to delete the original blocking source file.",
			"CBlockingSrvc::Block:2",
			is_InstanceName.getPointer());
} /* End If */

m_pBuf1->termSelf();
m_pBuf2->termSelf();
return m_lRC;
}  /* End of Block */



long CBlockingSrvc::Block(char* a_szFileName, CFileList* a_pcFileList,
						  BOOL  a_bReplace){
return Block(a_szFileName, m_pcParms->GetBufferSize(), 
			 m_pcParms->GetRecordLength(), a_pcFileList, a_bReplace);
}  /* End Block */



long CBlockingSrvc::Block(char* a_szFileName, long a_lBlkLen, 
						  long a_lRcrdLen,    BOOL  a_bReplace){
return Block(a_szFileName, a_lBlkLen, a_lRcrdLen, NULL, a_bReplace);
}  /* End Block */



long CBlockingSrvc::Block(char* a_szFileName, BOOL  a_bReplace){
return Block(a_szFileName, m_pcParms->GetBufferSize(), 
			 m_pcParms->GetRecordLength(), NULL, a_bReplace);
}  /* End Block */




//----------------------------------------------------------------------
// Function: Deblock
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will convert a fixed length blocked (Mainframe 
//           style) sequential file into a line feed delimited (PC style)
//           sequential file.
// Args:     char*    Name of the File
//           long     Block length of the Input file.
//           int      Logical record length of the Input file.
//
// Dev Log: 
// 01)  Handle it if there is no XML Document Header
//
// Returns: 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CBlockingSrvc::Deblock(char* a_szFileName, long a_lBlkLen, 
                            long  a_lRcrdLen,   BOOL a_bReplace,
                            BOOL  a_bRename){
CStringList     l_pcResult;
char*           l_szFileName = a_szFileName;
long            l_lRC = KOCC_GOOD;
CString         l_szNewFileName;

//
//  Retrieve the Attributes of the specified file. 
//
l_lRC = GetFileAttributes(a_szFileName, m_pcDataHdr);
if (l_lRC != KOCC_GOOD){
	iu_errors.AddError(errno, l_lRC,
			"Unable to Get File Attributes",
			"CBlockingSrvc::Deblock:1",
			is_InstanceName.getPointer());
} // end if



//
//  On Rename Specifed, reset the Source File Name
//
if (a_bRename) {
	l_szNewFileName = m_pcParms->GetDstDir();
	l_szNewFileName << m_pcFNameSrvc->m_FileDlmtr
					<< m_pcDataHdr->getSrcFileName();
	m_pcDataHdr->setSrcFileName(l_szNewFileName.c_str());
} /* End If */

//
//  Allocate sub-directories on this machine per specifications and 
//  the original sub-directory on the remote host. 
//
MakeDir(m_pcFNameSrvc->getFilePath(l_szNewFileName.c_str()));

//
//  Initialize files for I/O creating a temp file in the 
//  original sub-directory
//
l_lRC = this->initSelf(a_szFileName, m_pcFNameSrvc->CreateTempFileName(
               m_pcFNameSrvc->getFilePath(l_szNewFileName.c_str())));

if (l_lRC != KOCC_GOOD){
	iu_errors.AddError(errno, l_lRC,
			"Unable to initialize and create a temporary file.",
			"CBlockingSrvc::Deblock:2",
			is_InstanceName.getPointer());
} // end if

if (l_lRC == KOCC_GOOD) {
	l_szFileName = m_pcDataHdr->getSrcFileName();

	m_pBuf1->initSelf(m_pcDataHdr->getBlkLen() + 1);
	m_pBuf2->initSelf(m_pcDataHdr->getBlkLen() + 1,true,
					  m_pcDataHdr->getRcrdLen());
	m_pBuf2->setIOBlockingDir(CBUF_OUT_BOUND);

	//
	//  Skip past the Header info within the data file
	//
	this->ExtractHdr(m_pcDataHdr);

	//
	//  Deblock the file.
	//
	l_lRC = this->Copy();

	if (l_lRC != KOCC_GOOD){
	    iu_errors.AddError(errno, l_lRC,
			"Unable to copy / deblock the file.",
			"CBlockingSrvc::Deblock:3",
			is_InstanceName.getPointer());
	} // end if
} /* end If */
this->termSelf();

//
//  On Specified, Rename the deblocked version of the file. 
//  1)  Clean-up the file name used to unload from the queue
//  2)  Clean-up any previous files using the file name
//  3)  Build the sub-directory for the specified filename.
//  4)  Rename the deblocked file to its specified filename.
//
if ((l_lRC == KOCC_GOOD) && a_bReplace) {
	l_lRC = Delete(a_szFileName);
	if (l_lRC != KOCC_GOOD){	
		switch (errno){
		case EACCES:              // File Access error
			iu_errors.AddError(errno, l_lRC,
				"Unable to clean up the temporary file.",
				"CBlockingSrvc::Deblock:4",
				is_InstanceName.getPointer());
			l_lRC = KOCC_GOOD;
			break;

		case ENOENT:              // File not found
			l_lRC = KOCC_GOOD;
		} // end switch   
	} // end if

	l_lRC = Delete(l_szFileName);
	if (l_lRC != KOCC_GOOD){
		switch (errno){
		case EACCES:              // File Access error
			iu_errors.AddError(errno, l_lRC,
				"Unable to assure availability of the destination file name.",
				"CBlockingSrvc::Deblock:5",
				is_InstanceName.getPointer());
			l_lRC = KOCC_GOOD;
			break;

		case ENOENT:              // File not found
			l_lRC = KOCC_GOOD;
		} // end switch
	} // end if

	MakeDir(m_pcFNameSrvc->getFilePath(l_szFileName));
	l_lRC = Rename(m_pcFNameSrvc->getTempFileName(), l_szFileName);
	if (l_lRC != KOCC_GOOD){
	    iu_errors.AddError(errno, l_lRC,
			"Unable to rename the temporary file.",
			"CBlockingSrvc::Deblock:6",
			is_InstanceName.getPointer());

		if ((errno == EACCES)    //  Reset return code on 
		||  (errno == ENOENT))   //  predictable conditions
		   l_lRC = KOCC_GOOD; 
	} // end if
} /* end If */

m_pBuf1->termSelf();
m_pBuf2->termSelf();

return l_lRC;
}  /* End Deblock */






long CBlockingSrvc::Deblock(char* a_szFileName,    
							BOOL  a_bReplace,    
							BOOL  a_bRename){
return Deblock(a_szFileName, 2 mb, 4 kb, a_bReplace, a_bRename);
}  /* End Deblock */





//----------------------------------------------------------------------
// Function: GetFileAttributes
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     tbd.
// Args:     char*    Name of the File
//
// Returns: 
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
long CBlockingSrvc::GetFileAttributes(char* a_szFileName, 
									  CDataHeader* a_pcDataHdr){
CStringList	l_pcResult;
char*		l_szFileName = a_szFileName;

m_pBuf1->initSelf(2 mb);
m_pBuf2->initSelf(2 mb, true, 4 kb);

m_lRC = this->initSrcFile(a_szFileName);
if (m_lRC == KOCC_GOOD) {
	//
	//  Separate the Header info from the data file
	//
	m_lRC = this->ExtractHdr(a_pcDataHdr);
	//
	//  Extract Attribute information from the XML Document 
	//  and reset the I/O Buffers as specified.
	//
	if (m_lRC == KOCC_GOOD) {
		a_pcDataHdr->UnloadXML();
	} /* end If */
} /* end If */
this->termSrcFile();
m_pBuf2->termSelf();
m_pBuf1->termSelf();
return m_lRC;
}  /* End GetFileAttributes */




//----------------------------------------------------------------------
// Function: getRcrdCount
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Getter function for the Record Count Attribute.
// Args:     None
//
// Returns:  long      Count of the Records processed
// History: 
// 11/15/00 DRC Created original version. 
//----------------------------------------------------------------------
long CBlockingSrvc::getRcrdCount(){
return getBlkCount(m_pFile2);
} /* end getRcrdCount */






//----------------------------------------------------------------------
// Function: getTempFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Getter function for the TempFileName Attribute.
// Args:     None
//
// Returns:  char*      Address of the TempFileName string.
// History: 
// 11/15/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CBlockingSrvc::getTempFileName(){
return m_pcFNameSrvc->getTempFileName();
} /* end getTempFileName */






//----------------------------------------------------------------------
// Function: initSelf
// Written:  April 2002
// Author:   Daniel R Creager
// Desc:     Initialize this instance of this class.
// Args:     char*    Source File Name
//           char*    Destination File Name
// Returns:  As set by ancestral code. 
// History: 
// 04/02/2002 DRC Created original version. 
//----------------------------------------------------------------------
long CBlockingSrvc::initSelf( char* a_szSrcFileName,   
							  char* a_szDstFileName){
long l_lRC = CFileModificationSrvc::initSelf(a_szSrcFileName, 
											 a_szDstFileName);
//
//  Clear out previous Data Headers
//
m_pcDataHdr->clear();

return l_lRC;
} /* End initSelf */




//----------------------------------------------------------------------
// Function: setTempFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Setter function for the TempFileName Attribute.
// Args:     None
//
// Returns:  char*      Address of the TempFileName string.
// History: 
// 11/15/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CBlockingSrvc::setTempFileName(char* a_szDir){
	/*
	char* l_szTempFileName = NULL;

	l_szTempFileName = tmpnam(NULL); 

	if(l_szTempFileName == NULL) {
		l_szTempFileName = "~ESUCpy1.dat";
	} /* End If *//*
	m_szTempFileName.fill();
	#ifdef _WIN32
	m_szTempFileName << "c:";
	#endif
	if (*l_szTempFileName != *(m_pcFNameSrvc->m_FileDlmtr))
		m_szTempFileName << m_pcFNameSrvc->m_FileDlmtr;
	m_szTempFileName << l_szTempFileName;


	return m_szTempFileName.getPointer();
	*/
	return m_pcFNameSrvc->CreateTempFileName(a_szDir);
} /* end setTempFileName */
