/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) CtlQue.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     CtlQue.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CCtlQue class.
//
//            This class represents a Control Queue in the Utilities
//            and knows how to get the Message Count out of a
//            Control Message.
//  
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif




#ifndef __CTLQUE_H__
#define __CTLQUE_H__
#include "Que.h"



//---------------------------------------------------------------------+
//  Note:  The code below tells the compiler not to align the 
//         Control Message structure along word boundaries.
//
//         This is necessary to ensure the structure is not altered
//         and accurately reflects the contents of the control message.
//---------------------------------------------------------------------+
#ifndef _WIN32
#pragma pack(1);
#else
#pragma pack(push,CtlQue,1)
#endif


//---------------------------------------------------------------------+
//  Control Message Field Lenghts and Structure
//---------------------------------------------------------------------+
#define CTL_PARTNER_PROF_LEN 10
#define CTL_MESSAGE_TYPE_LEN 30
#define CTL_BATCHNUMBER_LEN 14
#define CTL_COUNT_LEN  11
#define CTL_AMOUNT_LEN 23

typedef struct _SCtlRecord
{
	char	rPartnerProfile[ CTL_PARTNER_PROF_LEN ];
	char	rMessageType[ CTL_MESSAGE_TYPE_LEN ];
	char	rBatchNumber[ CTL_BATCHNUMBER_LEN ];
	char	rControlCount1[ CTL_COUNT_LEN ];
	char	rControlCount2[ CTL_COUNT_LEN ];
	char	rControlCount3[ CTL_COUNT_LEN ];
	char	rControlCount4[ CTL_COUNT_LEN ];
	char	rControlCount5[ CTL_COUNT_LEN ];
	char	rControlAmount1[ CTL_AMOUNT_LEN ];
	char	rCntlAmtSign1;
	char	rControlAmount2[ CTL_AMOUNT_LEN ];
	char	rCntlAmtSign2;
	char	rControlAmount3[ CTL_AMOUNT_LEN ];
	char	rCntlAmtSign3;
	char	rControlAmount4[ CTL_AMOUNT_LEN ];
	char	rCntlAmtSign4;
	char	rControlAmount5[ CTL_AMOUNT_LEN ];
	char	rCntlAmtSign5;
} SCtlRecord;



//---------------------------------------------------------------------+
//  Turn off Compiler directives
//---------------------------------------------------------------------+
#ifndef _WIN32
#pragma pack;
#else
#pragma pack(pop,CtlQue)
#endif





//---------------------------------------------------------------------+
//  Class:    CCtlQue derived from CQue
//---------------------------------------------------------------------+
class CCtlQue : public CQue
{
private:
	ulong	m_lDataMsgCount;
	ulong	m_lNumDataMsg;
	ulong	m_lCorlID;
	ulong	m_lCtlMsgCount;
	ulong	m_lUncommittedMsgCount;

	long	LoadData(CBuffer& cBuffer);
public:
	CCtlQue(CQueMgr* v_pcQM);
	CCtlQue();

	virtual ~CCtlQue();

	void  ClearUncommittedMsgCount();
	ulong GetUncommittedMsgCount() const ;
	ulong GetDataMsgCount() const ;
	ulong GetNumDataMsg() const ;
	BOOL  DataMsgCountMatches() const;
	void  CountGoodDataMsg();
	ulong GetRecCount() const ;
	void  SetRecCount(const ulong);
	ulong GetCtlMsgCount() const;
	void  SetCtlMsgCount(const ulong);
	void  IncCtlMsgCount();
	friend ostream& operator << (ostream&, CCtlQue&);

	long BrowseFirst(CBuffer& cBuffer, 
		             const char* szMsgID=NULL, ulong ulCorlID=0);

	long BrowseNext(CBuffer& cBuffer);
	
};
#endif
