//---------------------------------------------------------------------+
//  File:     MsgFile.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CMsgFile class.
//
//            This class represents a file that can be used as an
//            input for Put programs and an Output for Get Programs.
//  History: 
//  03/12/99 DMT Created original code.
//  09/18/00 DRC Renamed CFile to CMsgFile.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __FILE_H__
#define __FILE_H__
//#include <fstream.h>
//#include <iomanip.h>
//#include "CString.h"

class CLog;

#include "Log.h"
#include "CFile.h"
#include "Any2.h"


//---------------------------------------------------------------------+
//  Class:    CMsgFile
//---------------------------------------------------------------------+
class CMsgFile : public CFile, public CAny2
{
protected:
	CLog*			 m_pcLog;	

public:
	CMsgFile();
	virtual ~CMsgFile();
	long	Advance(CBuffer& cBuffer, long vl_lines);
	long	initSelf(CLog* v_pcLog, char* szName);
	long	Open(int vi_mode);
	long	Read(CBuffer& cBuffer);
	long	Remove();
	long    termSelf();
	long	Write(CBuffer& cBuffer);
};
#endif
