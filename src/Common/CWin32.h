//-----------------------------------------------------------------------+
//  File:     CWin32.h
//  Author:   Daniel R Creager
//  Written:  July 1996
//  Desc:     .... 
//
//  History: 
//  07/06/96  DRC Created original code.
//-----------------------------------------------------------------------+
#ifndef _CAny
#define _CAny
#include "CAny.h"
#endif

#ifndef _windows
#define _windows
#include <windows.h>
#endif

#ifndef _CString
#define _CString
#include "CString.h"
#endif

//---------------------------------------------------------------------+
//  Class:    CWin32
//---------------------------------------------------------------------+
class CWin32 : public CAny
{
protected:
HANDLE	 mh_Process;

public:
   virtual void  f_Beep(  int vi_freq, int vi_dur );
   virtual void  f_Sleep( int vi_dur);
   virtual DWORD getLastError(void);
   virtual DWORD shell(CString* ls_Command, DWORD ld_timeOut);
   virtual long  exec(CString* ls_Command);
   virtual long  start(CString* as_Command, HANDLE* ah_Process);
   virtual long  check(HANDLE ah_Process, DWORD* ap_ExitCode);



   CWin32( void );
   ~CWin32( void );
};


