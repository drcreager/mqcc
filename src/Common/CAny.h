/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CAny.h 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CAny.h
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Declaration file for the CAny abstract class. 
//
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __ANY_H__
#define __ANY_H__

//#ifndef _Common
//#define _Common
#include "Common.h"
//#endif

//---------------------------------------------------------------------+
//  Class:    CAny
//---------------------------------------------------------------------+
class CAny {

public:
CAny(void);
~CAny(void);

virtual long initSelf(void);
virtual BOOL isValidPointer(const void* a_pArg);
virtual long termSelf(void);


};

#endif    
