//---------------------------------------------------------------------+
//  File:     CmdInterface.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CCmdInterface class.
//
//  History: 
//  03/12/99 DMT Created original code.
//  11/09/00 DRC Added APP_MODE_UTILITY constant and ExtServices class.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CMDINTERFACE_H__
#define __CMDINTERFACE_H__

#include "Any2.h"
#include "Log.h"
#include "CmdLine.h"
#include <stdio.h>
#include <limits.h>
#include "StartupParamFile.h"
#include "ExtSrvcsParamFile.h"
#include "GetSingle.h"
#include "GetBatch.h"
#include "PutSingle.h"
#include "PutBatch.h"
#include "DeadSingle.h"
#include "DeadReport.h"
#include "RespBatch.h"
#include "ExtServices.h"
#include "CSysCmd.h"


//---------------------------------------------------------------------+
// Define User-Selectable Modes
// These are "selectable via the "Mode" parameter
// in the parameter file.  Only one of these
// is allowed.
//---------------------------------------------------------------------+
#define USER_MODE_INVALID		0
#define	USER_MODE_BATCH			1
#define	USER_MODE_SINGLE_QUEUE	2
#define	USER_MODE_SINGLE_MSG	4	
#define	USER_MODE_FUTURE		8
//
// Modes below are for Dead Letter
#define USER_MODE_DLQ           1
#define USER_MODE_DLQ_REPORT	2


//---------------------------------------------------------------------+
// Define Application Specific Modes
// These are application-set modes and must be set
// by the calling application shell during initSelf().
// Only 1 of these modes is allowed at 1 time.
// The user mode must be combined with the Application Mode
// at runtime to determine what the application will do.
//---------------------------------------------------------------------+
#define APP_MODE_INVALID		0
#define	APP_MODE_PUT			16
#define	APP_MODE_GET			32
#define APP_MODE_BROWSE			64
#define	APP_MODE_DEAD_LETTER	128	
#define APP_MODE_EXT_SRVCS      256




//---------------------------------------------------------------------+
//  Class:    CCmdInterface
//---------------------------------------------------------------------+
class CCmdInterface : public CAny2 {

private:
	CString	is_appname;
	CString	is_appdesc;
	CString	is_appversion;
	int		ii_appmode;
	int		ii_usermode;


public:

//	virtual void Banner(ostream& s);
	             CCmdInterface();
	virtual     ~CCmdInterface();

	        long execKOMQ( int     argc, 
				   const  char*    argv[]);
	        long execESU( int      argc, 
				   const  char*    argv[]);
	virtual int	 initSelf(CString& rs_appname,
						  CString& rs_appdesc,
						  CString& rs_appversion,
						  int	   ii_appmode);
};
#endif