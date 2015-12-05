//---------------------------------------------------------------------+
//  File:     CBatchList.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CBatchList class, which is a collection
//            of message batches from an MQ Series message queue that 
//            meet the search criterial provided upon instantiation.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "Any2.h"
#include "CCollection.h"
#include "CItem.h"
#include "CString.h"
#include "Log.h"
#include "ExtSrvcsParamFile.h"

#include "QueMgr.h"
#include "CtlQue.h"
#include "CBuffer.h"
//#include <string.h>
#ifndef _iostream
#define _iostream
#include <iostream.h>
#endif


class CBatchList : public CCollection,  public CAny2 {
protected:
CLog*				m_pcLog;
CExtSrvcsParamFile*	m_pcParms;
CBuffer			    m_cBuffer;
CString*			m_pBatch;
CItem*				m_pBatchElement;
//char				m_Work[260];
CString*            m_pBatchList;

public:
    virtual void add(	char*	a_szBatchId);
	        CBatchList(	CLog*				v_pcLog,
						CExtSrvcsParamFile* v_pcParms,
				        char*				a_szBatchIdMask);
	virtual ~CBatchList();
	virtual CString*  getBatchList ( void );
	virtual long load(	char*	a_szBatchIdMask);
	virtual void unload(void);
	friend  ostream&    operator<< ( ostream& au_stream, CBatchList& au_arg1);

};

