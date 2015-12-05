//---------------------------------------------------------------------+
//  File:     CFileList.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CFileList class, which is a collection
//            of file objects from the local file system that meet the 
//            search criterial provided upon it instantiation.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "CCollection.h"
#include "CFileCatalog.h"
#include "CFile.h"
#include "CItem.h"
#include <string.h>

class CFileList : public CCollection  {
protected:
CFile*			m_pFile;
CItem*			m_pFileElement;
char			m_Work[260];

public:
    virtual void add(	char*	a_szFileName);
	        CFileList(	char*	a_szFileMask, 
						BOOL	a_bSubDirSrch);
	virtual ~CFileList();
    virtual char* getNextDirName(
						char*	a_szFullName,
                                                char*   a_szDelimiter,
						char*	a_szFileMask);
	virtual void load(	char*	a_szFileMask, 
						BOOL	a_bSubDirSrch);
	virtual void unload(void);

};

