/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) StartupParamFile.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     StartupParamFile.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the 
//            CStartParamFile Class.
//  
//            This class represents a Parameter File and is derived from
//            CParamFile.  It's a sibling to CRestartFile.
//  
//  Dev Log:  
//
//  History: 
//  01/15/99 CAH Created original version.
//  02/24/99 DMT Added two more parameters. 
//               TranslateKeyValues
//               AutoBalance
//
//  09/28/99 DRC Added four more parameters.
//               WaitInterval
//	             CCSID
//               Encoding
//               ReturnReceipt
//  02/12/00 DRC Added the show method
//  04/12/00 DRC Added missing parameter EmptyQueueRC.
//
//---------------------------------------------------------------------+
#include "StartupParamFile.h"

const char* CStartParamFile::m_szRestartFileName;
const char* CStartParamFile::m_szLogFileName;
const char* CStartParamFile::m_szDataFileName;
const char* CStartParamFile::m_szCtlFileName;
const char* CStartParamFile::m_szQueMgrName;
const char* CStartParamFile::m_szErrQueName;
const char* CStartParamFile::m_szCtlQueName;
const char* CStartParamFile::m_szDataQueName;
const char* CStartParamFile::m_szReplyToQName;
      long  CStartParamFile::m_lBatchInterval;
      long  CStartParamFile::m_lBufferSize;
const char* CStartParamFile::m_szMsgID;
      long  CStartParamFile::m_lCorrelID;
const char* CStartParamFile::m_szPassword;
      long  CStartParamFile::m_lMode;
	  long  CStartParamFile::m_lAppMode;
const char* CStartParamFile::m_szDeadLetterQueName;
      long  CStartParamFile::m_lFailOnQuiesce;
      long  CStartParamFile::m_lExpirationInterval;
const char* CStartParamFile::m_szMessageFormat; 
      long  CStartParamFile::m_lAdminAccess;
	  long  CStartParamFile::m_lTranslateKeyValues; 	// DMT 02/24/99
      long  CStartParamFile::m_lAutoBalance;	        // DMT 02/24/99
      long  CStartParamFile::m_lWaitInterval;           // DRC 09/28/99
      long  CStartParamFile::m_lCCSID;              	// DRC 09/28/99
      long  CStartParamFile::m_lEncoding;           	// DRC 09/28/99
const char* CStartParamFile::m_szReturnReceipt;	        // DRC 09/28/99
      long  CStartParamFile::m_lEmptyQueueRC;           // DRC 04/12/02 


      long  CStartParamFile::m_lDataMsgsProcessed;
      long  CStartParamFile::m_lTotalDataMsgsProcessed;
      long  CStartParamFile::m_lCtrlMsgsProcessed;
      long  CStartParamFile::m_lTotalCtrlMsgsProcessed;
      long  CStartParamFile::m_lExpectedMsgs;

	  long  CStartParamFile::m_lReplyMsgsProcessed;
	  long  CStartParamFile::m_lTotalReplyMsgsProcessed;

      long  CStartParamFile::m_lErrorMsgsProcessed;
	  long  CStartParamFile::m_lTotalErrorMsgsProcessed;

	  long  CStartParamFile::m_lSynchpoints;

	  long  CStartParamFile::m_lUncommittedDataMsgs;
	  long  CStartParamFile::m_lUncommittedCtrlMsgs;







