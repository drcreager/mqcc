//---------------------------------------------------------------------+
//  File:     CmdInterface.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the 
//            CCmdInterface Class.
//
//            The calling "application shell" calls this class's
//            initSelf Method to set the Application Mode 
//            (ie Get or Put or DLQ), and Application Name.
//
//            The calling "application shell" calls this class's
//            execute method to do the work.
//              
//            This class's execute method gets Command Line Parameters 
//            via CCmdArgs, Parameter File Parms via CStartParamFile,
//            and opens a log file via CLog.
//
//            If the parameter "getting" is okay, the "execute"
//            method of the appropriate Algorithm class is called
//            to perform the requested "Transfer".
//
//            Finally, this class logs the run statistics and closes
//            the log file.
//
//  Dev Log: 
//  History: 
//  03/12/99 DMT Created original version.
//  02/12/00 DRC Moved the Banner method from CCmdInterface to CLog
//  11/09/00 DRC Added the ExtServices class and moved the 
//               "Report Status" logic to the CTransfer class so that it
//               could be customized in non-CTransfer descendants.
//---------------------------------------------------------------------+
#include "CmdInterface.h"






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     None
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/??/99  DMT Created original version.
//---------------------------------------------------------------------+
CCmdInterface::CCmdInterface() : CAny2() {

	is_appname		= "Application Name Not Specified";
	is_appdesc		= "Application Description Not Specified";
	is_appversion	= "0.0.00";

	ii_appmode		= APP_MODE_INVALID;
	ii_usermode		= USER_MODE_INVALID;

	is_InstanceName	= "Command Interface";

}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     None
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/??/99  DMT Created original version.
//---------------------------------------------------------------------+
CCmdInterface::~CCmdInterface() {
//	termSelf();
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   execKOMQ
//  Author:   David Tamkun
//  Written:  February 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  Need to define a "User Mode" for Dead Letter
//
//            Need to make sure command line parms update
//            parm file parms.
//
//            Banner Method really should go through log file, or
//            pass parms to log file and have it generate the 
//            "Banner".
//
//            Command Line and Parameter File classes probably should
//            get a pointer to the Log Class so they can display
//            their own errors and not rely on this class to Log them.
//
//  History: 
//  02/27/99 DMT Created original version.
//---------------------------------------------------------------------+
long CCmdInterface::execKOMQ(int argc, const char *argv[]) {

	int				rc				= KOCC_FAIL;
	long			ll_return		= KOCC_FAIL;
	long			ll_final_return	= KOCC_FAIL;
	BOOL			bLogOpen		= FALSE;
	CStartParamFile	   cParamFile;
	CLog			cLog;
	CRespBatch*     pcRespBatch;
	CString			ls_msg(132);
	CString         ls_parm(48);
	//
	//  Services Functions
	//
	CGetSingle 		cGetSingle;
	CGetBatch 		cGetBatch;
	CPutSingle 		cPutSingle;
	CPutBatch 		cPutBatch;
	CDeadSingle 	cDeadSingle;
	CDeadReport 	cDeadReport;
	CSysCmd         cSysCmds;

	//-----------------------------------------------------------------+
	// Display Version Information
	//-----------------------------------------------------------------+
	//Banner(cout);
	cLog.Banner(&is_appname, &is_appversion, &is_appdesc, LOGCONSOLE);


	//-----------------------------------------------------------------+
	// Get Parameters from Command Line
	//-----------------------------------------------------------------+
	CCmdArgs	cCmdLine(argc, argv);
	CopyErrors(&cCmdLine);
	cLog.LogErrors(cCmdLine, LOGCONSOLE);


	//-----------------------------------------------------------------+
	// If okay, Get Parameters from Parameter File
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {
		cParamFile.Load(cCmdLine);

		// If the command line said restart, set the member in the
		// parmfile class, so we only have to pass around the 
		// parmfile class.
		cParamFile.SetRestart(cCmdLine.IsRestart());

		CopyErrors(&cParamFile);
		cLog.LogErrors(cParamFile);
	}/* End if */


	//-----------------------------------------------------------------+
	// If okay, Validate Global Parms
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {

		ii_usermode = cParamFile.GetMode();

		long li_appmode = 0;

		li_appmode = cParamFile.GetAppMode();

		if (li_appmode != 0) {
			ii_appmode = li_appmode;
		}

		// Always			Log file name		do here
		// Always			Mode				do here
		
		// Always			Queue Manager Name	CTransfer
		// Always			Data Queue name		CTransfer
		// Always			Buffer size			CTransfer
		//
		// Get, Put			Restart file name	CGet
		// Get, Put			Data file name		CGet
		// Get, Put			Batch interval		CGet
		//
		// Get, Put Mode 1	Control Queue name	CGetBatch
		// Get, Put Mode 1  Control File name	CGetBatch
		//
		// Put				Message ID			CPut?
		//
		// DLQ				Dead Letter Queue Name	CDLQ
		//
		// Never			Error Queue name
		// Future			Correlation ID
		// Future			Password
//		cParamFile.Load(cCmdLine);
//		CopyErrors(&cParamFile);
	}
	
	
	//-----------------------------------------------------------------+
	// If okay, Open Log File
	//
	// If not okay, set ll_return and use ll_return to determine if
	// we continue.
	//
	// Note that here we switch from checking for Errors in the Errors
	// collection to using return codes.
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {

		ll_return = cLog.initSelf((char*) cParamFile.GetLogFileName());
	}
	else {
		ll_return = KOCC_FAIL;
	}  /* End If */



	//-----------------------------------------------------------------+
	// If okay, Instantiate and Open the Response cache
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ls_parm = (char*) cParamFile.GetQueMgrName();
		pcRespBatch = new CRespBatch(&cLog, &ls_parm);

		ls_parm =  (char*) cParamFile.GetReplyToQName();
		ll_return = pcRespBatch->open(&ls_parm);
	}  /* End If */



	//-----------------------------------------------------------------+
	// If okay, 
	// -  Pass the RespBatch object pointer to Log
	// -  Log App Version and Parameter Values
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		cLog.setRespBatch(pcRespBatch);
		//Banner(cLog);
		cLog.Banner(&is_appname, &is_appversion, &is_appdesc, LOGFILE);

		//cLog << cParamFile << endl;
		cParamFile.show(cLog, LOGFILE);
	}  /* End If */



	//-----------------------------------------------------------------+
	// If okay, switch on Request/Mode
	//-----------------------------------------------------------------+
	if(ll_return == KOCC_GOOD) {

		switch (ii_usermode | ii_appmode) {

		//-------------------------------------------------------------+
		// Get Batch
		//-------------------------------------------------------------+
		case APP_MODE_GET | USER_MODE_BATCH:

			ll_return = cGetBatch.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cGetBatch.Execute();
			}

			cGetBatch.termSelf();
			ls_msg << cGetBatch.GetStatus();
			break;

		//-------------------------------------------------------------+
		// Put Batch
		//-------------------------------------------------------------+
		case APP_MODE_PUT | USER_MODE_BATCH:

			ll_return = cPutBatch.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cPutBatch.Execute();
			}

			cPutBatch.termSelf();
			ls_msg << cPutBatch.GetStatus();
			break;
			
		//-------------------------------------------------------------+
		// Process Dead Letter Queue
		//-------------------------------------------------------------+
		case APP_MODE_DEAD_LETTER | USER_MODE_DLQ:

			ll_return = cDeadSingle.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cDeadSingle.Execute();
			}

			cDeadSingle.termSelf();
			ls_msg << cDeadSingle.GetStatus();
			break;



		//-------------------------------------------------------------+
		// Run Dead Letter Diagnostics Report
		//-------------------------------------------------------------+
		case APP_MODE_DEAD_LETTER | USER_MODE_DLQ_REPORT:

			ll_return = cDeadReport.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cDeadReport.Execute();
			}

			cDeadReport.termSelf();
			ls_msg << cDeadReport.GetStatus();
			break;



		//-------------------------------------------------------------+
		// Get Single Queue
		//-------------------------------------------------------------+
		case APP_MODE_GET | USER_MODE_SINGLE_QUEUE:


			ll_return = cGetSingle.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cGetSingle.Execute();
			}


			cGetSingle.termSelf();
			ls_msg << cGetSingle.GetStatus();
			break;

		//-------------------------------------------------------------+
		// Put Single Queue
		//-------------------------------------------------------------+
		case APP_MODE_PUT | USER_MODE_SINGLE_QUEUE:

			ll_return = cPutSingle.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cPutSingle.Execute();
			}

			cPutSingle.termSelf();
			ls_msg << cPutSingle.GetStatus();
			break;


		//-------------------------------------------------------------+
		// Unexpected/Invalid Combination
		//-------------------------------------------------------------+
		default:

			ll_final_return = KOCC_FAIL;

			ls_msg << "Mode Setting '" 
				   << ii_usermode 
				   << "' not valid with application setting '"
				   << ii_appmode 
				   << "'.";

			cLog.LogError(KORC_INVALID_APP_USER_MODE, ll_final_return, 
				ls_msg,
				"CCmdInterface::execute::1", 
				is_InstanceName.getPointer());

			break;
		}	// end switch

	}

	if (ll_final_return == KOCC_WARNING) {
		ll_final_return = KOCC_GOOD;
	}


	//-----------------------------------------------------------------+
	// Report Status
	//-----------------------------------------------------------------+
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

	#ifdef _RunTime_
	ls_msg = "";
	ls_msg << "            Elapsed run time:  " 
		       << cSysCmds.getTimerIntervalStr() << ".\n";
	#else
	ls_msg = "\n";
	#endif
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

	ls_msg = "";
	ls_msg << is_appname << " ending with return code " 
		   << ll_final_return << ".\n";
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);



	//-----------------------------------------------------------------+
	// If okay, write, close, and release the Response cache
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ls_parm =  (char*) cParamFile.GetMessageID();
		ls_msg  =  (char*) cParamFile.GetCorrelIDStr();
		if (cParamFile.isLogReceipt() 
	    ||  cParamFile.isDataReceipt()) {
			pcRespBatch->put(&ls_parm, &ls_msg);
		} // end if
		pcRespBatch->close();
		delete pcRespBatch;
	}  /* End If */
	
	
	//-----------------------------------------------------------------+
	// If open, Close Log File
	//-----------------------------------------------------------------+
	cLog.termSelf();

	return ll_final_return;
}  /* End execKOMQ */






