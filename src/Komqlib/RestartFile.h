/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) RestartFile.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     RestartFile.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CRestartFile class.
//
//            This class represents a Restart File and is derived from
//            CParamFile.  It's a sibling to CStartParamFile.
//  
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//  02/12/00 DRC Added the show method
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __RESTARTFILE_H__
#define __RESTARTFILE_H__
#include <fstream.h>
#include "CString.h"
#include "Log.h"
//#include "Que.h"
//#include "DataQue.h"
//#include "QueMgr.h"
#include "ParamFile.h"
//#include "Exceptions.h"






//---------------------------------------------------------------------+
//  Class:    CRestartFile derived from CParamFile
//---------------------------------------------------------------------+
class CRestartFile : public CParamFile
{
private:
	static char* m_szMsgID;
	static ulong m_lDataMsgs;
	static ulong m_lCorlID;
	static ulong m_lCtlMsgs;
	static ulong m_lExpected;
#ifdef _ver2_
	static char* m_szRestartKey;
#else
	static CString m_sRestartKey;
#endif

	static SParamTable sParamTable[];
	//void update();
public:
	CRestartFile(const char* filename);
	CRestartFile();

	char*	GetMsgID();
	void	SetMsgID(char* szMsgID);

	ulong	GetDataMsgsRead();
	void	SetDataMsgsRead(ulong vul_DataMsgsRead);

	ulong	GetExpectedCount();
	void	SetExpectedCount(ulong vul_ExpectedCount);

	ulong	GetCorlID();
	void	SetCorlID(ulong vul_CorlID);


	ulong	GetCtlMsgCount();
	void	SetCtlMsgCount(ulong vul_CtlMsgCount);

        char*   GetRestartKey();
	void	SetRestartKey(char* as_Arg1);

//	void	Init(/*CCtlQue&*/);
//	void	Checkpoint(/*CCtlQue&*/);

	long	WriteRestartRec();
	long	WriteRestartRec(ulong vul_DataMsgsRead);

	virtual ~CRestartFile();
	virtual	const SParamTable* GetParamTable();

	virtual void show(CLog& a_pcLog, int a_iEntryType);
};
#endif
