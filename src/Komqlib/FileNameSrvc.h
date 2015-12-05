//---------------------------------------------------------------------+
//  File:     FileNameSrvc.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the declaration of the CFileNameSrvc 
//            class.  This class encapsulates behaviors relating to the 
//            creation and manipulation of file names.
//
//  History: 
//  11/10/00 DRC Created original version. 
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "CAny.h"
#include "ExtSrvcsParamFile.h"
#include "CString.h"


class CFileNameSrvc : public CAny  
{
public:
char     m_FileDlmtr[2];

protected:
	CExtSrvcsParamFile*	m_pcParms;
	CString				m_szWork;
	CString             m_szTempFileName;


public:
	        CFileNameSrvc(CExtSrvcsParamFile* a_pcParms);
	virtual ~CFileNameSrvc();
	virtual char* CreateTempFileName(char*  a_szDir);
	virtual char* getArchiveName(CString*   a_szFullFileName);
	virtual char* getArchivePath(CString*   a_szFullFileName,
								 CString*   a_szSrcFileName);
	virtual long  getDlmtrPos(   CString*   a_szFullFileName);
	virtual char* getFileName(   CString*   a_szFullFileName);
	virtual char* getFileName(   char*      a_szFullFileName);
	virtual char* getFilePath(   CString*   a_szFullFileName);
	virtual char* getFilePath(   char*      a_szFullFileName);
	virtual char* getLocalFileName(CString* a_szArchiveName);
	virtual char* getTempFileName();
	virtual void  setTempFileName( char*    a_szTempFileName);
	virtual void  setTempFileName( char*    a_szDir, 
		                           char*    a_szTempFileName);
	virtual CString* getTimeStamp();
};
