//---------------------------------------------------------------------+
//  File:     komqutl.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Application Stub.   Front end for the UTL program.
//
//            Sets App Mode to APP_MODE_UTILITY.
//            Sets Application Name, Description, and Version.
//  
//  Dev Log: 
//  History: 
//  11/09/00 DRC Created original version.
//  03/25/02 DRC Corrected generating DataFileName when Blocking is off.
//---------------------------------------------------------------------+
#include "CmdInterface.h"

int main(int argc, const char *argv[]) {

	CCmdInterface	cCmdInterface;
	long			ll_return	= KOCC_FAIL;	      // assume bad
	int				li_appmode	= APP_MODE_EXT_SRVCS; // Extended Services Application

	CString			ls_appname(50);
	CString			ls_appdesc(80);
	CString			ls_appversion("1.0.00.01");


	switch (li_appmode) {

	case APP_MODE_EXT_SRVCS:
		ls_appname = "KOMQESU";
		ls_appdesc = "KO MQ Series Extended Services Utility";
		break;

	default:
		ls_appname = "KOMQBATCH";
		ls_appdesc = "KO MQ Series Batch Utility";
		break;
	}


	cCmdInterface.initSelf(ls_appname, ls_appdesc, ls_appversion, 
							li_appmode);

	ll_return = cCmdInterface.execESU(argc, argv);

	cCmdInterface.termSelf();

	return (int) ll_return;
}


