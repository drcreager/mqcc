/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) DataQue.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     DataQue.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CDataQue class.
//
//            This class represents a Data Queue in the Utilities.
//
//            There really isn't much different between it and CQue.
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

#ifndef __DATAQUE_H__
#define __DATAQUE_H__
#include "Que.h"





//---------------------------------------------------------------------+
//  Class:    CDataQue derived from CQue.
//---------------------------------------------------------------------+
class CDataQue : public CQue
{
private:
	static const char *QueName;
public:
	CDataQue(CQueMgr* v_pcQM);
	CDataQue();

	long GetNext(CBuffer& cBuffer, 
		         const char *szMsgID=NULL, ulong ulCorlID=0);

	virtual ~CDataQue();
	
};
#endif