//---------------------------------------------------------------------+
//  Method:   execESU
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This execute is for Extension Services
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:
//
//  History: 
//  11/10/99 DRC Created original version.
//---------------------------------------------------------------------+
long CCmdInterface::execESU(int argc, const char *argv[]) {

	int				rc				= KOCC_FAIL;
	long			ll_return		= KOCC_FAIL;
	long			ll_final_return	= KOCC_FAIL;
	BOOL			bLogOpen		= FALSE;
	CExtSrvcsParamFile cParamFile;
	CLog			cLog;
	CString			ls_msg(132);
	CString         ls_parm(48);
	//
	//  Services Functions
	//
	CExtServices    cExtSrvcs;
	CSysCmd         cSysCmds;

	//-----------------------------------------------------------------+
	// Display Version Information
	//-----------------------------------------------------------------+
	cLog.Banner(&is_appname, &is_appversion, &is_appdesc, LOGCONSOLE);


	//-----------------------------------------------------------------+
	// Get Parameters from Command Line
	//-----------------------------------------------------------------+
	CCmdArgs	cCmdLine(argc, argv);
	CopyErrors(&cCmdLine);
	cLog.LogErrors(cCmdLine, LOGCONSOLE);


	//-----------------------------------------------------------------+
	// If okay, Get Parameters from Parameter File
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {
		cParamFile.Load(cCmdLine);

		CopyErrors(&cParamFile);
		cLog.LogErrors(cParamFile);
	}/* End if */


	//-----------------------------------------------------------------+
	// If okay, Validate Global Parms
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {

		ii_usermode = cParamFile.GetMode();

		long li_appmode = 0;

		li_appmode = cParamFile.GetAppMode();

		if (li_appmode != 0) {
			ii_appmode = li_appmode;
		} /* End If */
	} /* End If */
	
	
	//-----------------------------------------------------------------+
	// If okay, Open Log File
	//
	// If not okay, set ll_return and use ll_return to determine if
	// we continue.
	//
	// Note that here we switch from checking for Errors in the Errors
	// collection to using return codes.
	//-----------------------------------------------------------------+
	if (!iu_errors.HasError()) {

		ll_return = cLog.initSelf((char*) cParamFile.GetLogFileName());
	    cLog.Banner(&is_appname, &is_appversion, &is_appdesc, LOGFILE);
		cParamFile.show(cLog, LOGFILE);

	}
	else {
		ll_return = KOCC_FAIL;
	}  /* End If */



	//-----------------------------------------------------------------+
	// If okay, switch on Request/Mode
	//-----------------------------------------------------------------+
	if(ll_return == KOCC_GOOD) {

		switch (ii_usermode | ii_appmode) {


		//-------------------------------------------------------------+
		// Extended Services Utility
		//-------------------------------------------------------------+
		case APP_MODE_EXT_SRVCS | USER_MODE_BATCH:

			ll_return = cExtSrvcs.initSelf(&cParamFile, &cLog);

			if (ll_return == KOCC_GOOD) {
				ll_final_return = cExtSrvcs.Execute();
			}

			cExtSrvcs.termSelf();
			ls_msg << cExtSrvcs.GetStatus();
			break;


		//-------------------------------------------------------------+
		// Unexpected/Invalid Combination
		//-------------------------------------------------------------+
		default:

			ll_final_return = KOCC_FAIL;

			ls_msg << "Mode Setting '" 
				   << ii_usermode 
				   << "' not valid with application setting '"
				   << ii_appmode 
				   << "'.";

			cLog.LogError(KORC_INVALID_APP_USER_MODE, ll_final_return, 
				ls_msg,
				"CCmdInterface::execute::1", 
				is_InstanceName.getPointer());

			break;
		}	// end switch

	}

	if (ll_final_return == KOCC_WARNING) {
		ll_final_return = KOCC_GOOD;
	}


	//-----------------------------------------------------------------+
	// Report Status
	//-----------------------------------------------------------------+
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

	#ifdef _RunTime_
	ls_msg = "";
	ls_msg << "                    Elapsed run time:  " 
		   << cSysCmds.getTimerIntervalStr() << ".\n";
	#else
	ls_msg = "\n";
	#endif
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

	ls_msg = "";
	ls_msg << is_appname << " ending with return code " 
		   << ll_final_return << ".\n";
	cLog.WriteLog(ls_msg, LOGCONSOLE + LOGFILE);



	//-----------------------------------------------------------------+
	// If open, Close Log File
	//-----------------------------------------------------------------+
	cLog.termSelf();

	return ll_final_return;
}  /* End execESU */




//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  03/??/99  DMT Created original version.
//---------------------------------------------------------------------+
int	CCmdInterface::initSelf(CString&   rs_appname,
						    CString&   rs_appdesc,
						    CString&   rs_appversion,
							int		   vi_appmode) {

	int li_return = KOCC_GOOD;

	li_return = CAny2::initSelf();

	if (li_return == KOCC_GOOD) {
		is_appname		= rs_appname;
		is_appdesc		= rs_appdesc;
		is_appversion	= rs_appversion;
		ii_appmode		= vi_appmode;
	}

	return li_return;
}  /* End Method */
