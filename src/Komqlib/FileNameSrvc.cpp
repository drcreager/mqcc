//---------------------------------------------------------------------+
//  File:     FileNameSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the CFileNameSrvc 
//            class.  This class encapsulates behaviors relating to the 
//            creation and manipulation of file names.
//
//  History: 
//  11/10/00 DRC Created original version. 
//---------------------------------------------------------------------+
#include "FileNameSrvc.h"








//----------------------------------------------------------------------
// Function: CFileNameSrvc (Default Constructor)
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will initialize this object.
// Args:     None.
// Returns:  Nothing.
// History: 
// 09/19/00 DRC Created original version.
// 11/10/00 DRC Add OS specific initialization of m_FileDlmtr 
//----------------------------------------------------------------------
CFileNameSrvc::CFileNameSrvc(CExtSrvcsParamFile* a_pcParms){
if (isValidPointer(a_pcParms))  m_pcParms = a_pcParms;                   

#ifdef _WIN32 
strcpy(m_FileDlmtr,"\\");
#else
strcpy(m_FileDlmtr,"/");
#endif
}  /* end CFileNameSrvc */





//----------------------------------------------------------------------
// Function: ~CFileNameSrvc (Default Destructor)
// Written:  September 2000
// Author:   Daniel R Creager
// Desc:     This function will release this object's resources.
// Args:     None.
// Returns:  Nothing.
// History: 
// 09/19/00 DRC Created original version. 
//----------------------------------------------------------------------
CFileNameSrvc::~CFileNameSrvc(){
} /* End ~CFileNameSrvc */




//----------------------------------------------------------------------
// Function: CreateTempFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Create a Temporary File Name.
// Args:     char*      Directory in which to create file name.
//
// Returns:  char*      Address of the TempFileName string.
//
// Dev Notes: 
// 01)  The generate temporary file name function uses a numeric file 
//      extension and increments it for each call. (e.. J1Kg.1, J1Kg.2, etc)
//      Since we replace the extension this can call duplicated file names. 
//      To remove problem we will replace the original extension with a 
//      psuedo extension to avoid duplication. 
//
// History: 
// 11/15/00 DRC Created original version.
// 04/15/02 DRC Modified to eliminate duplicates across multiple processes. 
//----------------------------------------------------------------------
char* CFileNameSrvc::CreateTempFileName(char* a_szDir){
char*   l_WorkFileName = NULL;
CString l_TempFileName;

//
//  Create a file name using OS System services
//
l_WorkFileName = tmpnam(NULL);
 
//
//  OnSuccess,  modify extension Ref: See Dev Note #01 above
//  OnFailure,  substitute a default 
//
if(l_WorkFileName != NULL){
	l_TempFileName += l_WorkFileName;
	l_TempFileName.replace(".","_");
	l_TempFileName += ".ext";
} 
else {
	//
	//  Prefix the temp name with MMSS for uniqueness across processes.
	//
	l_TempFileName =  "\\\0";
	l_TempFileName += getTimeStamp()->substr(10,4); //Use Only "MMSS"
	l_TempFileName += "tmp1.dat";
}  // end if


//
//  Persist the created file name
//
setTempFileName(a_szDir, l_TempFileName.getPointer());
return getTempFileName();
} /* end CreateTempFileName */


//----------------------------------------------------------------------
// Function: getArchiveName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will remove the prefix of a fully qualified 
//           file name defined by the SourceFiles parameter and return
//           the result.
// 
//           SourceFiles=c:\aaa\bbb\*.*
//           c:\aaa\bbb\ccc\FileName.xxx -> ccc\FileName.xxx
//
// Args:     CString* (Fully Qualified File Name)
// Returns:  char*(Simple File Name) 
// History: 
// 11/10/00 DRC Created original version.
// 03/07/01 DRC Modified to avoid prefixing the name with a delimiter. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getArchiveName(CString* a_szFullFileName){

int l_iPrefixLen = strlen(getFilePath(m_pcParms->GetSrcFileMask()));

//
//  On non-zero PrefixLen, remove the prefix and its delimiter
//
if (l_iPrefixLen > 0) return a_szFullFileName->mid(l_iPrefixLen + 2);
else                  return a_szFullFileName->c_str();
}  /* End getArchiveName */



//----------------------------------------------------------------------
// Function: getArchivePath
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will the archival path for a fully qualified 
//           file name.
// 
//           SourceFiles=c:\aaa\*.*
//           FileArchive=c:\xxx
//
//           c:\aaa\bbb\FileName.xxx -> c:\xxx\bbb
//
// Args:     CString* (Fully Qualified File Name)
// Returns:  char*(Archival Path Name) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getArchivePath(CString* a_szFullFileName,
								    CString* a_szSrcFileName){
m_szWork.fill();
m_szWork << getFilePath(a_szSrcFileName) << "\\"
         << getArchiveName(a_szFullFileName);


return m_szWork.getPointer();
}  /* End getArchivePath */



//----------------------------------------------------------------------
// Function: getDlmtrPos
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will locate the delimiter that separates a
//           a fully qualified file name's path from its file name and
//           return its position in the string to the caller.
//                         * 
//           c:\aaa\bbb\ccc\FileName.xxx -> 15
//
// Args:     CString* (Fully Qualified File Name)
// Returns:  long(position of the separating delimiter)
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
long CFileNameSrvc::getDlmtrPos(CString* a_szFullFileName){
long l_Pos = 1;
long l_Prev = 0;

l_Pos = a_szFullFileName->pos(l_Pos, m_FileDlmtr);
while (l_Pos > 0){
	l_Prev = l_Pos++;
	l_Pos = a_szFullFileName->pos(l_Pos, m_FileDlmtr);
} /* End While */
return l_Prev;
}  /* End getDlmtrPos */





