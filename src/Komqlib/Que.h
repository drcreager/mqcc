/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) Que.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     Que.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CQue class.
//
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//  01/08/00 DRC Added Return Receipt variables.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __QUE_H__
#define __QUE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>
#include <iomanip.h>
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#include "CString.h"
//#include <CQueue.h>
#include "QueMgr.h"
#include "CBuffer.h"
#include "Log.h"
#include "isdec.h"

//class CQueMgr;




//---------------------------------------------------------------------+
// Macro insures that number returned is 
// on a long (4 byte) boundry. This is what
// is common alignment for data elements in
// a structure or class.
//---------------------------------------------------------------------+
#define ALIGN(n) ((((n)/4)+1)*4)





//---------------------------------------------------------------------+
// Note there are two lengths for these char arrays.
// The allocated space and the actual defined length needed.
// To keep the data attributes of the class aligned on 32 bit
// boundries, they are extended by one char for null termination and 
// then sized up to the nearest divisor of 4.
// Thus the MSG_ID_LEN will be something smaller than the
// MSG_ID_ALLOC.
//---------------------------------------------------------------------+
#define MSG_ID_LEN 24
#define CORL_ID_LEN 24
#define REPLY_TO_QUE_LEN 48
#define QUEUE_NAME_LEN 48
#define FORMAT_LEN 8

#define MSG_ID_ALLOC ALIGN(MSG_ID_LEN+1)
#define CORL_ID_ALLOC ALIGN(CORL_ID_LEN+1)
#define REPLY_TO_QUE_ALLOC ALIGN(REPLY_TO_QUE_LEN+1)
#define QUEUE_NAME_ALLOC ALIGN(QUEUE_NAME_LEN+1)
#define FORMAT_ALLOC ALIGN(FORMAT_LEN+1)


#define KOMT_DATA		8	// MQMT_DATAGRAM
#define KOMT_CONTROL	1	// MQMT_REQUEST


//---------------------------------------------------------------------+
//  Class:    CQue
//---------------------------------------------------------------------+
class CQue : public CAny2
{
private:
	char		m_szName[QUEUE_NAME_ALLOC];	// name of this que.
	int 		m_iQueHandle;		// the get call needs all of this.
	long        m_lMsgLen;
	long		m_lExpiration;
	long		m_lBatchInterval;
	ulong		m_lCorlID;			// binary rep of Correllation ID.
	BOOL		m_bSyncpointEnable;
	BOOL		m_bPartialMsgs;
	long		m_lPersistence;
	char		m_szMsgID[ MSG_ID_ALLOC ];
	char		m_szCorlID[ CORL_ID_ALLOC ];
	char		m_szReplyToQue[ REPLY_TO_QUE_ALLOC ];
	long		m_lMsgType;
	long		m_lFeedbackCode;
	long		m_lReportOptions;
	char		m_szFormat[ FORMAT_ALLOC ];
	int			m_iFormatSize;
	char		m_cGetMode;
	long		m_lWaitInterval;
	long		m_lReqDataLen;
	long		m_lDeadReason;
	BOOL		m_bMatchMsgID;
	BOOL		m_bMatchCorlID;
	BOOL		m_bIsOpen;
	long            m_lPriority;
	BOOL            m_bFailIfQuiescing;
	BOOL		m_bXlateKeyValues;
#ifdef _ver2_
	BOOL		m_bLogReceipt;
	BOOL		m_bDataReceipt;
#endif

	void InitVars();
protected:
	CQueMgr*	m_pcQueMgr;			// Link back to our Q manager.
	CLog*		m_pcLog;
	static const char doBrowseFirst;
	static const char doBrowseNext;
	static const char doGetNext;
	static const char doGetUnderCursor;
	
public:
	static const int Output;
	static const int Exclusive;
	static const int Input;
	static const int Browse;

				CQue();
				CQue(CQueMgr* pcQM);

	void		ResizeBuffer(ulong);

	virtual		~CQue();

	long		QOpen(const char* Name, const int =Input);

	long		QOpen(const int =Input);

	long		BrowseFirst(CBuffer& cBuffer, const char* szMsgID=NULL,
		                    ulong ulCorlID=0);

	long		BrowseNext(CBuffer& cBuffer);

	long		GetUnderCursor(CBuffer& cBuffer);

	long		GetNext(CBuffer& cBuffer, const char* szMsgID=NULL, 
		                ulong ulCorlID=0);
	long		GetMsg(CBuffer& cBuffer, const char GetMode=doGetNext,
		               const char* szMsgID=NULL, ulong ulCorlID=0);


	long		GetMsgA(CBuffer& cBuffer, const char GetMode=doGetNext,
		               const char* szMsgID=NULL, ulong ulCorlID=0);


	const char* GetMsgID() const;
	void		SetMsgID(const char*);

	void		SetCorlID(const ulong);

	const char* GetCorlID() const;
	const ulong GetCorlIDBin() const;

	long		GetMsgTextLen() const;

	BOOL		Good();

	long		GetCompCode();

	long		GetReason();

	BOOL		IsOpen();

	const char* GetName() { return m_szName; }

//	CQueMgr&	GetQueMgr() { return m_cQueMgr; } 
//
//	Methods associated with putting something on a Queue
//

	long		Put(CBuffer& cBuffer);
	
	long		Close();
#ifdef _ver2_
	long		initSelf(CLog* v_pcLog, CQueMgr* v_pcQM, 
		                                 char* szQueueName,
						 BOOL  vb_fail,
						 BOOL  vb_xlate,
						 BOOL  vb_LogReceipt,
						 BOOL  vb_DataReceipt);

      long        GetAttrLng(        int   ai_Type);
      long        SetAttrLng(        int   ai_Type, 
		                             long  al_Arg);
      char*       GetAttrStr(        int   ai_Type);
      long        SetAttrStr(        int   ai_Type, 
                                     char* as_Arg);
#endif
	long		initSelf(CLog* v_pcLog, CQueMgr* v_pcQM, 
		                                 char* szQueueName,
						 BOOL  vb_fail,
						 BOOL  vb_xlate);

	long		GetExpiration();
	void		SetExpiration(long	vl_value);

	char*		GetReplyToQueueName();
	void		SetReplyToQueueName(char* szName);

	char*		GetMsgFormat();
	void		SetMsgFormat(char* szFormat);

	long		GetWaitInterval();
	void		SetWaitInterval(long	vl_value);

	long		GetMsgType();
	void		SetMsgType(long vl_value);

	long		GetPersistence();
	void		SetPersistence(long vl_value);

	long		GetFeedbackCode();
	void		SetFeedbackCode(long vl_value);

	long		GetReportOptions();
	void		SetReportOptions(long vl_value);

	long		GetDeadReason();

	long        GetPriority();
	void        SetPriority(long vl_value);


	BOOL        GetFailIfQuiescing();
	void        SetFailIfQuiescing(BOOL vb_value);

	BOOL        GetTranslate();
	void        SetTranslate(BOOL vb_value);


	BOOL        GetSyncpointEnable();
	void        SetSyncpointEnable(BOOL vb_value);

};
#endif
