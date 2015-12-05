//---------------------------------------------------------------------+
//  File:     CWin32.cpp
//  Author:   Daniel R Creager
//  Written:  July 1996
//  Desc:     Implementation of the CWin32 class. 
//
//  History: 
//  07/06/96  DRC Created original code.
//---------------------------------------------------------------------+
#include "CWin32.h"

//---------------------------------------------------------------------+
//  Method:   CWin32::Beep
//  Desc:     Make a beep sound.  
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  07/02/96  DRC Created original code.
//---------------------------------------------------------------------+
void CWin32::f_Beep(int vi_freq, int vi_dur) {
Beep(vi_freq,vi_dur);
return;
}

//---------------------------------------------------------------------+
//  Method:   CWin32::Sleep
//  Desc:     Wait in a non-dispatachable state for the specified 
//            period of time.  
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  07/02/96  DRC Created original code.
//---------------------------------------------------------------------+
void CWin32::f_Sleep(int vi_dur) {
Sleep(vi_dur);
return;
}

//---------------------------------------------------------------------+
//  Method:   CWin32::getLastError
//  Desc:     Return the last Error which occured within the current 
//            thread.  
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  04/03/97  DRC Created original code.
//---------------------------------------------------------------------+
DWORD CWin32::getLastError( void ) {
return GetLastError();
}

//---------------------------------------------------------------------+
//  Method:   CWin32::shell
//  Desc:     Return the last Error which occured within the current 
//            thread.  
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  04/03/97  DRC Created original code.
//---------------------------------------------------------------------+
DWORD CWin32::shell(CString* ls_Command, DWORD ld_timeOut) {
//
//  declare local variables 
//
STARTUPINFO			lu_startUpInfo;
PROCESS_INFORMATION	lu_procInfo;
BOOL				lb_RC;

//
//  Retrieve the STARTUP structure for the currrent process
//
GetStartupInfo(&lu_startUpInfo);

//
//  Create the child process
//
lb_RC = CreateProcess(0,ls_Command->getPointer(), 0, 0,
					  FALSE,CREATE_NEW_CONSOLE, 0,0, 
					  &lu_startUpInfo, &lu_procInfo);

//
//  Wait for the child process to complete
//
if (ld_timeOut == 0) ld_timeOut = INFINITE;

if (lb_RC) return WaitForSingleObject(lu_procInfo.hProcess,ld_timeOut);
else       return GetLastError();
}

   
//---------------------------------------------------------------------+
//  Method:   CWin32::exec
//  Desc:     Return the last Error which occured within the current 
//            thread.  
//
//  Parms:    CString*(Command String)
//  Returns:  DWORD(Return Code) = Value Returned by Invoked Process
// 
//  History: 
//  11/25/97  DRC Created original code.
//---------------------------------------------------------------------+
long CWin32::exec(CString* ls_Command) {
DWORD  ld_RC;
DWORD  ld_AppRC;
HANDLE lh_Process;
int    li_Interval = 1000;

ld_RC = start(ls_Command, &lh_Process);
if (ld_RC == 0){ 
	while (ld_RC != 1){
		f_Sleep(li_Interval); 
		ld_RC = check(lh_Process, &ld_AppRC);
	}
	return ld_AppRC;
}
else {
	return ld_RC;	
}
}

//---------------------------------------------------------------------+
//  Method:   CWin32::start
//  Desc:     Asyncronously start a new process based upon the 
//            command string provided by the caller.
//  
//            Note:  this is a wrapper function for CreateProcess  
//
//  Parms:    CString*(Command String)
//            Handle(Handle to the new Process, set by this method)
//  Returns:  DWORD(Return Code) =  0 = Successful Start up
//                               != 0 = Error code from CreateProcess
// 
//  History: 
//  11/25/97  DRC Created original code.
//---------------------------------------------------------------------+
long CWin32::start(CString* as_Command, HANDLE* ah_Process) {
//
//  declare local variables 
//
STARTUPINFO			lu_startUpInfo;
PROCESS_INFORMATION	lu_procInfo;
BOOL				lb_RC;

//
//  Retrieve the STARTUP structure for the currrent process
//
GetStartupInfo(&lu_startUpInfo);

//
//  Create the child process
//
lb_RC = CreateProcess(0,as_Command->getPointer(), 0, 0,
					  FALSE,CREATE_NEW_CONSOLE, 0,0, 
					  &lu_startUpInfo, &lu_procInfo);

if (lb_RC) {
	*ah_Process = lu_procInfo.hProcess;
	mh_Process  = lu_procInfo.hProcess;
	return 0;
}
else {
	*ah_Process = NULL;
	mh_Process  = NULL;
	return GetLastError();
}
}


//---------------------------------------------------------------------+
//  Method:   CWin32::check
//  Desc:     Asyncronously determine if a process has terminated and 
//            return its return code to the caller.  (Note: this 
//            method could logically have been called "isProcessDone")
//
//            Note:  STILL_ACTIVE = 259(x'103)
//
//  Parms:    CString*(Command String)
//  Returns:  DWORD(Process Status) =  0 = Process is still active
//                                  =  1 = Process has Terminated
//                                  = -1 = Error determining status
// 
//  History: 
//  11/25/97  DRC Created original code.
//---------------------------------------------------------------------+
long CWin32::check(HANDLE ah_Process, LPDWORD ap_ExitCode) {

long    ll_RC;
DWORD   ld_ExitCode;
LPDWORD	lp_ExitCode;

lp_ExitCode = &ld_ExitCode;
ll_RC = GetExitCodeProcess(mh_Process, lp_ExitCode);
*ap_ExitCode = ld_ExitCode;

if (ll_RC == 0) {
	*ap_ExitCode = GetLastError();
	return -1;
}
else {
	if (ld_ExitCode == STILL_ACTIVE){
		return 0;
	}
	else {
		return 1;
	}
}
}


//---------------------------------------------------------------------+
//  Method:   CWin32::CWin32
//  Desc:     This Class Constructor is currently a null method. 
//
//  Parms:    Nothing.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
CWin32::CWin32() { 
mh_Process = NULL;
}

//---------------------------------------------------------------------+
//  Method:   CWin32::~CWin32
//  Desc:     This Class Destructor is currently a null method. 
//
//  Parms:    Nothing.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
CWin32::~CWin32() { return; }