//---------------------------------------------------------------------+
//  Table that defined parameters in the Parameter File
//
//	NULL/TypeInvalid entry indicates end of table.
//---------------------------------------------------------------------+
CStartParamFile::SParamTable CStartParamFile::sParamTable[] =
{
//
//	Argument Name		   Argument Type  Ptr to storage        Req?   Loaded?
//
	{ "QMgrName",			TypeString, &m_szQueMgrName,	    TRUE,  FALSE },
	{ "DataQueueName",		TypeString, &m_szDataQueName, 	    TRUE,  FALSE },
	{ "ControlQueueName",	TypeString, &m_szCtlQueName, 	    FALSE, FALSE },
	{ "ErrorQueueName",		TypeString, &m_szErrQueName, 	    FALSE, FALSE },
	{ "LogFileName",		TypeString, &m_szLogFileName,	    TRUE,  FALSE },
	{ "DataFileName",		TypeString, &m_szDataFileName,	    FALSE, FALSE },
	{ "ControlFileName",	TypeString, &m_szCtlFileName,	    FALSE, FALSE },
	{ "RestartFileName",	TypeString, &m_szRestartFileName,   FALSE, FALSE },
	{ "BatchInterval",		TypeLong,	&m_lBatchInterval,	    FALSE, FALSE },
	{ "BufferSize",			TypeLong,	&m_lBufferSize,		    TRUE,  FALSE },
	{ "ReplyToQName",		TypeString, &m_szReplyToQName,      FALSE, FALSE },
	{ "MsgID",              TypeString, &m_szMsgID,             FALSE, FALSE },
	{ "CorrelID",           TypeLong,   &m_lCorrelID,           FALSE, FALSE },
	{ "Password",           TypeString, &m_szPassword,          FALSE, FALSE },
	{ "Mode",               TypeLong,   &m_lMode,               FALSE, FALSE },
	{ "AppMode",            TypeLong,   &m_lAppMode,            FALSE, FALSE },
	{ "DeadLetterQueueName",TypeString, &m_szDeadLetterQueName, FALSE, FALSE },
	{ "FailOnQuiesce",      TypeLong,   &m_lFailOnQuiesce,      FALSE, FALSE },
	{ "ExpirationInterval", TypeLong,   &m_lExpirationInterval, FALSE, FALSE },
	{ "MessageFormat",      TypeString, &m_szMessageFormat,     FALSE, FALSE },
#ifdef _ver2_
	{ "KeyTranslation",     TypeLong,   &m_lTranslateKeyValues, FALSE, FALSE },
#else
	{ "TranslateKeyValues", TypeLong,   &m_lTranslateKeyValues, FALSE, FALSE },
#endif
	{ "AutoBalance",        TypeLong,   &m_lAutoBalance,        FALSE, FALSE },
	{ "WaitInterval",       TypeLong,   &m_lWaitInterval,       FALSE, FALSE },
	{ "CCSID",              TypeLong,   &m_lCCSID,              FALSE, FALSE },
	{ "Encoding",           TypeLong,   &m_lEncoding,           FALSE, FALSE },
	{ "ReturnReceipt",      TypeString, &m_szReturnReceipt,     FALSE, FALSE },
	{ "EmptyQueueRC",       TypeLong,   &m_lEmptyQueueRC,       FALSE, FALSE },

#ifdef _restart_extract_
//	{ "AdminAccess",        TypeLong,   &m_lAdminAccess,        FALSE, FALSE },
		
//	{ "DataMsgsRead",			TypeLong,   &m_lDataMsgsProcessed,	FALSE, FALSE },
//	{ "TotalDataMsgsProcessed",	TypeLong,   &m_lTotalDataMsgsProcessed,	FALSE, FALSE },
//	{ "CtrlMsgsProcessed",		TypeLong,	&m_lCtrlMsgsProcessed,	FALSE, FALSE },
//	{ "TotalCtrlMsgsProcessed",	TypeLong,   &m_lTotalCtrlMsgsProcessed,	FALSE, FALSE },
//	{ "ExpectedMessageCount",	TypeLong,	&m_lExpectedMsgs,		FALSE, FALSE },

//  { "ReplyMsgsProcessed",     TypeLong,   &m_lReplyMsgsProcessed, FALSE, FALSE },
//  { "TotalReplyMsgsProcessed",TypeLong,   &m_lTotalReplyMsgsProcessed, FALSE, FALSE },

//  { "ErrorMsgsProcessed",     TypeLong,   &m_lErrorMsgsProcessed, FALSE, FALSE },
//  { "TotalErrorMsgsProcessed",TypeLong,   &m_lTotalErrorMsgsProcessed, FALSE, FALSE },

//  { "Synchpoints",            TypeLong,   &m_lSynchpoints,     FALSE, FALSE },

//	{ "UncommittedDataMsgs",    TypeLong,   &m_lUncommittedDataMsgs, FALSE, FALSE },
//  { "UncommittedCtrlMsgs",    TypeLong,   &m_lUncommittedCtrlMsgs, FALSE, FALSE },
#endif
	{ NULL,						TypeInvalid, NULL,				    TRUE,  FALSE }
};





//---------------------------------------------------------------------+
//  Method:   GetParamTable
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Pointer to the Parameter Table
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const CStartParamFile::SParamTable* CStartParamFile::GetParamTable()
{
	return sParamTable;
}





//---------------------------------------------------------------------+
//  Method:   LoadParams
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets Parameter Filename from Command Args Class
//
//            Calls Load Method to Load Parameters from 
//            File
//
//            Overrides Batch Interval if present in Commmand Args
//
//            Dumps Parameter Values to Error Device
//
//  Args:     ByRef CCmdArgs Class
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::LoadParams(CCmdArgs& args)
{
	long	ll_return = KOCC_GOOD;

	Open(args.GetParamFileName());

	if(!iu_errors.HasError()) {
		Load();

		if (TRUE == args.BatchIntervalOverride())
			m_lBatchInterval = args.GetBatchInterval();
	}

}






//---------------------------------------------------------------------+
//  Method:   initMembers
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/24/99 DMT Added two more members to initialize.
//  09/28/99 DRC Added four more members to initialize.
//  01/08/00 DRC Modified the default for TranslateKeyValues to ON
//---------------------------------------------------------------------+
void CStartParamFile::initMembers()
{
    m_szRestartFileName		= NULL;
    m_szLogFileName			= NULL;
    m_szDataFileName		= NULL;
    m_szQueMgrName			= NULL;
    m_szErrQueName			= NULL;
    m_szCtlQueName			= NULL;
    m_szDataQueName			= NULL;

	m_szCtlFileName			= NULL;	// DMT 02/24/99
	m_szReplyToQName		= NULL; // DMT 02/24/99

	m_lBatchInterval		= 0;
	m_lBufferSize			= 0;

	m_szMsgID				= NULL;
	m_lCorrelID			    = 0;
	m_szPassword			= NULL;
	m_lMode					= 1;
	m_lAppMode              = 0;
	m_szDeadLetterQueName	= NULL;

	m_lFailOnQuiesce		= FALSE;
	m_lExpirationInterval	= KO_MAX_EXPIRY;

	m_szMessageFormat		= NULL;		// Must default to "MQSTR".
	SetString((char**) &m_szMessageFormat, KOFMT_DEFAULT);

    m_bIsRestart			= FALSE;

	m_lAdminAccess          = 0;
#ifdef _ver2_
	m_lTranslateKeyValues   = TRUE;	    		// DRC 01/08/00
#else
	m_lTranslateKeyValues   = FALSE;	        // DMT 02/24/99
#endif
	m_lAutoBalance          = TRUE;			    // DMT 02/24/99

	m_lWaitInterval         = KO_DEFAULT_WAIT_INTERVAL; // DRC 09/28/99
	m_lCCSID                = 0;              	// DRC 09/28/99
    m_lEncoding             = 0;           		// DRC 09/28/99
    m_szReturnReceipt		= NULL;	            // DRC 09/28/99
	SetString((char**) &m_szReturnReceipt, KO_DEFAULT_RETURN_RECEIPT);
    m_lEmptyQueueRC         = 0;                // DRC 04/12/02

	m_lDataMsgsProcessed		= 0;
	m_lTotalDataMsgsProcessed	= 0;

	m_lCtrlMsgsProcessed		= 0;
	m_lTotalCtrlMsgsProcessed	= 0;

	m_lExpectedMsgs				= 0;

	m_lReplyMsgsProcessed		= 0;
	m_lTotalReplyMsgsProcessed	= 0;

	m_lErrorMsgsProcessed		= 0;
	m_lTotalErrorMsgsProcessed	= 0;

	m_lSynchpoints				= 0;

	m_lUncommittedDataMsgs		= 0;
	m_lUncommittedCtrlMsgs		= 0;

	is_InstanceName			= "Parameter File";
}





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Initializes Member Variables
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CStartParamFile::CStartParamFile() : CParamFile() 
{
	initMembers();
}






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     CCmdArgs Class Reference
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CStartParamFile::CStartParamFile(CCmdArgs& args) : CParamFile(args.GetParamFileName())
{
	initMembers();

	// why do this twice when it's done in initMembers?
	//m_lBatchInterval = 0;
	//m_lBufferSize = 0;
	LoadParams(args);
}






