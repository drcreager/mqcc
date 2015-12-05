/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) StartupParamFile.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     StartupParamFile.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CStartParamFile class.
//
//            This class represents a Parameter File and is derived from
//            CParamFile.  It's a sibling to CRestartFile.
//  
//  History: 
//  01/15/99 CAH Created original code.
//  02/21/99 DMT Removed Error Queue
//  02/24/99 DMT Added Destructor
//  01/08/00 DRC Added Return Receipt "is" functions.
//  02/12/00 DRC Added the show method
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __STARTUPPARAMFILE_H__
#define __STARTUPPARAMFILE_H__
#include "ParamFile.h"
#include <stdlib.h>
#include <string.h>
#include "CmdLine.h"
#include "Log.h"



class CCmdArgs;




//---------------------------------------------------------------------+
//  Class:    CStartParamFile
//---------------------------------------------------------------------+
class CStartParamFile : public CParamFile
{
private:
	static SParamTable sParamTable[];
	static const long  m_lDefaultBuffSize;
	static const char* m_szRestartFileName;
	static const char* m_szLogFileName;
	static const char* m_szDataFileName;
	static const char* m_szCtlFileName;
	static const char* m_szQueMgrName;
	static const char* m_szErrQueName;
	static const char* m_szCtlQueName;
	static const char* m_szDataQueName;
	static const char* m_szReplyToQName;
	static long	m_lBatchInterval;
	static long	m_lBufferSize;

	static const char* m_szMsgID;
	static long m_lCorrelID;
	static const char* m_szPassword;
	static long	m_lMode;
	static long m_lAppMode;
	static const char* m_szDeadLetterQueName;

	static long	m_lFailOnQuiesce;
	static long	m_lExpirationInterval;
	static const char* m_szMessageFormat; 
	static long m_lAdminAccess;
	static long m_lTranslateKeyValues;
	static long m_lAutoBalance;

	static long m_lWaitInterval;           // DRC 09/28/99
	static long m_lCCSID;              	   // DRC 09/28/99
	static long m_lEncoding;           	   // DRC 09/28/99
	static const char* m_szReturnReceipt;  // DRC 09/28/99
	static long	m_lEmptyQueueRC;           // DRC 04/12/02

	//-----------------------------------------------------------------+
	// Note:  The "Total" Msgs Processed member 
	//        reflects the total in the batch and the other number
	//        reflects the number of Messages processed in
	//        this run.
	//
	//        Example:  In a 500 message batch, 250 data messages
	//                  were processed when a failure occurred.  This
	//                  run was a restart run and 100 additional
	//                  messages were processed.
	//
	//                  Original Run
	//                  ------------
	//                  DataMsgsProcessed:      250
	//                  TotalDataMsgsProcessed: 250
	//
	//                  This Run
	//                  --------
	//                  DataMsgsProcessed:      100
	//                  TotalDataMsgsProcessed: 350
	//
	//        To be consistent with the mainframe, I'll only report
	//        xxxMsgsProcessed, not TotalxxxMsgsProcessed.
	//-----------------------------------------------------------------+

	static long m_lDataMsgsProcessed;
	static long m_lTotalDataMsgsProcessed;

	static long m_lCtrlMsgsProcessed;
	static long m_lTotalCtrlMsgsProcessed;

	static long m_lExpectedMsgs;

	static long m_lReplyMsgsProcessed;
	static long m_lTotalReplyMsgsProcessed;

	static long m_lErrorMsgsProcessed;
	static long m_lTotalErrorMsgsProcessed;

	static long m_lSynchpoints;
	static long m_lUncommittedDataMsgs;
	static long m_lUncommittedCtrlMsgs;

	BOOL m_bIsRestart;

	void	initMembers();
	void	LoadParams(CCmdArgs&);

protected:
	const SParamTable* GetParamTable();
	BOOL	CheckPassword();

public:
	CStartParamFile(CCmdArgs&);
	CStartParamFile();
	void Load(CCmdArgs&);
	void Load();

	static const char* GetRestartFileName();
	long SetRestartFileName(char* szValue);

	static const char* GetLogFileName();
	long SetLogFileName(char* szValue);

	static const char* GetDataFileName();
	long SetDataFileName(char* szValue);
	
	static const long GetEmptyQueueRC();

