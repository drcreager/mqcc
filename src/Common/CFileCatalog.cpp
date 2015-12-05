//---------------------------------------------------------------------+
//  File:     CFileCatalog.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation of the CFileCatalog class, which 
//            encapsulates the knowledge of how to locate the 
//            files located on a local file system. 
//
//  Dev Log: 
//       01)  The mask provided on the DIR method is applied equally to
//            both Sub-Directory and File names. This means that a 
//            search for "C:\\*c*" will find not only file that contain
//            the letter c but will also search only sub-directory names 
//            that contain the letter c as well.    D. Creager                                    
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "CFileCatalog.h"

//---------------------------------------------------------------------+
//  Method:   CFileCatalog (default Constructor)
//  Desc:     Initialize the members of this class.. 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CFileCatalog::CFileCatalog(){

memset(m_FileName,'\0',128);
memset(m_Path,'\0',256);

#ifdef _WIN32
m_file.attrib = 0;
memset(m_file.name,'\0',260);
m_file.size = 0;
m_file.time_access= 0;
m_file.time_create= 0;
m_file.time_write= 0;
m_hFile = 0;
strcpy(m_Dlmtr,"\\");

#else
m_pcDir = NULL;
m_pcDirEntry = NULL;
strcpy(m_Dlmtr,"/");
#endif

} /* End CFileCatalog */





//---------------------------------------------------------------------+
//  Method:   ~CFileCatalog (default Destructor)
//  Desc:     currently unused. 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CFileCatalog::~CFileCatalog(){
#ifndef _WIN32
//
//  Deallocate the resources allocated by opendir()
//
if (m_pcDir != NULL){
	closedir(m_pcDir);
	m_pcDir = NULL;
} /* end if */
#endif
} /* End ~CFileCatalog */




//---------------------------------------------------------------------+
//  Method:   Dir
//  Desc:     Retrieves an initial file/directory based upon a 
//            search mask which is retained for future use.
//  Args:     char* ( Search Argument )
//  Returns:  char* ( File / Directory Name )
//            NULL = No Files Found
//---------------------------------------------------------------------+
char* CFileCatalog::Dir(char* a_szFileMask){
setPath(a_szFileMask);
setMask(a_szFileMask);
#ifdef _WIN32
	m_hFile =_findfirst( a_szFileMask, &m_file );

	if (m_hFile == -1L) return "";
	else if (!isValidName(&m_file.name[0]))
	         return this->Dir();
	else     return &m_file.name[0];
#else
	//
	//  open the specified directory defaulting to the current dir
	//
	m_pcDir = opendir(strlen(m_Path) > 0 ? m_Path : "./");
	if (m_pcDir == NULL){
		//
		//  An opendir() error has occured.  Either the directory the 
		//  directory named canot be accessed or there is not enough memory  
		//  to hold the entire stream.
		//
		return NULL;
	}
	else {
		//
		//  opendir() has successfully constructed a list, so let's 
		//  return the first entry from it. 
		//
		return Dir();
	} /* end if */
#endif
} /* End Dir */





//---------------------------------------------------------------------+
//  Method:   Dir
//  Desc:     Retrieves subsequent file/directories based upon a 
//            previously established search mask.
//  Args:     None.
//  Returns:  char* ( File / Directory Name)
//            NULL = End of File List
//---------------------------------------------------------------------+
char* CFileCatalog::Dir(){
#ifdef _WIN32
	int l_iRC = _findnext(m_hFile, &m_file);
	if (l_iRC != 0) {
		 _findclose(m_hFile);
		 return "";
	}
	else if (!isValidName(&m_file.name[0]))
		 return this->Dir();
	else {
		 strcpy(m_FileName,m_file.name);
		 return &m_file.name[0];
	} /* end If */
#else
        //
	//  obtain the file name
	//
	m_pcDirEntry = readdir(m_pcDir);
        //
	//  validate the file name
	//
        if (m_pcDirEntry == NULL) {     
	        closedir(m_pcDir);
	        m_pcDir = NULL;
		return NULL;
        }
	//	
	//   filter out filename of . and/or ..
	//
	else if (!isValidName(m_pcDirEntry->d_name))
		 return this->Dir();

	//	
	//   does name match the established pattern?
	//
	else if (fnmatch(m_Mask, m_pcDirEntry->d_name, 0) != 0)  
		 return this->Dir();

	//	
	//   Looks like a specified file, use it
	//
	else {                   
                 //  capture the filename
		 strcpy(m_FileName,m_pcDirEntry->d_name);

                 //  retrieve the file information
                 stat(getFullName(), &m_file);

                 //  return to caller
		 return m_pcDirEntry->d_name;
	} /* end If */
#endif
} /* End Dir */



//---------------------------------------------------------------------+
//  Method:   getXXXXX
//  Desc:     Retrieves various attributes of this class.
//  Args:     None.
//  Returns:  char* (Method specific Class Attribute)
//            NULL = End of File List
//---------------------------------------------------------------------+
char*  CFileCatalog::getDelimiter(void){
return m_Dlmtr;
} /* End getDelimiter */




