//---------------------------------------------------------------------+
//  File:     komqput.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Application Stub.   Front end for the PUT program.
//
//            Sets App Mode to APP_MODE_PUT.
//            Sets Application Name, Description, and Version.
//  
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+

#include "CmdInterface.h"

int main(int argc, const char *argv[]) {

	CCmdInterface	cCmdInterface;
	long			ll_return	= KOCC_FAIL;	// assume bad

	int				li_appmode	= APP_MODE_PUT;	// Put Application

	CString			ls_appname(50);
	CString			ls_appdesc(80);
#ifdef _ver2_
	CString			ls_appversion("2.0.02.03");
#else
	CString			ls_appversion("2.0.02.01");
#endif


	switch (li_appmode) {

	case APP_MODE_PUT:
		ls_appname = "KOMQPUT";
		ls_appdesc = "KO MQ Series Put Utility";
		break;

	case APP_MODE_GET:
		ls_appname = "KOMQGET";
		ls_appdesc = "KO MQ Series Get Utility";
		break;

	case APP_MODE_DEAD_LETTER:
		ls_appname = "KOMQDLQ";
		ls_appdesc = "KO MQ Series Dead Letter Processing Utility";
		break;

	default:
		ls_appname = "KOMQBATCH";
		ls_appdesc = "KO MQ Series Batch Utility";
		break;
	}


	cCmdInterface.initSelf(ls_appname, ls_appdesc, ls_appversion, 
							li_appmode);


	ll_return = cCmdInterface.execKOMQ(argc, argv);


	cCmdInterface.termSelf();

	return (int) ll_return;
}