//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M. Tamkun
//  Written:  February 1999
//
//  Desc:     Free's Memory for Dynamically allocated String Parameter
//            Values
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  02/24/99 DMT Created original version.
//  09/28/99 DRC Added removal of ReturnReceipt.
//---------------------------------------------------------------------+
CStartParamFile::~CStartParamFile()
{
	// If not NULL, delete free's the memory these pointers to 
	// strings reference.
	delete [] (char *) m_szRestartFileName;
	m_szRestartFileName = NULL;

	delete [] (char *) m_szLogFileName;
	m_szLogFileName = NULL;

	delete [] (char *) m_szDataFileName;
	m_szDataFileName = NULL;

	delete [] (char *) m_szCtlFileName;
	m_szCtlFileName = NULL;

	delete [] (char *) m_szQueMgrName;
	m_szQueMgrName = NULL;

	delete [] (char *) m_szErrQueName;
	m_szErrQueName = NULL;

	delete [] (char *) m_szCtlQueName;
	m_szCtlQueName = NULL;

	delete [] (char *) m_szDataQueName;
	m_szDataQueName = NULL;

	delete [] (char *) m_szReplyToQName;
	m_szReplyToQName = NULL;

	delete [] (char *) m_szMsgID;
	m_szMsgID			= NULL;

	delete [] (char *) m_szPassword;
	m_szPassword		= NULL;

	delete [] (char *) m_szDeadLetterQueName;
	m_szDeadLetterQueName	= NULL;

	delete [] (char *) m_szMessageFormat;
	m_szMessageFormat	= NULL;

	delete [] (char *) m_szReturnReceipt;
	m_szReturnReceipt	= NULL;
}







//---------------------------------------------------------------------+
//  Method:   Load
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     ref to CCmdArgs
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::Load(CCmdArgs& args)
{
	LoadParams(args);
}





//---------------------------------------------------------------------+
//  Method:   GetRestartFileName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetRestartFileName()
{
	return m_szRestartFileName;
}





//---------------------------------------------------------------------+
//  Method:   GetLogFileName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetLogFileName()
{
	return m_szLogFileName;
}






//---------------------------------------------------------------------+
//  Method:   GetCtlFileName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetCtlFileName()
{
	return m_szCtlFileName;
}






//---------------------------------------------------------------------+
//  Method:   GetDataFileName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetDataFileName()
{
	return m_szDataFileName;
}






//---------------------------------------------------------------------+
//  Method:   GetQueMgrName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetQueMgrName()
{
	return m_szQueMgrName;
}






//---------------------------------------------------------------------+
//  Method:   GetErrQueName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/22/99 DMT Removed - No longer using Error Queue.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetErrQueName()
{
	return m_szErrQueName;
}






//---------------------------------------------------------------------+
//  Method:   GetCtlQueName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetCtlQueName()
{
	return m_szCtlQueName;
}






//---------------------------------------------------------------------+
//  Method:   GetDataQueName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetDataQueName()
{
	return m_szDataQueName;
}



//---------------------------------------------------------------------+
//  Method:   GetEmptyQueueRC
//  Author:   Daniel R Creager
//  Written:  April 2002
//  
//  Dev Log:  
//  History: 
//  04/12/2002 DRC Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetEmptyQueueRC(){
	return m_lEmptyQueueRC;
}


    