//----------------------------------------------------------------------
// Function: getFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will extract a simple file name from 
//           a fully qualified file name. 
//           c:\aaa\bbb\ccc\FileName.xxx -> FileName.xxx
// Args:     CString* (Fully Qualified File Name)
// Returns:  char*(Simple File Name) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getFileName(CString* a_szFullFileName){
return a_szFullFileName->mid(getDlmtrPos(a_szFullFileName) + 1);
}  /* End getFileName */




//----------------------------------------------------------------------
// Function: getFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will extract a simple file name from 
//           a fully qualified file name. 
//           c:\aaa\bbb\ccc\FileName.xxx -> FileName.xxx
// Args:     char* (Fully Qualified File Name)
// Returns:  char*(Simple File Name) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getFileName(char* a_szFullFileName){
m_szWork = a_szFullFileName;
return getFileName(&m_szWork);
}  /* End getFileName */




//----------------------------------------------------------------------
// Function: getFilePath
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will extract a path name from 
//           a fully qualified file name. 
//           c:\aaa\bbb\ccc\FileName.xxx -> c:\aaa\bbb\ccc
// Args:     CString* (Fully Qualified File Name)
// Returns:  char*(File Path) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getFilePath(CString* a_szFullFileName){

return a_szFullFileName->mid(1, getDlmtrPos(a_szFullFileName) - 1);
}  /* End getFilePath */




//----------------------------------------------------------------------
// Function: getFilePath
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will extract a path name from 
//           a fully qualified file name. 
//           c:\aaa\bbb\ccc\FileName.xxx -> c:\aaa\bbb\ccc
// Args:     CString* (Fully Qualified File Name)
// Returns:  char*(File Path) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getFilePath(char* a_szFullFileName){
m_szWork = a_szFullFileName;

return getFilePath(&m_szWork);
}  /* End getFilePath */





//----------------------------------------------------------------------
// Function: getLocalFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     This function will prefix the destination directory with 
//           the archival name to produce a localized, fully qualified, 
//           file name. 
// 
//           DestDir=j:\xxx\yyy
//           a_szFileName=ccc\FileName.xxx
//           Result= j:\xxx\yyy\ccc\FileName.xxx
//
// Args:     CString* (Archival File Name)
// Returns:  char*(Fully qualified local File Name) 
// History: 
// 11/10/00 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getLocalFileName(CString* a_szArchiveName){

//
//  Load the locally specified file path
//
m_szWork = m_pcParms->GetDstDir();

//
//  When needed, insert a file delimiter
//
if (a_szArchiveName[0] != m_FileDlmtr){
	m_szWork << m_FileDlmtr; 
} /* End If */

//
//  Finally, append the archival file name
//
m_szWork << a_szArchiveName->c_str(); 

//
//  return the results to the caller
//
return m_szWork.c_str();
}  /* End getLocalFileName */





//----------------------------------------------------------------------
// Function: getTempFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Get the a Temporary File Name attribute.
// Args:     None.
// Returns:  char*      Address of the TempFileName string.
// History: 
// 03/06/01 DRC Created original version. 
//----------------------------------------------------------------------
char* CFileNameSrvc::getTempFileName(){
return m_szTempFileName.c_str();
} /* end getTempFileName */





//----------------------------------------------------------------------
// Function: setTempFileName
// Written:  November 2000
// Author:   Daniel R Creager
// Desc:     Set the a Temporary File Name attribute.
// Args:     char*      Address of a TempFileName string.
// Returns:  Nothing
// History: 
// 03/06/01 DRC Created original version.
//----------------------------------------------------------------------
void CFileNameSrvc::setTempFileName(char* a_szTempFileName){
//
//  Clear any pre-existing temporary names and 
//  if no value was specified, then set C:\ as a default.
//
m_szTempFileName.fill();
#ifdef _WIN32
if (a_szTempFileName == NULL) m_szTempFileName << "c:" << m_FileDlmtr;
#endif
m_szTempFileName << a_szTempFileName;
} /* end setTempFileName */





void CFileNameSrvc::setTempFileName(char* a_szDir, char* a_szTempFileName){
CString l_szTempFileName;
//
// Concatenate Dir with Temp file name
//
if (a_szDir != NULL) l_szTempFileName = a_szDir;
l_szTempFileName << m_FileDlmtr << getFileName(a_szTempFileName);
//
//  Persist the combined value
//
setTempFileName(l_szTempFileName.c_str());
} /* end setTempFileName */

//----------------------------------------------------------------------
// Function: getTimeStamp
// Written:  April 2002
// Author:   Daniel R Creager
// Desc:     Construct string from the current date and time.
// Args:     char*      Address of timestamp string ("YYYYMMDDHHMMSS").
// Returns:  Nothing
// History: 
// 03/06/01 DRC Created original version.
//----------------------------------------------------------------------
CString* CFileNameSrvc::getTimeStamp(){
time_t	lt_time;
tm*		lstr_time	= NULL;	

time(&lt_time);
lstr_time = localtime(&lt_time);
m_szWork = "              ";

if (lstr_time != NULL) 
	strftime(m_szWork.c_str(), 15, "%Y" "%m%d%H" "%M%S", lstr_time);

return &m_szWork;
} // end getTimeStamp
