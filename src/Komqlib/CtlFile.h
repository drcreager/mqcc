//---------------------------------------------------------------------+
//  File:     CtlFile.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CCtlFile class.
//
//            This class represents a Control File in the Utilities
//            and knows how to get the Message Count out of a
//            Control Message.
//
//  History: 
//  03/23/99 DMT Created original code.
//  09/20/00 DRC Converted File references to MsgFile.
//  11/10/00 DRC Added UnloadData and Export Methods
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CTLFILE_H__
#define __CTLFILE_H__
#include "MsgFile.h"
#include "isdec.h"
#include "stdlib.h"
#include "CBuffer.h"

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
#pragma pack(push,CtlFile,1)
#endif


//---------------------------------------------------------------------+
//  Control Message Field Lengths and Structure
//---------------------------------------------------------------------+
#define CTL_PARTNER_PROF_LEN	10
#define CTL_MESSAGE_TYPE_LEN	30
#define CTL_BATCHNUMBER_LEN		14
#define CTL_COUNT_LEN			11
#define CTL_AMOUNT_LEN			23

typedef struct _SCtlRecord2
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
} SCtlRecord2;



//---------------------------------------------------------------------+
//  Turn off Compiler directives
//---------------------------------------------------------------------+
#ifndef _WIN32
#pragma pack;
#else
#pragma pack(pop,CtlFile)
#endif




//---------------------------------------------------------------------+
//  Class:    CCtlFile derived from CFile
//---------------------------------------------------------------------+
class CCtlFile : public CMsgFile
{
private:
	long	m_lNumMsgs;

protected:
	SCtlRecord2*	m_pcCtlRcrd;

public:
	CCtlFile();
	virtual ~CCtlFile();
	long    Export(    long     a_lMsgCnt);
	long	GetNumMsgs();
	long    LoadData(  CBuffer& cBuffer);
	long	Read(      CBuffer& cBuffer);
	void    SetNumMsgs(long     a_lArg);
	long    UnloadData(CBuffer& cBuffer);
};


#endif

