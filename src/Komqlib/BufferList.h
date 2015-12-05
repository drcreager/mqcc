/*
 *  Copyright 2000, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 00/01/10	@(#) CBufferList.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     BufferList.h
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Declaration file for the CBufferList class. 
//
//  History: 
//  01/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CCollection
//#define _CCollection
#include "CCollection.h"
//#endif

//#ifndef _CBuffer
//#define _CBuffer
#include "CBuffer.h"
//#endif

//#ifndef _CLog
//#define _CLog
#include "Log.h"
//#endif

class CBufferList : public CCollection  
{
public:
	virtual long  clear();
	virtual long  push(	        CBuffer*	a_Arg);
	virtual long  pop(	        CBuffer*	a_Arg);
	virtual void  top(	        CBuffer*	a_Arg);
	              CBufferList(	CLog*		a_pcLog);
	virtual      ~CBufferList();

protected:
	CBuffer*    m_pElement;
	CLog*		m_pcLog;

};
