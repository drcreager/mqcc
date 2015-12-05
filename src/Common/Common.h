/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) Common.h 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     Common.h
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Common C++ declarations and definitions. 
//
//  History: 
//  03/28/96  DRC Created original code.
//  12/03/98  DMT Added NEWANSICPP switch to allow code to switch from
//                New ANSI std libs to older "classic" libs.
//  12/18/98  CAH-001 Added conditionals for _WIN32 platform specific 
//                stuff
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif


#ifndef __COMMON_H__
#define __COMMON_H__

//#define NEWANSICPP

//---------------------------------------------------------------------+
//  Type definitions and defines
//---------------------------------------------------------------------+
#define begBlk {
#define endBlk }
#define endIf
#define not !
#define and &&
#define or  |

#if _WIN32
#define ulong  unsigned long
#define uint   unsigned int
#else
#define true   1
#define false  0
#endif

#define True   1
#define False  0
#define kb     *    1024
#define mb     * 1048576

#ifndef NULL
#define NULL   '\0'
#endif

#ifdef _WIN32
#define DllImport	__declspec( dllimport )
#define DllExport	__declspec( dllexport )
#else
//
// [CAH-001] No such thing as DllExport or DllImport in UNIX.
// Use null definition.
//
#define DllExport
#define DllImport
#endif

#ifndef BOOL
typedef int BOOL;
#endif

#endif

//
// Generalized Completion Codes
//
enum{
	KOCC_GOOD,
	KOCC_WARNING,
	KOCC_FAIL
};
