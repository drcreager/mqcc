/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) CmdLine.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     CmdLine.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CCmdArgs class.
//
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#ifdef _WIN32
 #pragma once
 #ifdef _NO_POSIX_
  #define PATH_MAX            512
 #endif
#else
 #pragma options once
#endif

#ifndef __CMDARGS_H__
#define __CMDARGS_H__
#include <fstream.h>
#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include "CString.h"
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#include "Any2.h"



//---------------------------------------------------------------------+
//  Class:    CCmdArgs
//---------------------------------------------------------------------+
class CCmdArgs : public CAny2
{
private:
	CCmdArgs(){};
	CCmdArgs(CCmdArgs&){}

	// Members for possible Command Line Option Values
	BOOL	m_bRestart;
	ulong	m_ulBatchInterval;
	char	m_szParameterFile[PATH_MAX+1];

public:
	CCmdArgs(int argv, const char *argc[]);

	// Methods for Accessing Command Line Values
	const char* GetParamFileName();
	const BOOL IsRestart() const;
	const ulong GetBatchInterval() const;
	const BOOL BatchIntervalOverride() const;

	virtual ~CCmdArgs();
};
#endif
