/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) ExtSrvcsParamFile.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     ExtSrvcsParamFile.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     This file contains the implementation of the 
//            CExtSrvcsParamFile Class.
//  
//            This class represents a Parameter File and is derived from
//            CParamFile.
//  
//  Dev Log:  
//
//  History: 
//  11/20/00 DRC Created original version.
//---------------------------------------------------------------------+
#include "ExtSrvcsParamFile.h"

      long  CExtSrvcsParamFile::m_lAppMode;
const char* CExtSrvcsParamFile::m_szBlocking;
      long  CExtSrvcsParamFile::m_lBufferSize;
      char* CExtSrvcsParamFile::m_szCtlFileName;
const char* CExtSrvcsParamFile::m_szCtlQueName;
      char* CExtSrvcsParamFile::m_szDataFileName;
const char* CExtSrvcsParamFile::m_szDataQueName;
      long  CExtSrvcsParamFile::m_lCtrlFilesProcessed;
      long  CExtSrvcsParamFile::m_lDataFilesProcessed;
const char* CExtSrvcsParamFile::m_szDeblocking;
      long  CExtSrvcsParamFile::m_lDecode;
      long  CExtSrvcsParamFile::m_lDecrypt;
      long  CExtSrvcsParamFile::m_lEncode;
      long  CExtSrvcsParamFile::m_lEncrypt;
const char* CExtSrvcsParamFile::m_szCipherKey1;
const char* CExtSrvcsParamFile::m_szCipherKey2;
const char* CExtSrvcsParamFile::m_szCipherKey3;
const char* CExtSrvcsParamFile::m_szFileClassList;
      char* CExtSrvcsParamFile::m_szFileArchive;
      char* CExtSrvcsParamFile::m_szFileDisp;
      char* CExtSrvcsParamFile::m_szLogFileName;
const char* CExtSrvcsParamFile::m_szMsgID;
      long  CExtSrvcsParamFile::m_lMode;
const char* CExtSrvcsParamFile::m_szOperation;
const char* CExtSrvcsParamFile::m_szParmFileName;
const char* CExtSrvcsParamFile::m_szQueMgrName;
	  char* CExtSrvcsParamFile::m_szRestartFileName;
      char* CExtSrvcsParamFile::m_szSrcFileMask;
      char* CExtSrvcsParamFile::m_szDstDir;
      long  CExtSrvcsParamFile::m_lSubDirSrch;
      long  CExtSrvcsParamFile::m_lRcrdLen;
      char* CExtSrvcsParamFile::m_szSetName;
      char* CExtSrvcsParamFile::m_szSetClsName;
      char* CExtSrvcsParamFile::m_szDataClsName;
	  char* CExtSrvcsParamFile::m_szGetProcessor;
	  char* CExtSrvcsParamFile::m_szPutProcessor;
	  char* CExtSrvcsParamFile::m_szOnSuccessCmd;
	  char* CExtSrvcsParamFile::m_szOnFailCmd;




