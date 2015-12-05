//---------------------------------------------------------------------+
//  File:     ExtServices.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CExtServices class.
//
//  History: 
//  11/09/00 DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __EXTSRVCS_H__
#define __EXTSRVCS_H__

#include "Service.h"
#include "Any2.h"
#include "PutFilesSrvc.h"
#include "GetFilesSrvc.h"
#include "BlockingSrvc.h"
#include "MonitoringSrvc.h"
//#include "EncodingSrvc.h"
//#include "EncryptionSrvc.h"
#include "ExtSrvcsParamFile.h"
#include "CString.h"
#include <stdio.h>


class CExtServices : public CService 
{
protected: 
	CExtSrvcsParamFile*	   m_pcParms;
    CString                m_cStatus;
	CFileModificationSrvc* m_pcFileMod;
	CMonitoringSrvc*	   m_pcService;
		
	virtual long  CheckParms();

public:
	              CExtServices();
	virtual       ~CExtServices();
	virtual long  Execute();
	virtual char* GetStatus();
	virtual void  GetTypeStatus(int* a_iTypeCnt, char* a_szDlmtr);

	virtual long  initSelf(CExtSrvcsParamFile* v_pcParms, CLog* v_pcLog);
	virtual long  termSelf();
};

#endif 
