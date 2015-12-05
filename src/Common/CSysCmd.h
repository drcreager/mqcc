//---------------------------------------------------------------------+
//  File:     CSysCmd.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CSysCmd class, which encapsulates 
//            the functionality of local operating system's command 
//            processor. 
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "CAny.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
	#ifdef _POSIX_
	#undef _POSIX_ 
	#define _NO_POSIX_
		#include <process.h>
	#endif
    #include <direct.h>
#else
        #include <stdlib.h>
	#include <unistd.h>
	#include <errno.h>
        #include <sys/stat.h>
	#include <sys/types.h>
	#include <sys/wait.h>
#endif

#define HOUR   3600
#define MINUTE 60

class CSysCmd : public CAny  {
protected:
    time_t   m_Timer;
	char     m_Buffer[50];

public:
	             CSysCmd();
	            ~CSysCmd();
	virtual double getTimerInterval( void );
	virtual char*  getTimerIntervalStr( void );
    virtual long   MakeDir(  char* a_szPathName);
	virtual void   setTimer( void ); 
	virtual void   Sleep(    long  a_Interval);
    virtual long   Spawn(    char* a_szPgmName, char* a_szArg);

    virtual long   Spawn(    char* a_szPgmName, char* a_szArg1, 
					         char* a_szArg2,    char* a_szArg3, 
					         char* a_szArg4,    char* a_szArg5);

    virtual long   SpawnWait(char* a_szPgmName, char* a_szArg); 

    virtual long   SpawnWait(char* a_szPgmName, char* a_szArg1, 
				   	         char* a_szArg2,    char* a_szArg3, 
					         char* a_szArg4,    char* a_szArg5);
};


#ifdef _WIN32
	#ifdef _NO_POSIX_
	#undef _NO_POSIX_
	#define _POSIX_
	#endif
#endif