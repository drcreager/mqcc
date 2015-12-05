//---------------------------------------------------------------------+
//  File:     PutFilesSrvc.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CPutFilesSrvc class.
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

class CPutFilesSrvc : public CMonitoringSrvc  
{
public:
	CPutFilesSrvc(CExtSrvcsParamFile* a_pcParms, 
				  CLog*               a_pcLog);

	virtual ~CPutFilesSrvc();
	virtual long  CheckParms(void);
	virtual long  Execute();

};