//---------------------------------------------------------------------+
//  Table that defined parameters in the Parameter File
//
//	NULL/TypeInvalid entry indicates end of table.
//---------------------------------------------------------------------+
CExtSrvcsParamFile::SParamTable CExtSrvcsParamFile::sParamTable[] = {
//
//	Argument Name		   Argument Type  Ptr to storage        Req?   Loaded?
//
	{ "AppMode",            TypeLong,   &m_lAppMode,            FALSE, FALSE },
	{ "Blocking",			TypeString,	&m_szBlocking,			FALSE, FALSE },
	{ "BufferSize",			TypeLong,	&m_lBufferSize,		    TRUE,  FALSE },
	{ "ControlFileName",	TypeString, &m_szCtlFileName,	    FALSE, FALSE },
	{ "ControlQueueName",	TypeString, &m_szCtlQueName, 	    FALSE, FALSE },
	{ "DataFileName",		TypeString, &m_szDataFileName,	    FALSE, FALSE },
	{ "DataClass",			TypeString, &m_szDataClsName,	    FALSE, FALSE },
	{ "DataQueueName",		TypeString, &m_szDataQueName, 	    FALSE, FALSE },
	{ "Deblocking",			TypeString,	&m_szDeblocking,		FALSE, FALSE },
	{ "Decode",				TypeLong,	&m_lDecode,				FALSE, FALSE },
	{ "Decrypt",			TypeLong,	&m_lDecrypt,			FALSE, FALSE },
	{ "DestDir",			TypeString, &m_szDstDir,		    FALSE, FALSE },
	{ "Encode",				TypeLong,	&m_lEncode,				FALSE, FALSE },
	{ "Encrypt",			TypeLong,	&m_lEncrypt,			FALSE, FALSE },
	{ "EncryptionKey1",		TypeString, &m_szCipherKey1,	    FALSE, FALSE },
	{ "EncryptionKey2",		TypeString, &m_szCipherKey2,	    FALSE, FALSE },
	{ "EncryptionKey3",		TypeString, &m_szCipherKey3,	    FALSE, FALSE },
	{ "FileClassList",		TypeString, &m_szFileClassList,		FALSE, FALSE },
	{ "FileArchive",		TypeString, &m_szFileArchive,	    FALSE, FALSE },
	{ "FileDisp",			TypeString, &m_szFileDisp,			FALSE, FALSE },
	{ "LogFileName",		TypeString, &m_szLogFileName,	    TRUE,  FALSE },
	{ "MsgID",              TypeString, &m_szMsgID,             FALSE, FALSE },
	{ "Mode",               TypeLong,   &m_lMode,               FALSE, FALSE },
	{ "Operation",			TypeString, &m_szOperation,			TRUE,  FALSE },
	{ "ParmFile",			TypeString, &m_szParmFileName,	    FALSE, FALSE },
	{ "QMgrName",			TypeString, &m_szQueMgrName,	    FALSE, FALSE },
	{ "SetClass",			TypeString, &m_szSetClsName,	    FALSE, FALSE },
	{ "SetName",			TypeString, &m_szSetName,		    FALSE, FALSE },
	{ "SourceFiles",		TypeString, &m_szSrcFileMask,	    FALSE, FALSE },
	{ "SubDirectorySearch",	TypeLong,	&m_lSubDirSrch,		    FALSE, FALSE },
	{ "RecordLength",		TypeLong,	&m_lRcrdLen,		    FALSE, FALSE },
	{ "GetProcessor",		TypeString, &m_szGetProcessor,	    FALSE, FALSE },
	{ "PutProcessor",		TypeString, &m_szPutProcessor,	    FALSE, FALSE },
	{ "OnSuccessCmd",		TypeString, &m_szOnSuccessCmd,	    FALSE, FALSE },
	{ "OnFailCmd",			TypeString, &m_szOnFailCmd,	        FALSE, FALSE },
	{ NULL,					TypeInvalid, NULL,				    FALSE, FALSE }
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
const CExtSrvcsParamFile::SParamTable* CExtSrvcsParamFile::GetParamTable()
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
void CExtSrvcsParamFile::LoadParams(CCmdArgs& args)
{
	long	ll_return = KOCC_GOOD;

	Open(args.GetParamFileName());

	if(!iu_errors.HasError()) {
		CParamFile::Load();
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
void CExtSrvcsParamFile::initMembers()
{
    m_szBlocking			= NULL;
	m_lBufferSize			= NULL;
    m_szCtlFileName			= NULL;
    m_szCtlQueName			= NULL;
    m_szDataFileName		= NULL;
	SetString(&m_szDataClsName,"[DefaultClass]");
    m_szDataQueName			= NULL;
    m_szDeblocking			= NULL;
	m_lDecode               = 0;
	m_lDecrypt              = 0;
    m_szDstDir  			= NULL;
	m_lEncode               = 0;
	m_lEncrypt              = 0;
    m_szCipherKey1			= NULL;
    m_szCipherKey2			= NULL;
    m_szCipherKey3			= NULL;
    m_szFileClassList		= NULL;
    m_szFileArchive			= NULL;
    m_szFileDisp			= NULL;
    m_szLogFileName			= NULL;
    m_szOperation			= NULL;
    m_szParmFileName		= NULL;
    m_szQueMgrName			= NULL;
	m_szRestartFileName     = NULL;
	SetString(&m_szSetName,"[DefaultSet]");
	SetString(&m_szSetClsName,"[DefaultSet]");
    m_szSrcFileMask			= NULL;
    m_szGetProcessor 	    = NULL;
    m_szPutProcessor 	    = NULL;
    m_szOnSuccessCmd		= NULL;
    m_szOnFailCmd			= NULL;

	m_lSubDirSrch           = 0;
	m_lRcrdLen              = 0;

	m_szMsgID				= NULL;
	m_lMode					= 1;
	m_lAppMode              = 0;

	m_lDataFilesProcessed	= 0;
	m_lCtrlFilesProcessed	= 0;
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
CExtSrvcsParamFile::CExtSrvcsParamFile() : CParamFile() 
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
CExtSrvcsParamFile::CExtSrvcsParamFile(CCmdArgs& args) : CParamFile(args.GetParamFileName())
{
	initMembers();
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
CExtSrvcsParamFile::~CExtSrvcsParamFile()
{
	// If not NULL, delete free's the memory these pointers to 
	// strings reference.
	delete [] (char *) m_szLogFileName;
	m_szLogFileName = NULL;

	delete [] (char *) m_szDataFileName;
	m_szDataFileName = NULL;

	delete [] (char *) m_szCtlFileName;
	m_szCtlFileName = NULL;

	delete [] (char *) m_szQueMgrName;
	m_szQueMgrName = NULL;

	delete [] (char *) m_szCtlQueName;
	m_szCtlQueName = NULL;

	delete [] (char *) m_szDataQueName;
	m_szDataQueName = NULL;

	delete [] (char *) m_szBlocking;
	m_szBlocking = NULL;

	delete [] (char *) m_szDeblocking;
	m_szDeblocking = NULL;

	delete [] (char *) m_szDstDir;
	m_szDstDir = NULL;

	delete [] (char *) m_szCipherKey1;
	m_szCipherKey1 = NULL;

	delete [] (char *) m_szCipherKey2;
	m_szCipherKey2 = NULL;

	delete [] (char *) m_szCipherKey3;
	m_szCipherKey3 = NULL;

	delete [] (char *) m_szFileClassList;
	m_szFileClassList = NULL;

	delete [] (char *) m_szFileArchive;
	m_szFileArchive = NULL;

	delete [] (char *) m_szFileDisp;
	m_szFileDisp = NULL;

	delete [] (char *) m_szOperation;
	m_szOperation = NULL;

	delete [] (char *) m_szMsgID;
	m_szMsgID		 = NULL;

	delete [] (char *) m_szParmFileName;
	m_szParmFileName = NULL;

	delete [] (char *) m_szRestartFileName;
	m_szRestartFileName = NULL;

	delete [] (char *) m_szSrcFileMask;
	m_szSrcFileMask = NULL;

	delete [] (char *) m_szSetName;
	m_szSetName = NULL;

	delete [] (char *) m_szSetClsName;
	m_szSetClsName = NULL;

	delete [] (char *) m_szDataClsName;
	m_szDataClsName = NULL;

	delete [] (char *) m_szGetProcessor;
	m_szGetProcessor = NULL;

	delete [] (char *) m_szPutProcessor;
	m_szPutProcessor = NULL;

	delete [] (char *) m_szOnSuccessCmd;
	m_szOnSuccessCmd = NULL;

	delete [] (char *) m_szOnFailCmd;
	m_szOnFailCmd = NULL;

}



//---------------------------------------------------------------------+
//  Method:   IsFileDispXXXX
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Determine the File Disposition. 
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CExtSrvcsParamFile::IsFileDispArchive() {
BOOL l_bResult = false;

if (!m_szFileDisp == NULL){
	(!strncmp(m_szFileDisp, "Archive", 7)) ? l_bResult = true : 0;
}  /* End If */
return l_bResult;
} /* End IsFileDispArchive */



BOOL CExtSrvcsParamFile::IsFileDispDelete() {
BOOL l_bResult = false;

if (!m_szFileDisp == NULL){
	(!strncmp(m_szFileDisp, "Delete", 6)) ? l_bResult = true : 0;
}  /* End If */
return l_bResult;
} /* End IsFileDispDelete */



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
BOOL CExtSrvcsParamFile::IsValidBufferSize() {
	
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
						   "CExtSrvcsParamFile::IsValidBufferSize:1",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}




//---------------------------------------------------------------------+
//  Method:   IsValidDisp
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Validates FileDisp and FileArchive 
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CExtSrvcsParamFile::IsValidDisp() {
BOOL l_bResult = true;

switch (toupper(*m_szFileDisp)){
case 'K':                      // FileDisp=Keep
	break;

case 'D':                      // FileDisp=Delete
	break;

case 'A':                      // FileDisp=Archive
	l_bResult = IsValidFileName((char*) m_szFileArchive, "Archive"); 
	break;

default: 
	l_bResult = false;

} /* End Switch */
return l_bResult;
}  /* end isValidDisp */



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
BOOL CExtSrvcsParamFile::IsValidQueueMgrName() {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (m_szQueMgrName == NULL) {

		lb_return = FALSE;

		ls_msg << "Required Queue Manager Name was not provided.";

		iu_errors.AddError(KORC_INVALID_QMGR_NAME, KOCC_FAIL,
						   ls_msg,
						   "CExtSrvcsParamFile::IsValidQueueMgrName:1",
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
						   "CExtSrvcsParamFile::IsValidQueueMgrName:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}




//---------------------------------------------------------------------+
//  Method:   IsValidCtlQueueName
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
BOOL CExtSrvcsParamFile::IsValidDataQueueName() {

	return IsValidQueueName((char*) m_szCtlQueName, "Data"); 
}





//---------------------------------------------------------------------+
//  Method:   IsValidCtlQueueName
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
BOOL CExtSrvcsParamFile::IsValidCtlQueueName() {

	return IsValidQueueName((char*) m_szCtlQueName, "Control"); 
}




	
//---------------------------------------------------------------------+
//  Method:   IsValidLogFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Log File Name. 
//
//            Name must not be NULL.
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
BOOL CExtSrvcsParamFile::IsValidLogFileName() {

	return IsValidFileName((char*) m_szLogFileName, "Log"); 
}





//---------------------------------------------------------------------+
//  Method:   IsValidDataFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Log File Name. 
//
//            Name must not be NULL.
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
BOOL CExtSrvcsParamFile::IsValidDataFileName() {

	return IsValidFileName((char*) m_szDataFileName, "Data"); 
}




//---------------------------------------------------------------------+
//  Method:   IsValidCtlFileName
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Validates provided Control File Name. 
//
//            Name must not be NULL.
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
BOOL CExtSrvcsParamFile::IsValidCtlFileName() {

	return IsValidFileName((char*) m_szCtlFileName, "Control"); 
}



		
//---------------------------------------------------------------------+
//  Method:   IsValidParmFile
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Validates provided Parameter File Name. 
//
//            Name must not be NULL.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CExtSrvcsParamFile::IsValidParmFile() {

	return IsValidFileName((char*) m_szParmFileName, "Parameter Template"); 
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
BOOL CExtSrvcsParamFile::IsValidQueueName(char* szName, char* szContext) {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (szName == NULL) {
		lb_return = FALSE;

		ls_msg << "Required "
			   << szContext
			   << " Queue Name was not provided.";

		iu_errors.AddError(KORC_INVALID_Q_NAME, KOCC_FAIL,
						   ls_msg,
						   "CExtSrvcsParamFile::IsValidQueueName:1",
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
						   "CExtSrvcsParamFile::IsValidQueueName:2",
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
BOOL CExtSrvcsParamFile::IsValidFileName(char* szName, char* szContext) {

	BOOL	lb_return = TRUE;
	CString	ls_msg;


	if (szName == NULL) {
		lb_return = FALSE;

		ls_msg << "Required "
			   << szContext
			   << " File Name was not provided.";

		iu_errors.AddError(KORC_INVALID_FILE_NAME, KOCC_FAIL,
						   ls_msg,
						   "CExtSrvcsParamFile::IsValidFileName:1",
						   is_InstanceName.getPointer());

	}
	else if ((strlen(szName) < 1)) {
		lb_return = FALSE;

		ls_msg << szContext
			   << " File Name must be at least 1 character in length.";

		iu_errors.AddError(KORC_INVALID_FILE_NAME, KOCC_FAIL,
						   ls_msg,
						   "CExtSrvcsParamFile::IsValidFileName:2",
						   is_InstanceName.getPointer());

	}

	return lb_return;

}





//---------------------------------------------------------------------+
//  Method:   IsValidCipherKey
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Validates the three segments of the Cipher Key. 
//
//            Key must not be NULL.
//            Key length must be == 8.
//
//            Adds Error to Error Collection if not valid.
//
//  Args:     None.
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log:  
//
//  History: 
//  11/20/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CExtSrvcsParamFile::IsValidCipherKey() {
BOOL l_bResult = true;
//
//  Three whole key segments must be provided
//
(m_szCipherKey1 == NULL) ? l_bResult = false : 0;
(m_szCipherKey2 == NULL) ? l_bResult = false : 0;
(m_szCipherKey3 == NULL) ? l_bResult = false : 0;

(strlen(m_szCipherKey1) != 8) ? l_bResult = false : 0;
(strlen(m_szCipherKey2) != 8) ? l_bResult = false : 0;
(strlen(m_szCipherKey3) != 8) ? l_bResult = false : 0;

return l_bResult;
}





BOOL CExtSrvcsParamFile::IsValidSrcFileMask(){
return IsValidFileName((char*) m_szSrcFileMask, "Search Mask"); 
} 




//---------------------------------------------------------------------+
//  Method:   IsXxxxxSpecified
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     These methods return specific attribute conditions and 
//            encapsulate the knowledge of the individual values that 
//            are stored to indicate those conditions within the  
//            ExtSrvcsParamFile class.
//  
//  Args:     None.
//
//  Returns:  BOOL - true if valid, false if not.
//  
//  Dev Log: 
//
//  History: 
//  11/20/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CExtSrvcsParamFile::IsEncodingSpecified(){ 
	return (m_lEncode == 1);
}

BOOL CExtSrvcsParamFile::IsDecodingSpecified(){ 
	return (m_lDecode == 1);
}

BOOL CExtSrvcsParamFile::IsEncryptionSpecified(){ 
	return (m_lEncrypt == 1);
}

BOOL CExtSrvcsParamFile::IsDecryptionSpecified(){ 
	return (m_lDecrypt == 1);
}

BOOL CExtSrvcsParamFile::IsBlockingSpecified(){
	BOOL l_bResult = false;
	
	if (!m_szBlocking == NULL){
		(!strncmp(m_szBlocking, "Fixed", 5))    ? l_bResult = true : 0;
		(!strncmp(m_szBlocking, "Variable", 8)) ? l_bResult = true : 0;
	}  /* End If */
	return l_bResult;
}

BOOL CExtSrvcsParamFile::IsDeblockingSpecified(){
	BOOL l_bResult = false;
	
	if (!m_szDeblocking == NULL){
		(!strncmp(m_szDeblocking, "Fixed", 5))    ? l_bResult = true : 0;
		(!strncmp(m_szDeblocking, "Variable", 8)) ? l_bResult = true : 0;
	}  /* End If */
	return l_bResult;
}

BOOL CExtSrvcsParamFile::IsSubDirSrchSpecified(){
	return (m_lSubDirSrch == 1);
}
	

BOOL CExtSrvcsParamFile::IsFileSetSpecified(){
	return ((m_szSetName != NULL) || (m_szSetClsName != NULL));
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
void CExtSrvcsParamFile::Load(CCmdArgs& args)
{
	LoadParams(args);
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
const char* CExtSrvcsParamFile::GetLogFileName()
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
const char* CExtSrvcsParamFile::GetCtlFileName()
{
	return m_szCtlFileName;
}


long CExtSrvcsParamFile::SetCtlFileName(char* a_szArg){
	SetString(&m_szCtlFileName, a_szArg);
	return KOCC_GOOD;
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
const char* CExtSrvcsParamFile::GetDataFileName()
{
	return m_szDataFileName;
}


long CExtSrvcsParamFile::SetDataFileName(char* a_szArg){
	SetString(&m_szDataFileName, a_szArg);
	return KOCC_GOOD;
}


long CExtSrvcsParamFile::SetLogFileName(char* a_szArg){
	SetString(&m_szLogFileName, a_szArg);
	return KOCC_GOOD;
}


char*  CExtSrvcsParamFile::GetParmFileName(){
	return (char*) m_szParmFileName;
}



long CExtSrvcsParamFile::SetGetProcessor(char* a_szArg){
	SetString(&m_szGetProcessor, a_szArg);
	return KOCC_GOOD;
}



char* CExtSrvcsParamFile::GetGetProcessor(){
	return (char*) m_szGetProcessor;
}



long CExtSrvcsParamFile::SetPutProcessor(char* a_szArg){
	SetString(&m_szPutProcessor, a_szArg);
	return KOCC_GOOD;
}



char* CExtSrvcsParamFile::GetPutProcessor(){
	return (char*) m_szPutProcessor;
}



long CExtSrvcsParamFile::SetOnSuccessCmd(char* a_szArg){
	SetString(&m_szOnSuccessCmd, a_szArg);
	return KOCC_GOOD;
}



char* CExtSrvcsParamFile::GetOnSuccessCmd(){
	return (char*) m_szOnSuccessCmd;
}




long CExtSrvcsParamFile::SetOnFailCmd(char* a_szArg){
	SetString(&m_szOnFailCmd, a_szArg);
	return KOCC_GOOD;
}



char* CExtSrvcsParamFile::GetOnFailCmd(){
	return (char*) m_szOnFailCmd;
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
const char* CExtSrvcsParamFile::GetQueMgrName()
{
	return m_szQueMgrName;
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
const char* CExtSrvcsParamFile::GetCtlQueName()
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
const char* CExtSrvcsParamFile::GetDataQueName()
{
	return m_szDataQueName;
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
const long CExtSrvcsParamFile::GetBufferSize()
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
void CExtSrvcsParamFile::SetBufferSize(long vl_value) {
	m_lBufferSize = vl_value;
}



int CExtSrvcsParamFile::GetOperation(){
int l_iOprCode = NoOpr;
 
(!strncmp(m_szOperation, "Put",15))          ? l_iOprCode = PutFiles   : 0;
(!strncmp(m_szOperation, "PutFile",15))      ? l_iOprCode = PutFiles   : 0;
(!strncmp(m_szOperation, "PutFiles",15))     ? l_iOprCode = PutFiles   : 0;
(!strncmp(m_szOperation, "Get",15))          ? l_iOprCode = GetFiles   : 0;
(!strncmp(m_szOperation, "GetFile",15))      ? l_iOprCode = GetFiles   : 0;
(!strncmp(m_szOperation, "GetFiles",15))     ? l_iOprCode = GetFiles   : 0;
(!strncmp(m_szOperation, "Update",15))       ? l_iOprCode = UpdateFile : 0;
(!strncmp(m_szOperation, "UpdateFile",15))   ? l_iOprCode = UpdateFile : 0;
(!strncmp(m_szOperation, "UpdateFiles",15))  ? l_iOprCode = UpdateFile : 0;
return l_iOprCode;
}





//---------------------------------------------------------------------+
//  Method:   GetRecordLength
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Expose the RcrdLen attribute
//
//  Args:     None
//
//  Returns:  long (Record Length)
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtSrvcsParamFile::GetRecordLength()
{
	return m_lRcrdLen;
}


//---------------------------------------------------------------------+
//  Method:   SetRecordLength
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Expose the RcrdLen attribute
//
//  Args:     long ( Record Length)
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CExtSrvcsParamFile::SetRecordLength(long vl_value) {
	m_lRcrdLen = vl_value;
}



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
//---------------------------------------------------------------------+
void CExtSrvcsParamFile::show(CLog& a_pcLog, int a_iEntryType){

	CString l_sLogRcrd(100);

	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Parameter File Values from '" 
		   << m_szFileName 
		   << "'.", a_iEntryType);
    
    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Operation Requested   : " 
		   <<  (char*) valid_str(m_szOperation), a_iEntryType);

	l_sLogRcrd =  "Log file name         : "; 
	l_sLogRcrd += (char*) valid_str(m_szLogFileName);
	a_pcLog.WriteLog(l_sLogRcrd, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Parameter Template    : " 
		   <<  (char*) valid_str(m_szParmFileName), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Source Files          : " 
		   <<  (char*) valid_str(m_szSrcFileMask), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Message ID            : " 
		   << (char*) valid_str(m_szMsgID), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Sub-Directory Search  : " 
		   << (char*) valid_bool(m_lSubDirSrch), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Destination Directory : " 
		   << (char*) valid_str(m_szDstDir), a_iEntryType);

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
		   << "File Disposition      : " 
		   << (char*) valid_str(m_szFileDisp), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Archive Directory     : " 
		   << (char*) valid_str(m_szFileArchive), a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd, a_iEntryType);       //  blank line 

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Buffer size           : " 
		   << m_lBufferSize, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Record Length         : " 
		   << m_lRcrdLen, a_iEntryType);

    l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Mode                  : " 
		   << m_lMode, a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Blocking              : " 
		   << (char*) valid_str(m_szBlocking), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Deblocking            : " 
		   << (char*) valid_str(m_szDeblocking), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Data Class            : " 
		   << (char*) valid_str(m_szDataClsName), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Set Class             : " 
		   << (char*) valid_str(m_szSetClsName), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Set Name              : " 
		   << (char*) valid_str(m_szSetName), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Get Processor         : " 
		   << (char*) valid_str(m_szGetProcessor), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "Put Processor         : " 
		   << (char*) valid_str(m_szPutProcessor), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "OnSuccessCommand      : " 
		   << (char*) valid_str(m_szOnSuccessCmd), a_iEntryType);

	l_sLogRcrd.fill();
	a_pcLog.WriteLog(l_sLogRcrd 
		   << "OnFailCommand         : " 
		   << (char*) valid_str(m_szOnFailCmd), a_iEntryType);


}  /* End Method */




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
const long CExtSrvcsParamFile::GetMode() {
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
void CExtSrvcsParamFile::SetMode(long vl_value) {
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
const long CExtSrvcsParamFile::GetAppMode() {
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
void CExtSrvcsParamFile::SetAppMode(long vl_value) {
	m_lAppMode = vl_value;
}






//---------------------------------------------------------------------+
//  Method:   GetDataFilesProcessed
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
long CExtSrvcsParamFile::GetDataFilesProcessed() {
		return m_lDataFilesProcessed;
}
long CExtSrvcsParamFile::GetFilesLocated() {
		return m_lDataFilesProcessed;
}



//---------------------------------------------------------------------+
//  Method:   SetDataFilesProcessed
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
void CExtSrvcsParamFile::SetDataFilesProcessed(long vl_value) {
	m_lDataFilesProcessed = vl_value;
}
void CExtSrvcsParamFile::SetFilesLocated(long vl_value) {
	m_lDataFilesProcessed = vl_value;
}



//---------------------------------------------------------------------+
//  Method:   GetCtrlFilesProcessed
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
long CExtSrvcsParamFile::GetCtrlFilesProcessed() {
	return m_lCtrlFilesProcessed;
}
long CExtSrvcsParamFile::GetFilesProcessed() {
	return m_lCtrlFilesProcessed;
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
void CExtSrvcsParamFile::SetCtrlFilesProcessed(long vl_value) {
	m_lCtrlFilesProcessed = vl_value;
}
void CExtSrvcsParamFile::SetFilesProcessed(long vl_value) {
	m_lCtrlFilesProcessed = vl_value;
}




//---------------------------------------------------------------------+
//  Method:   SetRestartFileName
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     char* (Restart File Name)
//
//  Returns:  long (Return Code)
//            00 = Successful function
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtSrvcsParamFile::SetRestartFileName(char* a_szArg){
	SetString(&m_szRestartFileName, a_szArg);
	return KOCC_GOOD;
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
char* CExtSrvcsParamFile::GetMessageID() {
	return (char*) m_szMsgID;
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
long CExtSrvcsParamFile::SetMessageID(char* szValue) {
	SetString((char**) &m_szMsgID, szValue);
	return KOCC_GOOD;
}




//---------------------------------------------------------------------+
//  Method:   GetTotalFilesProcessed
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
long CExtSrvcsParamFile::GetTotalFilesProcessed() {
	return (m_lCtrlFilesProcessed + m_lDataFilesProcessed);
}




//---------------------------------------------------------------------+
//  Method:   GetRestartFileName
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Source File Mask)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetRestartFileName(void){
	return m_szRestartFileName;
}





//---------------------------------------------------------------------+
//  Method:   GetSrcFileMask
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Source File Mask)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetSrcFileMask(){
	return m_szSrcFileMask;
}


//---------------------------------------------------------------------+
//  Method:   GetFileArchive
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Source File Mask)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetFileArchive(){
	return m_szFileArchive;
}


//---------------------------------------------------------------------+
//  Method:   GetFileDisp
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Source File Mask)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetFileDisp(){
	return m_szFileDisp;
}


//---------------------------------------------------------------------+
//  Method:   GetDstDir
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Source File Mask)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetDstDir(){
	return m_szDstDir;
}


//---------------------------------------------------------------------+
//  Method:   GetSetName
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Set Name)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetSetName(){
	return m_szSetName;
}


//---------------------------------------------------------------------+
//  Method:   GetSetClsName
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Set Class Name)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetSetClsName(){
	return m_szSetClsName;
}


//---------------------------------------------------------------------+
//  Method:   GetDataClsName
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Private Member Variable Get/Set Method
//
//  Args:     None
//
//  Returns:  char* (Data Class Name)
//            NULL if No Source File Mask Specified
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CExtSrvcsParamFile::GetDataClsName(){
	return m_szDataClsName;
}





//---------------------------------------------------------------------+
//  Method:   WriteParmFile
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Opens Parameter File
//            Streams Out Parameter Info
//            Closes Parameter File
//
//  Args:     None
//
//  Returns:  long	KOCC_GOOD if okay
//                  KOCC_FAIL if an error occurs.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CExtSrvcsParamFile::WriteParmFile(char* a_szSrcFileName,
									   char* a_szDstFileName) {

long	 ll_return = KOCC_GOOD;
CString	 ls_msg;
CString	 ls_errno_desc;

CFile     l_SrcFile;
char      l_szBuffer[100];

//
//  Copy the Template Parameter File into the Generated File
//

    l_SrcFile.open(a_szSrcFileName, ios::in || ios::nocreate);
	open(a_szDstFileName, ios::out);

	if ((l_SrcFile.is_open()) 
	&& (this->is_open())){
		l_SrcFile.getline(l_szBuffer, sizeof l_szBuffer);
		while (!l_SrcFile.eof()){
			*this << l_szBuffer << endl;
			l_SrcFile.getline(l_szBuffer, sizeof l_szBuffer);
		} /* end while */
		l_SrcFile.close();
	} /* End If */

	// 
	//  Insert Unique Parameter values for this file
	//
	if (m_szMsgID != NULL) 
		*this << "MsgID="       << m_szMsgID        << endl;

	*this << "DataFileName="    << m_szDataFileName << endl
	      << "ControlFileName=" << m_szCtlFileName  << endl
          << "LogFileName="     << m_szLogFileName  << endl
		  << "RestartFileName=" << m_szRestartFileName << endl
	      << "Mode="            << m_lMode          << endl
		  << "BufferSize="      << m_lBufferSize    << endl
		  << "EmptyQueueRC="    << KORC_EMPTY_QUEUE << endl
		  << ";  ******* End of Appendage *******"  << endl;
	close();
	return ll_return;
}







