//---------------------------------------------------------------------+
//  File:     CFileCatalog.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CFileCatalog class, which 
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
//  02/14/01  DRC Added Unix code using scandir() rather than _findfile()
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifdef _WIN32
	#ifdef _POSIX_
	#define _NO_POSIX_
	#undef _POSIX_
	#endif
#endif

#include "CAny.h"
#include <time.h>
#include <string.h>
#include <memory.h>
typedef int BOOL;

#ifdef _WIN32
	#include <io.h>
#else
	#include <dirent.h>
        #include <stdlib.h>
        #include <sys/stat.h>    
	#include <fnmatch.h>   
#endif

class CFileCatalog : public CAny {
protected: 
char m_Dlmtr[2];
char m_Mask[128];
char m_FileName[128];
char m_Path[256];
char m_Work[260];

#ifdef _WIN32
_finddata_t m_file;
long m_hFile;

#else
DIR*    m_pcDir;
struct dirent* m_pcDirEntry;
struct stat    m_file;
#endif




public:
CFileCatalog();
virtual ~CFileCatalog();
virtual char* Dir(char* a_szFileMask);
virtual char* Dir();
virtual char* getDelimiter();
virtual char* getLastDlm(char* a_szArg);
virtual char* getMask(void);
virtual char* getPath(void);
virtual char* getFullName(void);
        BOOL isNormal();
        BOOL isReadOnly();
        BOOL isHidden();
        BOOL isSystem();
        BOOL isSubDirectory();
        BOOL isArchive();
		BOOL isValidName(char* a_szArg);
virtual void setMask(char* a_szArg);
virtual void setPath(char* a_szArg);
};

#ifdef _WIN32
	#ifdef _NO_POSIX_
	#define _POSIX_
	#undef _NO_POSIX_
	#endif
#endif
