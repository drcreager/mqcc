//---------------------------------------------------------------------+
//  File:     MonitoringSrvc.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CMonitoringSrvc class.
//
//  History: 
//  11/10/00 DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifdef  _POSIX_
//#define _NO_POSIX_
//#undef  _POSIX_
//#endif

#include "Service.h"
#include "CFileList.h"
#include "CFile.h"
#include "CSysCmd.h"
#include "ExtSrvcsParamFile.h"
#include "FileNameSrvc.h"
#include "BlockingSrvc.h"
#include "CtlFile.h"
//#include <process.h>


class CMonitoringSrvc : public CService  {
protected:
CExtSrvcsParamFile*		m_pcParms;
CFileModificationSrvc*  m_pcFMSrvc;
CFileNameSrvc*          m_pcFNameSrvc;
CFileList*				m_pcFileList;
CFile*					m_pcFile;
CCtlFile*               m_pcCtlFile;
CString*                m_szWork;
CSysCmd*                m_pcSysCmd;

public:

    virtual char* ChangeFileExt(  CString* a_szFileName,
								  char*    a_szExtension);
    virtual char* ChangeFileExt(  char*    a_szFileName,
								  char*    a_szExtension);
	virtual long  CheckParms(     void);
	virtual long  count();
				  CMonitoringSrvc();
				  CMonitoringSrvc(CExtSrvcsParamFile* a_pcParms, 
					              CLog*               a_pcLog);
	virtual       ~CMonitoringSrvc();
    virtual long  Execute();
	virtual char* GetStatus();
};

//#ifdef  _NO_POSIX_
//#define _POSIX_
//#undef  _NO_POSIX_
//#endif