/*
//---------------------------------------------------------------------+
//  Method:   operator <<
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Dumps parameter values.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  Consider having this loop through parameter table and
//            use keyword values.  This will simplify maintenance.
//
//            This should really use the WriteLog method if this class
//            knew about the CLog class.  
//
//  History: 
//  01/15/99 CAH Created original version.
//  various  DMT Added new parms for Release 2.0 
//  04/27/99 DMT Added ReplyTo Queue Name in logging.
//  09/28/99 DRC Added four more parameters to the logging.
//               WaitInterval, CCSID, Encoding, ReturnReceipt
//---------------------------------------------------------------------+
ostream& operator << (ostream& os, CStartParamFile& pf)
{

	os << "Parameter File Values from '" << pf.m_szFileName << "'." << endl
		<< "Restart file name     : " << valid_str(pf.m_szRestartFileName) << endl
		<< "Log file name         : " << valid_str(pf.m_szLogFileName) << endl
		<< "Data file name        : " << valid_str(pf.m_szDataFileName) << endl
		<< "Control file name     : " << valid_str(pf.m_szCtlFileName) << endl
		<< "Queue Manager name    : " << valid_str(pf.m_szQueMgrName) << endl
		<< "Data Queue name       : " << valid_str(pf.m_szDataQueName) << endl
		<< "Control Queue name    : " << valid_str(pf.m_szCtlQueName) << endl
		<< "Reply Queue name      : " << valid_str(pf.m_szReplyToQName) << endl
		<< "Error Queue name      : " << valid_str(pf.m_szErrQueName) << endl
		<< "Batch interval        : " << pf.m_lBatchInterval << endl
		<< "Buffer size           : " << pf.m_lBufferSize << endl
		<< "Message ID            : " << valid_str(pf.m_szMsgID) << endl
		<< "Correlation ID        : " << pf.m_lCorrelID << endl
     // << "Password              : " << valid_str(pf.m_szPassword) << endl
		<< "Mode                  : " << pf.m_lMode << endl
		<< "Dead Letter Queue Name: " << valid_str(pf.m_szDeadLetterQueName) << endl
		<< "Fail On Quiesce       : " << pf.m_lFailOnQuiesce << endl
		<< "Expiration Interval   : " << pf.m_lExpirationInterval << endl
		<< "Message Format        : " << valid_str(pf.m_szMessageFormat) << endl
	    << "Translate Key Values  : " << pf.m_lTranslateKeyValues << endl
	    << "Auto Balance          : " << pf.m_lAutoBalance << endl
		<< "Wait Interval         : " << pf.m_lWaitInterval << endl
		<< "CCSID                 : " << pf.m_lCCSID << endl
		<< "Encoding              : " << pf.m_lEncoding << endl
		<< "Return Receipt        : " << valid_str(pf.m_szReturnReceipt) << endl;

	if (pf.m_lAdminAccess) {
		os << "Admin Access          : " << pf.m_lAdminAccess << endl;
	}

	return os;
}
*/



