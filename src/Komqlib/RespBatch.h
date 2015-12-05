/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21     @(#) CRespBatch.h 1.0@(#)
 */
//-----------------------------------------------------------------------+
//  File:     CRespBatch.h
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Declaration file for the CRespBatch class. 
//
//
//  History: 
//  09/24/99  DRC Created original code.
//  04/05/00  DRC Modified to support cross-aggregation w/CLog 
//-----------------------------------------------------------------------+
/*
    Tech Note:  Cross-Aggregation.

       When a class contains a member attribute of another class type it 
    is refered to as aggregation.  When two classes both contain a member
    of the other's type it can be described as cross-aggregation.

    Classes which contain Cross-aggregation must be set up very carefully 
	to be compiled correctly.  This is currently the case with CLog and 
	CRespBatch.  In order to compile correctly the following structure must
	be maintained. 

    Contents of the CLog Header file (.h).
	======================================
	- a forward declaration of CRespBatch.
	  Example:  class CRespBatch;
	- a class definition for CLog.
	  Example:  class CLog{ [insert member declarations] };

    Contents of the CRespBatch Header file (.h).
	============================================
	- an #include of the CLog header file.
	- a class definition for CRespBatch.

    Contents of the CLog implementation file (.cpp).
	================================================
	- an #include of the CRespBatch header, 
	  NOTE:  DO NOT #include the CLOG header in the implementation file. 
	- the implementation of CLog class methods.

    Contents of the CRespBatch implementation file (.cpp).
	======================================================
	- an #include of the CRespBatch header. 
	- the implementation of CRespBatch class methods.

    Failure to adhear to this configuration will produce compiler errors 
	because when each class is compiled it must have a definition of the 
	other class.  This becomes a recursive problem as illustrated below:  
	
	1)  Compilation of CLog requires a definition for CRespBatch 
	2)  Compilation of CRespBatch requires a definition for CLog

    This is a "chicken or the egg" scenario which leaves the compiler stuck
	because it can't get started.  
	
	To resolve it we provide a forward declaration of CRespBatch to inform
	the compiler a definition will be coming later enabling it can construct
	a CLog class definition.  
	
	Once CLog is defined, the compiler is able to construct a definition of
	CRespBatch. Together, these two definitions contain the details the 
	compiler will need to construct the methods of both classes in a 
	following phase of compilation.
	                                           
							              Daniel R Creager, April 2000     */

#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef _CRespBatch
#define _CRespBatch

#include "Any2.h"
#include "CBuffer.h"
#include "BufferList.h"
#include "CString.h"
#include "Que.h"
#include "QueMgr.h"
#include "Log.h"

//-----------------------------------------------------------------------+
//  Class:    CRespBatch
//-----------------------------------------------------------------------+
class CRespBatch : public CAny2 {

protected:
//
//  Object Attributes
//
long			m_lUseCount;
CLog*			m_pcLog;
CQueMgr			m_cQueMgr;
CQue			m_cQue;
CBuffer			m_cBuffer;
CBufferList*	m_pcCache;
CString			m_sQMgr_Name;
CString			m_sQueue_Name;


//
//  Object Methods
//
public:
    virtual long clear(     void);
    virtual long close(     void);
    CRespBatch(				CLog*		a_pcLog,
							CString*	as_QMgr_Name);
    ~CRespBatch();
    virtual long initSelf(	CLog*		a_pcLog,
							CString*	as_QMgr_Name);
    virtual long open(		CString*	as_Queue_Name);
    virtual long put(		CString*	as_MsgId,
							CString*	as_BatchId);
    virtual long set(       CBuffer*    a_Arg);
    virtual long set(       char*       a_Arg);
    virtual long termSelf(  void);

protected:
    virtual long checkParms(CString*	as_MsgId, 
		                    CString*	as_BatchId);
    virtual long writeCache(CString*	as_MsgId,
							CString*	as_BatchId);
};

#endif