/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CQueues.h 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CQueues.h
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     Declaration file for the CQueues Class. 
//
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT Increased Maximum Queues from 2 to 50.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CQUEUES_H__
#define __CQUEUES_H__

#include "CError.h"
#include "CQueue.h"

//#ifndef _cmqc
//#define _cmqc
#include "cmqc.h"
//#endif

#ifdef _WIN32
//#ifndef _windows
//#define _windows
#include <windows.h>
//#endif
#endif


#define KO_MAX_QUEUES	50

//---------------------------------------------------------------------+
//  Class:    CQueues
//---------------------------------------------------------------------+
class CQueues {

private:
CQueue*		iu_queues[KO_MAX_QUEUES + 1];

public:
CError		iu_error;

public:
	virtual long Init();

	virtual long AddQueue(CQueue*& ru_queue, int& ri_qid);

	virtual long GetQueue(CQueue*& ru_queue, int vi_qid);

	virtual long RemoveQueue(int vi_qid);

	virtual long RemoveAll( void );

	virtual long TermSelf();

	CQueues( void );
	~CQueues( void );
};


#endif
