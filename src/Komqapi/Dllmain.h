/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: %E%	%A%
 */
//-----------------------------------------------------------------------+
//  File:     DllMain.h
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     ...
//
//  History: 
//  11/07/98  DMT Created original code.
//  12/21/98  CAH-001 Updated to build on AIX
//-----------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DLLMAIN_H__
#define __DLLMAIN_H__

#include "common.h"
#include "CQueueMgr.h"

//#ifndef _CAPIParmSet
//#define _CAPIParmSet
#include "CAPIParmSet.h"
//#endif


// [CAH-001] - Only include windows if on a
// Windows platform.
//
#if defined(_WIN32) && !defined(_windows)
//#define _windows
#include <windows.h>
#endif

#ifndef _WIN32
#include <pthread.h>
//
// [CAH-001]
// Substitute windows defines for Unix
//
#define DWORD unsigned long
#define APIENTRY 
#define HINSTANCE unsigned long
#define LPVOID void*
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_PROCESS_DETACH 3
#define DLL_THREAD_DETACH 4 
#define GPTR 0
#endif

//
//  Declare a Thread Object Collection Structure
//
typedef struct {
	CQueueMgr*		lu_queue_mgr;
	CAPIParmSet*	lu_API_ParmSet;
} threadObjects;

//
//  Global variables
//
DWORD gp_tlsIndex;             // Thread local storage index

//
//  Function Prototype declarations
//
void	Allocate(void);
void	Release(void);


#endif