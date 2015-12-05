//---------------------------------------------------------------------+
//  File:     isdec.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     See isdec.cpp for description.
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

#ifndef __ISDEC_H__
#define __ISDEC_H__

#include <string.h>
#include "Common.h"

#ifndef FALSE
#define FALSE 0
#endif

extern "C" BOOL isdec(const char*);

#endif
