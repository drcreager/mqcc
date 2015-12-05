//---------------------------------------------------------------------+
//  File:     DeadBatch.h
//  Author:   David Tamkun
//  Written:  April 1999
//  Desc:     Declaration file for the CDeadBatch class.
//
//            This class represents a Batch of Messages found on 
//            the Dead Letter Queue and represents a node of a linked-
//            list of batches generated during DeadLetter Queue
//            Reporting (execution of the DeadReport Algorithm).
// 
//  History: 
//  04/15/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DEADBATCH_H__
#define __DEADBATCH_H__
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <fstream.h>
//#include <iomanip.h>
//#include <strstream.h>


#include "Any2.h"
#include "CString.h"
#include "Log.h"
#include <cmqc.h>







//---------------------------------------------------------------------+
//  Class:    CDeadBatch
//---------------------------------------------------------------------+
class CDeadBatch : public CAny2
{
private:
	CDeadBatch*	m_pcNext;
	CDeadBatch* m_pcPrevious;

protected:
	CLog*		m_pcLog;
	
public:
	CString		m_sBatchID;
	CString		m_sDestQueueName;
	CString     m_sDestQueueMgrName;
	CString		m_sReplyQueueName;

	CString     m_szStatus;
	CString     m_szDeadReason;
	
	long		m_lDeadReason;

	long        m_lExpectedMsgs;
	
	long        m_lDataCount;
	long        m_lCtrlCount;
	long        m_lReplyCount;
	long        m_lErrorCount;



				CDeadBatch();
	virtual		~CDeadBatch();

	long        initSelf(CLog* v_pcLog);

	long        termSelf();

	long        SetStatus();
	char*       GetStatus();

	CDeadBatch*	GetNext();
	CDeadBatch* GetPrevious();

	void SetNext(CDeadBatch* v_pcNext);
	void SetPrevious(CDeadBatch* v_pcPrevious);

	long		TallyBatch(long vl_MsgType, long vl_NumMsgs);





};
#endif
