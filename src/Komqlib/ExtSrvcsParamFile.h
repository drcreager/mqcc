/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 00/11/10	@(#) ExtSrvcsParamFile.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     ExtSrvcsParamFile.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CExtSrvcsParamFile class.
//
//            This class represents a Parameter File for Extended 
//            Services and is derived from CParamFile.
//  Dev Log:  
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __EXTPARAMFILE_H__
#define __EXTPARAMFILE_H__
#include "ParamFile.h"
#include <stdlib.h>
#include <string.h>
#include "CmdLine.h"
#include "Log.h"
#include "CFile.h"

class CCmdArgs;




//---------------------------------------------------------------------+
//  Class:    CStartParamFile
//---------------------------------------------------------------------+
class CExtSrvcsParamFile : public CParamFile {
private:
	static SParamTable sParamTable[];
	static       long  m_lAppMode;
	static const char* m_szBlocking;
	static       long  m_lBufferSize;
	static       char* m_szCtlFileName;
	static       long  m_lCtrlFilesProcessed;
	static const char* m_szCtlQueName;
	static       char* m_szDataFileName;
	static       long  m_lDataFilesProcessed;
	static const char* m_szDataQueName;
	static const char* m_szDeblocking;
	static       long  m_lDecode;
	static       long  m_lDecrypt;
	static       long  m_lEncode;
	static       long  m_lEncrypt;
	static const char* m_szCipherKey1;
	static const char* m_szCipherKey2;
	static const char* m_szCipherKey3;
	static const char* m_szFileClassList;
	static       char* m_szFileDisp;
	static       char* m_szFileArchive;
	static       char* m_szLogFileName;
	static const char* m_szMsgID;
	static       long  m_lMode;
	static const char* m_szOperation;
	static const char* m_szParmFileName;
	static const char* m_szQueMgrName;
	static       char* m_szRestartFileName;
	static       char* m_szSrcFileMask;
	static       char* m_szDstDir;
	static       long  m_lSubDirSrch;
	static       long  m_lRcrdLen;
	static       char* m_szSetName;
	static       char* m_szSetClsName;
	static       char* m_szDataClsName;
	static       char* m_szGetProcessor;
	static       char* m_szPutProcessor;
	static       char* m_szOnSuccessCmd;
	static       char* m_szOnFailCmd;

	void	initMembers();
	void	LoadParams(CCmdArgs&);

protected:
	const SParamTable* GetParamTable();

public:
	CExtSrvcsParamFile(CCmdArgs&);
	CExtSrvcsParamFile();
	void Load(CCmdArgs&);


	static const char* GetLogFileName();
	long SetLogFileName(char* szValue);

	static const char* GetDataFileName();
	long SetDataFileName(char* szValue);

	static const char* GetCtlFileName();
	long SetCtlFileName(char* szValue);

	static       char* GetParmFileName();

	static const char* GetQueMgrName();
	long SetQueMgrName(char* szValue);

	static const char* GetCtlQueName();
	long SetCtlQueName(char* szValue);

	static const char* GetDataQueName();
	long SetDataQueName(char* szValue);

    char* GetMessageID();
    long  SetMessageID(char* szValue);

	static const long GetMode();
	void SetMode(long vl_value);

	static const long GetAppMode();
	void SetAppMode(long vl_value);

	static const long GetBufferSize();
	void SetBufferSize(long vl_value);

	long GetRecordLength(void);	
	void SetRecordLength(long vl_value);

	char* GetRestartFileName(void);
	long  SetRestartFileName(char* a_szArg);

	long  SetGetProcessor(char* a_szArg);
	char* GetGetProcessor();

	long  SetPutProcessor(char* a_szArg);
	char* GetPutProcessor();

	long  SetOnSuccessCmd(char* a_szArg);
	char* GetOnSuccessCmd();

	long  SetOnFailCmd(char* a_szArg);
	char* GetOnFailCmd();

	char* GetDataClsName();	
	int   GetOperation();
	char* GetSetClsName();	
	char* GetSetName();
	char* GetSrcFileMask();	
	char* GetFileArchive();	
	char* GetFileDisp();	
	char* GetDstDir();	


	virtual ~CExtSrvcsParamFile();		


	BOOL IsFileDispArchive();
	BOOL IsFileDispDelete();
	BOOL IsValidQueueName(char* szName, char* szContext);
	BOOL IsValidFileName(char* szName, char* szContext);

	BOOL IsValidQueueMgrName();
	BOOL IsValidDataQueueName();
	BOOL IsValidDataFileName();
	BOOL IsValidDisp();
	BOOL IsValidCtlQueueName();

	BOOL IsValidLogFileName();
    BOOL IsValidCtlFileName();
	BOOL IsValidSrcFileMask();
    BOOL IsValidParmFile();		

	BOOL IsValidBufferSize();

	BOOL IsValidCipherKey();

    BOOL IsEncodingSpecified();
    BOOL IsDecodingSpecified();
    BOOL IsEncryptionSpecified();
    BOOL IsDecryptionSpecified();
    BOOL IsBlockingSpecified();
    BOOL IsDeblockingSpecified();
    BOOL IsSubDirSrchSpecified();
	BOOL IsFileSetSpecified();


	long GetDataFilesProcessed();
	void SetDataFilesProcessed(long vl_value);

	long GetCtrlFilesProcessed();
	void SetCtrlFilesProcessed(long vl_value);

	long GetFilesLocated();
	void SetFilesLocated(long vl_value);

	long GetFilesProcessed();
	void SetFilesProcessed(long vl_value);

	long GetTotalFilesProcessed();
	void SetTotalFilesProcessed(long vl_value);

	void show(CLog& a_pcLog, int a_iEntryType);
	long WriteParmFile(char* a_szSrcFileName,
					   char* a_szDstFileName);

};

#endif