//---------------------------------------------------------------------+
//  Method:   getFullName
//---------------------------------------------------------------------+
char*  CFileCatalog::getFullName(void){

memset(m_Work,'\0',260);           // clear out previous contents
if (strlen(m_Path) > 0) {          // on path available, 
	strcpy(m_Work, m_Path);    //    load the path 
	strcat(m_Work, m_Dlmtr );  //    and a terminating delimiter
} /* end if */

strcat(m_Work, m_FileName);
return m_Work;
} /* End getFullName */





//---------------------------------------------------------------------+
//  Method:   getLastDlm
//---------------------------------------------------------------------+
char* CFileCatalog::getLastDlm(char* a_szArg){
char* l_pos  = NULL;
char* l_last = NULL;

//
// Determine the length of the path
//
if (isValidPointer(a_szArg)) {
	l_pos = strchr(a_szArg, m_Dlmtr[0]);
	while (l_pos != NULL){
		l_last = l_pos;
		l_pos = strchr(l_pos+1, m_Dlmtr[0]);
	} /* End While */
} /* End If  */
return l_last;
} /* End getLastDlm */




//---------------------------------------------------------------------+
//  Method:   getMask
//---------------------------------------------------------------------+
char*  CFileCatalog::getMask(void){
return &m_Mask[0];
} /* End getMask */





//---------------------------------------------------------------------+
//  Method:   getPath
//---------------------------------------------------------------------+
char*  CFileCatalog::getPath(void){
return &m_Path[0];
} /* End getPath */





//---------------------------------------------------------------------+
//  Method:   isXXXXXXXX
//  Desc:     Exposes various attributes of the current file system obj.
//  Args:     None.
//  Returns:  BOOL (True = Attribute is present)
//---------------------------------------------------------------------+

//---------------------------------------------------------------------+
//  Method:   isNormal
//---------------------------------------------------------------------+ 
BOOL CFileCatalog::isNormal(){
#ifdef _WIN32
	return (m_file.attrib == _A_NORMAL); 
#else
	return (m_file.st_mode & _S_IFDIR);
#endif    
} /* End isNormal */

//---------------------------------------------------------------------+
//  Method:   isReadOnly
//---------------------------------------------------------------------+
BOOL CFileCatalog::isReadOnly(){
#ifdef _WIN32
	return (m_file.attrib & _A_RDONLY);
#else
	return false;
#endif
} /* End isReadOnly */

//---------------------------------------------------------------------+
//  Method:   isHidden
//---------------------------------------------------------------------+
BOOL CFileCatalog::isHidden(){
#ifdef _WIN32
	return (m_file.attrib & _A_HIDDEN);
#else
	return false;
#endif
} /* End isHidden */

//---------------------------------------------------------------------+
//  Method:   isSystem
//---------------------------------------------------------------------+
BOOL CFileCatalog::isSystem(){
#ifdef _WIN32
	return (m_file.attrib & _A_SYSTEM);
#else
	return false;
#endif
} /* End isSystem */

//---------------------------------------------------------------------+
//  Method:   isSubDirectory
//---------------------------------------------------------------------+
BOOL CFileCatalog::isSubDirectory(){
#ifdef _WIN32
	return (m_file.attrib & _A_SUBDIR);
#else
	return S_ISDIR(m_file.st_mode);
#endif
} /* End isSubDirectory */

//---------------------------------------------------------------------+
//  Method:   isArchive
//---------------------------------------------------------------------+
BOOL CFileCatalog::isArchive(){
#ifdef _WIN32
	return (m_file.attrib & _A_ARCH); 
#else
	return false;
#endif
} /* End isArchive */

//---------------------------------------------------------------------+
//  Method:   isValidName
//---------------------------------------------------------------------+
BOOL CFileCatalog::isValidName(char* a_szArg){
return !((*(a_szArg+0) == '.') 
	 && ((*(a_szArg+1) == '\0') || (*(a_szArg+1) == '.')));
}





//---------------------------------------------------------------------+
//  Method:   setXXXXX
//  Desc:     Establishes various class attribute values.
//  Args:     char* ( attribute value)
//  Returns:  Nothing.
//---------------------------------------------------------------------+


//---------------------------------------------------------------------+
//  Method:   setMask
//---------------------------------------------------------------------+
void CFileCatalog::setMask(char* a_szArg){

memset(m_Mask,'\0',128);
char* l_last = getLastDlm(a_szArg);
if (l_last != NULL)
	if (strlen(l_last+1) > 0)  strcpy(m_Mask, l_last+1);
	else                       strcpy(m_Mask, "*");
else               
	if (isValidPointer(a_szArg)) strcpy(m_Mask, a_szArg);
} /* End setMask */




//---------------------------------------------------------------------+
//  Method:   setPath
//---------------------------------------------------------------------+
void CFileCatalog::setPath(char* a_szArg){

memset(m_Path,'\0',256);
char* l_last = getLastDlm(a_szArg);
if ((l_last != NULL) && ((l_last - a_szArg) > 0))
	memcpy(m_Path, a_szArg, (l_last - a_szArg));
} /* End setPath */