//---------------------------------------------------------------------+
//  Method:   show
//  Author:   Daniel R Creager
//  Written:  February 2000
//
//  Desc:     Dumps parameter values.
//
//  Args:     CLog&  CLog Object
//            int    Type of Log entries to write. (Console | File)
//
//  Returns:  Nothing
//  
//  Dev Log:  Consider having this loop through parameter table and
//            use keyword values.  This will simplify maintenance. 
//
//  History: 
//  02/12/00 DRC Created original version from existing operator<<.
//  11/10/00 DRC Added use of valid_bool to translate boolean values.
//---------------------------------------------------------------------+
void CStartParamFile::show(CLog& a_pcLog, int a_iEntryType){

	CString l_sLogRcrd(100);
	
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Parameter File Values from '" 
		   << m_szFileName 
		   << "'.", a_iEntryType);
    
	l_sLogRcrd =  "Restart file name     : "; 
	l_sLogRcrd += (char*) valid_str(m_szRestartFileName);
	a_pcLog.WriteLog(l_sLogRcrd, a_iEntryType);

	l_sLogRcrd =  "Log file name         : "; 
	l_sLogRcrd += (char*) valid_str(m_szLogFileName);
	a_pcLog.WriteLog(l_sLogRcrd, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Data file name        : " 
		   <<  (char*) valid_str(m_szDataFileName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Control file name     : " 
		   << (char*) valid_str(m_szCtlFileName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Queue Manager name    : " 
		   << (char*) valid_str(m_szQueMgrName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Data Queue name       : " 
		   << (char*) valid_str(m_szDataQueName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Control Queue name    : " 
		   << (char*) valid_str(m_szCtlQueName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Reply Queue name      : " 
		   << (char*) valid_str(m_szReplyToQName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Error Queue name      : " 
		   << (char*) valid_str(m_szErrQueName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Batch interval        : " 
		   << m_lBatchInterval, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Buffer size           : " 
		   << m_lBufferSize, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Message ID            : " 
		   << (char*) valid_str(m_szMsgID), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Correlation ID        : " 
		   << m_lCorrelID, a_iEntryType);

  /*a_pcLog.WriteLog(l_sLogRcrd 
		   << "Password              : " 
		   << (char*) valid_str(m_szPassword), a_iEntryType); */

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Mode                  : " 
		   << m_lMode, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Dead Letter Queue Name: " 
		   << (char*) valid_str(m_szDeadLetterQueName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Fail On Quiesce       : " 
		   << (char*) valid_bool(m_lFailOnQuiesce), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Expiration Interval   : " 
		   << m_lExpirationInterval, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Message Format        : " 
		   << (char*) valid_str(m_szMessageFormat), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Translate Key Values  : " 
		   << (char*) valid_bool(m_lTranslateKeyValues), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Auto Balance          : " 
		   << (char*) valid_bool(m_lAutoBalance), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Wait Interval         : " 
		   << m_lWaitInterval, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "CCSID                 : " 
		   << m_lCCSID, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Encoding              : " 
		   << (char*) valid_bool(m_lEncoding), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Return Receipt        : " 
		   << (char*) valid_str(m_szReturnReceipt), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Empty Queue RC        : " 
		   << m_lEmptyQueueRC, a_iEntryType);

    l_sLogRcrd.fill();
	if (m_lAdminAccess) {
		a_pcLog.WriteLog(l_sLogRcrd 
		   << "Admin Access          : " 
		   << (char*) valid_bool(m_lAdminAccess), a_iEntryType);
	}  /* End If */
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   GetBatchInterval
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetBatchInterval()
{
	return m_lBatchInterval;
}




//---------------------------------------------------------------------+
//  Method:   SetBatchInterval
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetBatchInterval(long vl_value) {
	m_lBatchInterval = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetBufferSize
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetBufferSize()
{
	return m_lBufferSize;
}


//---------------------------------------------------------------------+
//  Method:   SetBufferSize
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetBufferSize(long vl_value) {
	m_lBufferSize = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetReplyToQName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetReplyToQName()
{
	return m_szReplyToQName;
}


//---------------------------------------------------------------------+
//  Method:   SetReplyToQName
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::SetReplyToQName(char* szValue) {
	return SetString((char**) &m_szReplyToQName, szValue);
}




//---------------------------------------------------------------------+
//  Method:   GetMessageID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetMessageID() {
	return m_szMsgID;
}




//---------------------------------------------------------------------+
//  Method:   SetMessageID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::SetMessageID(char* szValue) {
	return SetString((char **) &m_szMsgID, szValue);
}





//---------------------------------------------------------------------+
//  Method:   GetCorrelID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetCorrelID() {
	return m_lCorrelID;
}




//---------------------------------------------------------------------+
//  Method:   GetCorrelIDStr
//  Author:   Daniel R Creager
//  Written:  February 2000
//
//  Desc:     Private Member Variable Get Method
//
//  Args:     None
//
//  Returns:  char* to CorrelID in string format
//  
//  Dev Log:  
//  History: 
//  02/12/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CStartParamFile::GetCorrelIDStr() {
	static char szBuffer[24];
	sprintf(szBuffer, "%024d", m_lCorrelID);
	return szBuffer;
}






//---------------------------------------------------------------------+
//  Method:   SetCorrelID
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetCorrelID(long vl_value) {
	m_lCorrelID = vl_value;
}




//---------------------------------------------------------------------+
//  Method:   GetPassword
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetPassword(){
	return m_szPassword;
}




//---------------------------------------------------------------------+
//  Method:   GetMode
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetMode() {
	return m_lMode;
}



//---------------------------------------------------------------------+
//  Method:   SetMode
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetMode(long vl_value) {
	m_lMode = vl_value;
}


//---------------------------------------------------------------------+
//  Method:   GetAppMode
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetAppMode() {
	return m_lAppMode;
}




//---------------------------------------------------------------------+
//  Method:   SetAppMode
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetAppMode(long vl_value) {
	m_lAppMode = vl_value;
}






//---------------------------------------------------------------------+
//  Method:   GetDeadLetterQueName
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetDeadLetterQueName() {
	return m_szDeadLetterQueName;
}




//---------------------------------------------------------------------+
//  Method:   SetDeadLetterQueName
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::SetDeadLetterQueName(char* szValue) {
	return SetString((char **) &m_szDeadLetterQueName, szValue);
}






//---------------------------------------------------------------------+
//  Method:   IsRestart
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsRestart() {
	return m_bIsRestart;
}




//---------------------------------------------------------------------+
//  Method:   SetRestart
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetRestart(BOOL vb_restart) {
	m_bIsRestart = vb_restart;
}


#ifdef _ver2_
//---------------------------------------------------------------------+
//  Method:   isTranslate
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void
//
//  Returns:  BOOL
//  
//  Dev Log:  
//  History: 
//  01/08/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::isTranslate() {
	return (m_lTranslateKeyValues != 0);
}



//---------------------------------------------------------------------+
//  Method:   isLogReceipt
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Determine if the Return Receipt setting is LOG or BOTH
//
//  Args:     void
//
//  Returns:  BOOL
//  
//  Dev Log:  
//  History: 
//  01/08/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::isLogReceipt() {
 
return (*m_szReturnReceipt == 'B' || *m_szReturnReceipt == 'L'
	 || *m_szReturnReceipt == 'b' || *m_szReturnReceipt == 'l');
}




//---------------------------------------------------------------------+
//  Method:   isDataReceipt
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Determine if the Return Receipt setting is DATA or BOTH
//
//  Args:     void
//
//  Returns:  BOOL
//  
//  Dev Log:  
//  History: 
//  01/08/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::isDataReceipt() {

return (*m_szReturnReceipt == 'B' || *m_szReturnReceipt == 'D'
	 || *m_szReturnReceipt == 'b' || *m_szReturnReceipt == 'd');
}
#endif



//---------------------------------------------------------------------+
//  Method:   GetFailOnQuiescing
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetFailOnQuiescing() {
	return m_lFailOnQuiesce;
}





//---------------------------------------------------------------------+
//  Method:   SetFailOnQuiescing
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetFailOnQuiescing(long vl_value) {
	m_lFailOnQuiesce = vl_value;
}





//---------------------------------------------------------------------+
//  Method:   GetExpirationInterval
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetExpirationInterval() {
	return m_lExpirationInterval;
}




//---------------------------------------------------------------------+
//  Method:   SetExpirationInterval
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetExpirationInterval(long vl_value) {
	m_lExpirationInterval = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetMessageFormat
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const char* CStartParamFile::GetMessageFormat() {
	return m_szMessageFormat;
}





//---------------------------------------------------------------------+
//  Method:   SetMessageFormat
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::SetMessageFormat(char* szValue) {
	return SetString((char**) &m_szMessageFormat, szValue);
}




//---------------------------------------------------------------------+
//  Method:   HasAdminAccess
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  long - 1 if user Has Admin Access (entered correct
//                     password during the correct time period).
//
//                   0 otherwise.
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::HasAdminAccess() {
	return m_lAdminAccess;
}




//---------------------------------------------------------------------+
//  Method:   GetTranslate
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
const long CStartParamFile::GetTranslate() {
	return m_lTranslateKeyValues;
}



//---------------------------------------------------------------------+
//  Method:   SetTranslate
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetTranslate(long vl_value) {
	m_lTranslateKeyValues = vl_value;
}




//---------------------------------------------------------------------+
//  Method:   DoAutoBalance
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::DoAutoBalance() {
	return m_lAutoBalance;
}



//---------------------------------------------------------------------+
//  Method:   GetDataMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetDataMsgsProcessed() {
		return m_lDataMsgsProcessed;
}



//---------------------------------------------------------------------+
//  Method:   SetDataMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetDataMsgsProcessed(long vl_value) {
	m_lDataMsgsProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetTotalDataMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetTotalDataMsgsProcessed() {
	return m_lTotalDataMsgsProcessed;
}



//---------------------------------------------------------------------+
//  Method:   SetTotalDataMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetTotalDataMsgsProcessed(long vl_value) {
	m_lTotalDataMsgsProcessed = vl_value;
}




//---------------------------------------------------------------------+
//  Method:   GetCtrlMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetCtrlMsgsProcessed() {
	return m_lCtrlMsgsProcessed;
}





//---------------------------------------------------------------------+
//  Method:   SetCtrlMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetCtrlMsgsProcessed(long vl_value) {
	m_lCtrlMsgsProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetTotalCtrlMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetTotalCtrlMsgsProcessed() {
	return m_lTotalCtrlMsgsProcessed;
}




//---------------------------------------------------------------------+
//  Method:   SetTotalCtrlMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetTotalCtrlMsgsProcessed(long vl_value) {
	m_lTotalCtrlMsgsProcessed = vl_value;
}






