//---------------------------------------------------------------------+
//  File:     Service.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CService class.  CService
//            provides a common ancestor for both Transfer and 
//            Non-Transfer services allowing them to share the 
//            fact that they all use a CLog and CParamFile object.
//
//  History: 
//  11/09/00 DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "Any2.h"
#include "ParamFile.h"
#include "Log.h"
#include "CString.h"


class CService : public CAny2  
{
protected: 
	CLog*			m_pcLog;
    CString         m_cStatus;

	virtual long  CheckParms(void);

public:
	              CService();
	virtual       ~CService();
	virtual long  Execute(void) = 0;
	virtual char* GetStatus() = NULL;
	virtual long  initSelf(CLog* v_pcLog);
	virtual long  termSelf();
};

#endif 
