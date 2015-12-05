//---------------------------------------------------------------------+
//  File:     GetFilesSrvc.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CGetFilesSrvc class.
//
//  History: 
//  11/10/00 DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "MonitoringSrvc.h"
#include "BatchList.h"
#include "DataHeader.h"
#include "FileNameSrvc.h"

class CGetFilesSrvc : public CMonitoringSrvc  {
protected: 
CBatchList*		m_pcBatchList;
CFileNameSrvc*	m_pcFNameSrvc;
CString*		m_pcBatch;
BOOL			m_bFileSetComplete;

public:
        CGetFilesSrvc(CExtSrvcsParamFile* a_pcParms, 
					  CLog*               a_pcLog);
	virtual ~CGetFilesSrvc();
	virtual long  CheckParms(void);
	virtual long  Execute();
    virtual BOOL  isFileSetComplete();

protected:
	virtual void  RenameFileSet(CString*     l_szFileName,
						        CString*     l_szNewFileName);
	virtual void  RenameFile(   CString*     l_pcOldFile, 
						        CString*     l_pcNewFile,
						        char*        l_szExtension);
};