//---------------------------------------------------------------------+
//  Method:   GetExpectedMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetExpectedMsgs() {
	return m_lExpectedMsgs;
}



//---------------------------------------------------------------------+
//  Method:   SetExpectedMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetExpectedMsgs(long vl_ExpectedMsgs) {
	m_lExpectedMsgs = vl_ExpectedMsgs;
}






//---------------------------------------------------------------------+
//  Method:   GetReplyMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetReplyMsgsProcessed() {
		return m_lReplyMsgsProcessed;
}



//---------------------------------------------------------------------+
//  Method:   SetReplyMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetReplyMsgsProcessed(long vl_value) {
	m_lReplyMsgsProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetTotalReplyMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetTotalReplyMsgsProcessed() {
	return m_lTotalReplyMsgsProcessed;
}



//---------------------------------------------------------------------+
//  Method:   SetTotalReplyMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetTotalReplyMsgsProcessed(long vl_value) {
	m_lTotalReplyMsgsProcessed = vl_value;
}




//---------------------------------------------------------------------+
//  Method:   GetErrorMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetErrorMsgsProcessed() {
	return m_lErrorMsgsProcessed;
}




//---------------------------------------------------------------------+
//  Method:   SetErrorMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetErrorMsgsProcessed(long vl_value) {
	m_lErrorMsgsProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetTotalErrorMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetTotalErrorMsgsProcessed() {
	return m_lTotalErrorMsgsProcessed;
}




