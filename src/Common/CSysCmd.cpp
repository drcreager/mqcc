//---------------------------------------------------------------------+
//  File:     CSysCmd.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation of the CSysCmd class, which encapsulates 
//            the functionality of local operating system's command 
//            processor. 
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "CSysCmd.h"

//---------------------------------------------------------------------+
//  Method:   CSysCmd (default Constructor)
//  Desc:     Load the contents of this collection based upon the 
//            specifications provide on invocation. 
//  Args:     char* (a file search argument valid to the host file system)
//            BOOL  (Indicator about searching sub-directories)
//                   True  = Open and search all sub-directories
//                   False = Ignore all sub-directories.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CSysCmd::CSysCmd(){
	setTimer();
} // end CSysCmd 





//---------------------------------------------------------------------+
//  Method:   ~CSysCmd (destructor)
//  Desc:     Unload the contents of this collection releasing all 
//            of its resources. 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CSysCmd::~CSysCmd(){}




//---------------------------------------------------------------------+
//  Method:   MakeDir 
//  Desc:     Construct a file system directory.
//  Args:     tbd
//  Returns:  tbd
//---------------------------------------------------------------------+
long CSysCmd::MakeDir(char* a_szPathName){
long l_RC = 0;

#ifdef _WIN32
    l_RC = _mkdir(a_szPathName);
#else
	l_RC = mkdir(a_szPathName,S_ISGID | S_IRWXU | S_IRWXG | S_IROTH);
#endif
	return l_RC;
}  /* end MakeDir */
	
//---------------------------------------------------------------------+
//  Method:   Spawn 
//  Desc:     Execute a program in a new process.
//  Args:     tbd
//  Returns:  tbd
//---------------------------------------------------------------------+
long CSysCmd::Spawn(char* a_szPgmName, char* a_szArg1, 
					char* a_szArg2,    char* a_szArg3, 
					char* a_szArg4,    char* a_szArg5){

#ifdef _WIN32
return _spawnl(_P_NOWAIT, a_szPgmName, a_szPgmName,
			   a_szArg1,  a_szArg2,    a_szArg3, 
			   a_szArg4,  a_szArg5,    NULL);
#else
int l_iCID;
int l_iCStatus;
long l_lResult = 0;

//
//  Fork creates a second process that is a copy of this process
//
l_iCID = fork();  

//
//  The way to determine which process we are in after a fork(), 
//  is by checking the value of the child process id (l_iCID)
//  -1  == A fork() error occured there is no child process
//  0   == This is the child process 
//  1-n == A child process was created but this is the parent process.
//
switch (l_iCID){
	case -1:
		//
		//  Indicate the failure to create a child process to
		//  the caller.
		//
		l_lResult = errno;
		exit(errno);
		break;

	case 0:
		//
		//  Replace this program with the specified program and run 
		//  it within the child process.
		//
		l_iCStatus = execl(a_szPgmName, a_szPgmName, a_szArg1,  a_szArg2,    
			               a_szArg3,    a_szArg4,    a_szArg5,  NULL);
		//
		// report any execl() errors
		//
		if (l_iCStatus == -1) l_lResult = errno;
		exit(errno);
		break;

	default:
		l_lResult = 0;
} /* end switch */

return l_lResult;
#endif
} /* End Spawn */


long CSysCmd::Spawn(char* a_szPgmName, char* a_szArg){

return Spawn(a_szPgmName, a_szArg,  NULL,
			 NULL,        NULL,     NULL);
} /* End Spawn */


//---------------------------------------------------------------------+
//  Method:   SpawnWait 
//  Desc:     Execute a program in a new process and wait for it to 
//            complete.
//  Args:     tbd
//  Returns:  tbd
//---------------------------------------------------------------------+
long CSysCmd::SpawnWait(char* a_szPgmName, char* a_szArg1, 
				   	    char* a_szArg2,    char* a_szArg3, 
					    char* a_szArg4,    char* a_szArg5){ 

#ifdef _WIN32
	return _spawnl(_P_WAIT,   a_szPgmName, a_szPgmName,
			   a_szArg1,  a_szArg2,    a_szArg3, 
			   a_szArg4,  a_szArg5,    NULL);
#else
long l_lResult;
int  l_iCStatus;

l_lResult = Spawn(a_szPgmName, a_szArg1, a_szArg2,
	              a_szArg3,    a_szArg4, a_szArg5);
//
//  on SuccessfulSpawn, Wait for the child process to complete 
//  and capture its termination status
//
if (l_lResult == 0) {
	wait(&l_iCStatus);
	l_lResult = l_iCStatus;
} /* end if */

return l_lResult;
#endif
} /* End SpawnWait */



