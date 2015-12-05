//---------------------------------------------------------------------+
//  File:     DeadBatches.h
//  Author:   David Tamkun
//  Written:  April 1999
//  Desc:     Declaration file for the CDeadBatches class.
//
//            This class represents a "Collection" of CDeadBatch
//            objects that's implemented as a linked list.  This
//            collection is used by the DeadReport algorithm
//            (aka Dead Letter Mode 2).
//
//  History: 
//  04/15/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DEADBATCHES_H__
#define __DEADBATCHES_H__

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <fstream.h>
//#include <iomanip.h>
//#include <strstream.h>


#include "Any2.h"
#include "CString.h"
#include "DeadBatch.h"

#include "Log.h"








//---------------------------------------------------------------------+
//  Class:    CDeadBatches
//---------------------------------------------------------------------+
class CDeadBatches : public CAny2
{
private:

	CDeadBatch*	m_pcFirstBatch;
	CDeadBatch* m_pcLastBatch;

	long		m_lNumBatches;


	CDeadBatch* AddBatch(CString& rs_BatchID,      CString& rs_Reason, 
						 CString& rs_DestQueueMgr, CString& rs_DestQueue, 
						 CString& rs_ReplyToQ);

	CDeadBatch* GetBatch(CString& rs_BatchID);

	long        RemoveAll();
	
protected:
	CLog*		m_pcLog;
	
public:

				CDeadBatches();
	virtual		~CDeadBatches();

	long        initSelf(CLog* v_pcLog);

	long        termSelf();

	long		SetStatus();

	long        ProcessBatch(CString& rs_BatchID,
		                     long     vl_MsgType,
							 long     vl_ExpectedCount,
							 CString& rs_Reason,
							 CString& rs_DestQueueMgr,
							 CString& rs_DestQueue,
							 CString& rs_ReplyToQ);

	long        LogReport();

};
#endif
