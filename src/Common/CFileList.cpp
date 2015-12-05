//---------------------------------------------------------------------+
//  File:     CFileList.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation of the CFileList class, which is a collection
//            of file objects from the local file system that meet the 
//            search criterial provided upon it instantiation.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "CFileList.h"

//---------------------------------------------------------------------+
//  Method:   add
//  Desc:     Add a file object into this collection. 
//  Args:     char* (Name of the file to associate with the file object)
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CFileList::add(char* a_szFileName){
//
//  Instantiate a File Object 
//  Store file name in the CFile object

//  Instantiate a Item 
//  Set pointer to the file in the list element (CItem)
//  Add list element to the CFileList
//
m_pFile        = new CFile();
m_pFile->initSelf(a_szFileName);
m_pFileElement = new CItem(m_pFile);
CCollection::add(m_pFileElement);
} /* End add */




//---------------------------------------------------------------------+
//  Method:   CFileList (default Constructor)
//  Desc:     Load the contents of this collection based upon the 
//            specifications provide on invocation. 
//  Args:     char* (a file search argument valid to the host file system)
//            BOOL  (Indicator about searching sub-directories)
//                   True  = Open and search all sub-directories
//                   False = Ignore all sub-directories.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CFileList::CFileList(char* a_szFileMask, BOOL a_bSubDirSrch){
load(a_szFileMask, a_bSubDirSrch);
}





//---------------------------------------------------------------------+
//  Method:   ~CFileList (destructor)
//  Desc:     Unload the contents of this collection releasing all 
//            of its resources. 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CFileList::~CFileList(){
unload();
}




//---------------------------------------------------------------------+
//  Method:   getNextDirName 
//  Desc:     Format a search argument for the next lower level of 
//            directory.
//  Args:     char* ( Fully qualified name of the next lower directory)
//            char* ( File delimiter string )
//            char* ( Currently specified search argument)
//  Returns:  char* ( Fully qualified search argument)
//---------------------------------------------------------------------+
char* CFileList::getNextDirName(char* a_szFullName,
                                char* a_szDelimiter,
				char* a_szFileMask){
strcpy(m_Work, a_szFullName);
strcat(m_Work, a_szDelimiter);
strcat(m_Work, a_szFileMask);
return &m_Work[0];
} /* End getNextDirName */




//---------------------------------------------------------------------+
//  Method:   load 
//  Desc:     Load the file collection with the sub-set of file objects
//            retrieved by the user specified search specifications.
//  Args:     char* (a file search argument valid to the host file system)
//            BOOL  (Indicator about searching sub-directories)
//                   True  = Open and search all sub-directories
//                   False = Ignore all sub-directories.
//  Returns:  Nothing.
//
//  Tech Note: This method is entered recursively.  Due to the use of 
//            this technique, this method MUST have a local instance 
//            of the CFileCatalog object to function correctly.  DRC
//---------------------------------------------------------------------+
void CFileList::load(char* a_szMask, BOOL a_bSubDirSrch){
CFileCatalog	l_FileCtlg;
char*			l_szFileName;

l_szFileName = l_FileCtlg.Dir(a_szMask);
while (strlen(l_szFileName) > 0){
	//
	//  Parse sub-directories when requested
	//
	if (l_FileCtlg.isSubDirectory()){
		if (a_bSubDirSrch){
			this->load(getNextDirName(l_FileCtlg.getFullName(),
                                                  l_FileCtlg.getDelimiter(), 
                                                  l_FileCtlg.getMask()), 
			           a_bSubDirSrch);
		} /* End If */
	} /* End If */
	else {
		this->add(l_FileCtlg.getFullName());
	} /* End If */
	l_szFileName = l_FileCtlg.Dir();
} /* End While */
} /* End load */



//---------------------------------------------------------------------+
//  Method:   unload 
//  Desc:     Unload the file collection releasing all file objects 
//            and items currently found in the collection.
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CFileList::unload(void){
//
//  Release all associated file objects.
//
m_pFile = (CFile*) top();
while (!endOfList()){
	delete[] m_pFile;
	m_pFile = (CFile*) next();
} /* End while */
//
//  Release all items in this collection.
//
this->clear();
} /* End unload */