	static const char* GetCtlFileName();
	long SetCtlFileName(char* szValue);

	static const char* GetQueMgrName();
	long SetQueMgrName(char* szValue);

	static const char* GetErrQueName();
	long SetErrQueName(char* szValue);

	static const char* GetCtlQueName();
	long SetCtlQueName(char* szValue);

	static const char* GetDataQueName();
	long SetDataQueName(char* szValue);

	static const long GetBatchInterval();
	void SetBatchInterval(long vl_value);

	static const long GetBufferSize();
	void SetBufferSize(long vl_value);

	static const char* GetReplyToQName();
	long SetReplyToQName(char* szValue);

	static const char* GetMessageID();
	long SetMessageID(char* szValue);

	static const long GetCorrelID();
	static char* GetCorrelIDStr();
	void SetCorrelID(long vl_value);

	static const char* GetPassword();

	static const long GetMode();
	void SetMode(long vl_value);

	static const long GetAppMode();
	void SetAppMode(long vl_value);

	static const char* GetDeadLetterQueName();
	long SetDeadLetterQueName(char* szValue);

	static const long GetFailOnQuiescing();
	void SetFailOnQuiescing(long vl_value);

	static const long GetExpirationInterval();
	void SetExpirationInterval(long vl_value);

	static const char* GetMessageFormat();
    long SetMessageFormat(char* szValue);

	static const long HasAdminAccess();

	static const long GetTranslate();
	void SetTranslate(long vl_value);

	BOOL IsRestart();
	void SetRestart(BOOL vb_restart);

    void show(      CLog& a_pcLog, 
		            int   a_iEntryType);

#ifdef _ver2_
        BOOL isTranslate();
        BOOL isLogReceipt();
        BOOL isDataReceipt();
#endif


	//friend ostream& operator << (ostream&, CStartParamFile&);	// dump of values.
	virtual ~CStartParamFile();				// DMT 02/24/99


	BOOL IsValidQueueName(char* szName, char* szContext);
	BOOL IsValidFileName(char* szName, char* szContext);
	BOOL IsValidQueueMgrName();
	BOOL IsValidBufferSize();
	BOOL IsValidRestartFileName();
	BOOL IsValidDataQueueName();
	BOOL IsValidDataFileName();
	BOOL IsValidBatchInterval();
	BOOL IsValidCtrlQueueName();
	BOOL IsValidCtrlFileName();
	BOOL IsValidLogFileName();
	BOOL IsValidErrorQueueName();
	BOOL IsValidReplyToQueueName();
	BOOL IsValidDeadLetterQueueName();
	BOOL IsValidMsgID(BOOL vb_AllowNull = TRUE);
	// BOOL IsValidCorrelID();	Numeric check inherent in Load
	// BOOL Fail On Quiesc		Numeric check inherent in Load
	BOOL IsValidExpirationInterval();
	BOOL IsValidMessageFormat();
#ifdef _ver2_
	BOOL IsValidReceiptRequest();
#endif
	BOOL DoAutoBalance();



	long GetDataMsgsProcessed();
	void SetDataMsgsProcessed(long vl_value);

	long GetTotalDataMsgsProcessed();
	void SetTotalDataMsgsProcessed(long vl_value);

	long GetCtrlMsgsProcessed();
	void SetCtrlMsgsProcessed(long vl_value);

	long GetTotalCtrlMsgsProcessed();
	void SetTotalCtrlMsgsProcessed(long vl_value);

	long GetExpectedMsgs();
	void SetExpectedMsgs(long vl_ExpectedMsgs);



	long GetReplyMsgsProcessed();
	void SetReplyMsgsProcessed(long vl_value);

	long GetTotalReplyMsgsProcessed();
	void SetTotalReplyMsgsProcessed(long vl_value);

	long GetErrorMsgsProcessed();
	void SetErrorMsgsProcessed(long vl_value);

	long GetTotalErrorMsgsProcessed();
	void SetTotalErrorMsgsProcessed(long vl_value);



	long GetSynchpoints();
	void SetSynchpoints(long vl_value);

	long GetUncommittedDataMsgs();
	void SetUncommittedDataMsgs(long vl_value);

	long GetUncommittedCtrlMsgs();
	void SetUncommittedCtrlMsgs(long vl_value);
};

#endif