//---------------------------------------------------------------------+
//  Method:   SetTotalErrorMsgsProcessed
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetTotalErrorMsgsProcessed(long vl_value) {
	m_lTotalErrorMsgsProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetSynchpoints
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetSynchpoints() {
	return m_lSynchpoints;
}




//---------------------------------------------------------------------+
//  Method:   SetSynchpoints
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetSynchpoints(long vl_value) {
	m_lSynchpoints = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetUncommittedDataMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetUncommittedDataMsgs() {
	return m_lUncommittedDataMsgs;
}




//---------------------------------------------------------------------+
//  Method:   SetUncommittedDataMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetUncommittedDataMsgs(long vl_value) {
	m_lUncommittedDataMsgs = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetUncommittedCtrlMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
long CStartParamFile::GetUncommittedCtrlMsgs() {
	return m_lUncommittedCtrlMsgs;
}



//---------------------------------------------------------------------+
//  Method:   SetUncommittedCtrlMsgs
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     void on Gets, char* or long on Sets
//
//  Returns:  char* or long on Gets, Void on Sets
//  
//  Dev Log:  
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::SetUncommittedCtrlMsgs(long vl_value)  {
	m_lUncommittedCtrlMsgs = vl_value;
}







//---------------------------------------------------------------------+
//  Method:   Load
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls Ancestor Script to load File values into Parameter 
//            Table.
//
//            Resets Admin Access to 0 to prevent someone from turning
//            it on without providing valid password.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//
//  History: 
//  03/14/99 DMT Created original version.
//---------------------------------------------------------------------+
void CStartParamFile::Load() {

	CParamFile::Load();

	if(CheckPassword()) {
		m_lAdminAccess = 1;
	}
	else {
		m_lAdminAccess = 0;
	}
}







//---------------------------------------------------------------------+
//  Method:   IsValidQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name. 
//
//            Name must not be NULL.
//            Name length must be >= 1 and <= 48.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     szName	- Name to Validate
//
//            szContext - Description to format into Error Msg
//                        ie "Data" for a Data Queue Name, 
//                           "Control" for a Control Queue Name, etc.
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidQueueName(char* szName, char* szContext) {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (szName == NULL) {
		lb_return = FALSE;

		ls_msg << "Required "
			   << szContext
			   << " Queue Name was not provided.";

		iu_errors.AddError(KORC_INVALID_Q_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidQueueName:1",
						   is_InstanceName.getPointer());

	}
	else if ((strlen(szName) < 1) || (strlen(szName) > 48)) {
		lb_return = FALSE;

		ls_msg << szContext 
			   << " Queue Name '"
			   << szName
			   << "' must be between 1 and 48 characters.";

		iu_errors.AddError(KORC_INVALID_Q_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidQueueName:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;
}








//---------------------------------------------------------------------+
//  Method:   IsValidFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided file name. 
//
//            Name must not be NULL.
//            Name length must be >= 1.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     szName	- Name to Validate
//
//            szContext - Description to format into Error Msg
//                        ie "Data" for a Data File Name, 
//                           "Control" for a Control File Name, etc.
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidFileName(char* szName, char* szContext) {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (szName == NULL) {
		lb_return = FALSE;

		ls_msg << "Required "
			   << szContext
			   << " File Name was not provided.";

		iu_errors.AddError(KORC_INVALID_FILE_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidFileName:1",
						   is_InstanceName.getPointer());

	}
	else if ((strlen(szName) < 1)) {
		lb_return = FALSE;

		ls_msg << szContext
			   << " File Name must be at least 1 character in length.";

		iu_errors.AddError(KORC_INVALID_FILE_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidFileName:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}





//---------------------------------------------------------------------+
//  Method:   IsValidQueueMgrName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Queue Manager Name. 
//
//            Name must not be NULL.
//            Name length must be >= 1 and <= 48.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidQueueMgrName() {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_szQueMgrName == NULL) {

		lb_return = FALSE;

		ls_msg << "Required Queue Manager Name was not provided.";

		iu_errors.AddError(KORC_INVALID_QMGR_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidQueueMgrName:1",
						   is_InstanceName.getPointer());

	}
	else if ((strlen(m_szQueMgrName) < 1)   || 
		     (strlen(m_szQueMgrName) > 48))     {

		lb_return = FALSE;

		ls_msg << "Queue Manager Name '"
			   << (char*) m_szQueMgrName
			   << "' must be between 1 and 48 characters.";

		iu_errors.AddError(KORC_INVALID_QMGR_NAME, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidQueueMgrName:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}







//---------------------------------------------------------------------+
//  Method:   IsValidDataQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name by calling 
//            IsValidQueueName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidDataQueueName() {

	return IsValidQueueName((char*) m_szDataQueName, "Data"); 
}








//---------------------------------------------------------------------+
//  Method:   IsValidCtrlQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name by calling 
//            IsValidQueueName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidCtrlQueueName() {

	return IsValidQueueName((char*) m_szCtlQueName, "Control");
}








//---------------------------------------------------------------------+
//  Method:   IsValidErrorQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name by calling 
//            IsValidQueueName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidErrorQueueName() {

	return IsValidQueueName((char*) m_szErrQueName, "Error");
}






//---------------------------------------------------------------------+
//  Method:   IsValidReplyToQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name by calling 
//            IsValidQueueName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidReplyToQueueName() {

	return IsValidQueueName((char*) m_szReplyToQName, "Reply To");
}








//---------------------------------------------------------------------+
//  Method:   IsValidDeadLetterQueueName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided queue name by calling 
//            IsValidQueueName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidDeadLetterQueueName() {

	return IsValidQueueName((char*) m_szDeadLetterQueName, "Dead Letter");
}





//---------------------------------------------------------------------+
//  Method:   IsValidDataFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided file name by calling 
//            IsValidFileName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidDataFileName() {

	return IsValidFileName((char*) m_szDataFileName, "Data");
}






//---------------------------------------------------------------------+
//  Method:   IsValidCtrlFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided file name by calling 
//            IsValidFileName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidCtrlFileName() {

	return IsValidFileName((char*) m_szCtlFileName, "Control");
}








//---------------------------------------------------------------------+
//  Method:   IsValidLogFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided file name by calling 
//            IsValidFileName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidLogFileName() {

	return IsValidFileName((char*) m_szLogFileName, "Log");
}








//---------------------------------------------------------------------+
//  Method:   IsValidRestartFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided file name by calling 
//            IsValidFileName.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidRestartFileName() {

	return IsValidFileName((char*) m_szRestartFileName, "Restart");
}








//---------------------------------------------------------------------+
//  Method:   IsValidBufferSize
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Buffer Size. 
//
//            Buffer Size must be >= 1 and <= KO_MAX_BUFFER_SIZE.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidBufferSize() {
	
	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if ((m_lBufferSize < 1) || (m_lBufferSize > KO_MAX_BUFFER_SIZE)) {

		lb_return = FALSE;

		ls_msg << "Buffer Size " << m_lBufferSize
			   << " is invalid.  Buffer Size must be between 1 and "
			   << KO_MAX_BUFFER_SIZE 
			   << ".";

		iu_errors.AddError(KORC_INVALID_BUFFER_SIZE, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidBufferSize:1",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}






//---------------------------------------------------------------------+
//  Method:   IsValidBatchInterval
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Batch Interval. 
//
//            Batch Interval must be >= 0.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidBatchInterval() {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_lBatchInterval < 0) {

		lb_return = FALSE;

		ls_msg << "Batch Interval must be greater than or equal to zero.";

		iu_errors.AddError(KORC_INVALID_BATCH_INTERVAL, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidBatchInterval:1",
						   is_InstanceName.getPointer());

	}

	return lb_return;
}








