/*
 *  Copyright 2000, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 00/01/10	@(#) CStringList.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CAny.h
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Declaration file for the CStringList class. 
//
//  History: 
//  01/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "CCollection.h"
#include "CString.h"

class CStringList : public CCollection  
{
public:
	virtual long clear();
	virtual long push(char* a_Arg);
	virtual long push(CString* a_Arg);
	virtual long pop(CString* a_Arg);
	CStringList();
	virtual ~CStringList();

protected:
	CString*     mp_Element;
};