long CSysCmd::SpawnWait(char* a_szPgmName, char* a_szArg){ 

return SpawnWait(a_szPgmName, a_szArg,  NULL,
			     NULL,        NULL,     NULL);
} /* End SpawnWait */



//---------------------------------------------------------------------+
//  Method:   Sleep 
//  Desc:     Pause processing for a specified amount of time.
//  Args:     long The amount of time to sleep in milliseconds (1/1000s)
//  Returns:  void
//---------------------------------------------------------------------+
void CSysCmd::Sleep(long a_Interval){
	
//
//  1) Convert the Milliseconds into ClocksPerSecond
//  2) Calculate the time when the interval has passed
//  3) Loop until the interval is over
//
clock_t goal = (clock_t) ((a_Interval / 1000.0) * CLOCKS_PER_SEC);
goal += clock();
while(goal > clock());
} // end Sleep




//---------------------------------------------------------------------+
//  Method:   setTimer 
//  Desc:     Resets a system timer
//  Args:     None
//  Returns:  void
//---------------------------------------------------------------------+
void CSysCmd::setTimer( void ) {  
   time(&m_Timer);
} // end setTimer



//---------------------------------------------------------------------+
//  Method:   getTimerInterval 
//  Desc:     Retrieve the number of seconds since the Timer was last
//            reset.
//  Args:     None
//  Returns:  double Interval in seconds since the last reset.
//---------------------------------------------------------------------+
double CSysCmd::getTimerInterval( void ) {
   time_t  l_Finish;
   double  l_Result;

   time(&l_Finish);
   l_Result = difftime( l_Finish, m_Timer );

   return l_Result;
} // end getTimerInterval



//---------------------------------------------------------------------+
//  Method:   getTimerIntervalStr 
//  Desc:     Retrieve the number of seconds since the Timer was last
//            reset as a formated string.
//  Args:     None
//  Returns:  String Interval in seconds since the last reset.
//                   01 Hrs 03 Mins 18.023 Secs
//---------------------------------------------------------------------+
char* CSysCmd::getTimerIntervalStr( void ) {
   double l_Seconds = getTimerInterval();
   long   l_Hours = 0;
   long   l_Minutes = 0; 
 
   if (l_Seconds >= HOUR){
	   l_Hours    = (long) (l_Seconds / HOUR);
	   l_Seconds -= l_Hours * HOUR;
   } // end if

   if (l_Seconds >= MINUTE){
	   l_Minutes  = (long) (l_Seconds / MINUTE);
	   l_Seconds -= l_Minutes * MINUTE;
   } // end if

   if (l_Hours > 1)
	   sprintf(m_Buffer, "%-2.0d Hrs %-2.0d Mins %-2.0f Secs",
	                   l_Hours, l_Minutes, l_Seconds );
   else if (l_Hours > 0)
	   sprintf(m_Buffer, "%-2.0d Hour %-2.0d Mins %-2.0f Secs",
	                   l_Hours, l_Minutes, l_Seconds );
   else if (l_Minutes > 1)
	   sprintf(m_Buffer, "%-2.0d Mins %-2.0f Secs",l_Minutes, l_Seconds );

   else if (l_Minutes > 0)
	   sprintf(m_Buffer, "%-2.0d Minute %-2.0f Secs",l_Minutes, l_Seconds );

   else if (l_Seconds > 1)
	   sprintf(m_Buffer, "%-2.0f Secs",l_Seconds );

   else if (l_Seconds > 0)
	   sprintf(m_Buffer, "%-2.0f Second",l_Seconds );

   else
	   strcpy(m_Buffer,"Interval Unknown");
   return m_Buffer;
} // end getTimerInterval