//---------------------------------------------------------------------+
//  Method:   IsValidMsgID
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Message ID. 
//
//            Name must not be NULL, unless vb_AllowNull is TRUE.
//            Name length must be >= 1 and <= 24.
//
//            Note that for PUTs, Msg ID must be no more than 10 
//            characters in order to append a timestamp.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     vb_AllowNull	TRUE  if Null is a valid value (ie for GETS
//                                when you want to get whatever's there.
//
//                          TRUE  is the default value.
//
//                          FALSE if Null is not valid (ie for PUTS).
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidMsgID(BOOL vb_AllowNull) {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_szMsgID == NULL) {

		if(!vb_AllowNull) {

			lb_return = FALSE;

			ls_msg << "Required Message ID was not provided.";

			iu_errors.AddError(KORC_INVALID_MSG_ID, KOCC_FAIL,
							   ls_msg,
							   "CStartParamFile::IsValidMsgID:1",
							   is_InstanceName.getPointer());
		}

	}
	else if ((strlen(m_szMsgID) < 1) || (strlen(m_szMsgID) > 24)) {
		lb_return = FALSE;

		ls_msg << "Message ID '"
			   << (char*) m_szMsgID
			   << "' must be between 1 and 24 characters.";

		iu_errors.AddError(KORC_INVALID_MSG_ID, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidMsgID:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;
}







//---------------------------------------------------------------------+
//  Method:   IsValidExpirationInterval
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Expiration Interval. 
//
//            Expiration Interval must be > 0.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidExpirationInterval() {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_lExpirationInterval < 1) {

		lb_return = FALSE;

		ls_msg << "Expiration Interval must be greater than zero.";

		iu_errors.AddError(KORC_INVALID_EXPIRE, KOCC_FAIL,
					ls_msg,
					"CStartParamFile::IsValidExpirationInterval:1",
					is_InstanceName.getPointer());

	}

	return lb_return;

}







//---------------------------------------------------------------------+
//  Method:   IsValidMessageFormat
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Message Format. 
//
//            Message Format must not be NULL.
//            Message Format must be "MQSTR" or "MQHSAP".
//
//            Note:  Real MQ Series format field is an 8 byte field
//                   padded on the right with trailing spaces.
//
//                   "MQDEAD  " is the "real" format code for a message
//                              with a Dead Letter Header.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidMessageFormat() {


	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_szMessageFormat == NULL) {

		lb_return = FALSE;

		ls_msg << "Required Message Format was not provided.";

		iu_errors.AddError(KORC_INVALID_MSG_FORMAT, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidMessageFormat:1",
						   is_InstanceName.getPointer());

	}
	else if ((strcmp(m_szMessageFormat, KOFMT_STRING ) != 0) && 
		     (strcmp(m_szMessageFormat, KOFMT_SAP    ) != 0) &&
			 (strcmp(m_szMessageFormat, KOFMT8_STRING) != 0) &&
			 (strcmp(m_szMessageFormat, KOFMT8_SAP   ) != 0)) {

		lb_return = FALSE;

		ls_msg << "Message Format '"
			   << (char*) m_szMessageFormat
			   << "' is not a recognized message format.";

		iu_errors.AddError(KORC_INVALID_MSG_FORMAT, KOCC_FAIL,
						   ls_msg,
						   "CStartParamFile::IsValidMessageFormat:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}




#ifdef _ver2_
//---------------------------------------------------------------------+
//  Method:   IsValidReceiptRequest
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Validates combined parameter setup for returning receipts. 
//
//            If ReturnReceipt != None then KeyTranslation=1 is required 
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  03/21/99 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::IsValidReceiptRequest() {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if ((isDataReceipt() || isLogReceipt()) && !isTranslate()) {

		lb_return = FALSE;

		ls_msg << "Return Receipt processing requires Key Translation ON.";

		iu_errors.AddError(KORC_INVALID_PARM_COMBINATION, KOCC_FAIL,
					ls_msg,
					"CStartParamFile::IsValidReceiptRequest:1",
					is_InstanceName.getPointer());

	}

	return lb_return;

}
#endif





//---------------------------------------------------------------------+
//  Method:   CheckPassword
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     Returns true if a password was entered in the
//            parameter file and if it is correct.
//
//            Returns false if they do not match, or if one was not
//            entered.
//            
//
//  Args:     None
//
//  Returns:  TRUE  if password entered and correct
//            FALSE if no password was entered or if it is not correct.
//  
//  Dev Log:  This password stuff could be a separate class, but for
//            now it's only needed to validate a password parameter
//            and determine if Admin Access is allowed.
//
//            Consider breaking out into a separate class in the future.
//
//  History: 
//  03/31/99  DMT Created original version.
//---------------------------------------------------------------------+
BOOL CStartParamFile::CheckPassword() {

	long	ll_return = KOCC_GOOD;
	BOOL	b_return  = FALSE;

	CString	ls_good_pwd;

	if (m_szPassword != NULL) {

		ll_return = CalcPassword(ls_good_pwd);

		if (ll_return == KOCC_GOOD) {

			b_return = (ls_good_pwd == (char*) m_szPassword);

			if(!b_return) {

				//Set Error?
				iu_errors.AddError(KORC_ADMIN_ACCESS_DENIED, KOCC_FAIL,
					"Administrator Access Denied!",
					"CStartParamFile::CheckPassword:1",
					is_InstanceName.getPointer());

			}

		}
	}

	return b_return;

}
		



