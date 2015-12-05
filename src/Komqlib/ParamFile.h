/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) ParamFile.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     ParamFile.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CParamFile class.
//
//            This is a generic class derived from fstream and CAny2.
//            It does not know about the log file, so callers 
//            have to get ParamFile errors out of it's Errors 
//            Collection.
//
//            Descendents of this class define a static table that
//            describes the specific parameters in a parameter file.
//            Descendents also have member variables for storing
//            each parameter.
//
//            This ancestor contains the parameter file processing
//            logic.
//  
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//  04/28/99 DMT Added rtrim function.
//  11/10/00 DRC Added valid_bool function
//  02/21/01 DRC Modified inheritance from fstream to CFile to enable
//               the use of is_open within the AIX environment by 
//               decendants of this class.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __PARAMFILE_H__
#define __PARAMFILE_H__
//#include <fstream.h>
#include "CFile.h"
#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "CString.h"
#include "auto_ptr.h"
#include "Any2.h"
#include "isdec.h"


class CCmdArgs;



//---------------------------------------------------------------------+
//  Class:    CParamFile derived from fstream and CAny2.
//---------------------------------------------------------------------+
class CParamFile : public CFile, public CAny2
{
private:
	static const int m_iReadMax;
	char*			 m_szReadBuf;
	char*			 m_szLastError;

	

public:
	typedef enum
	{
		TypeLong,
		TypeString,
		TypeInvalid
	} EParamType;
protected:

	char*			 m_szFileName;
	
	typedef struct _sParamTable
	{
		const char* szParamName;
		EParamType	eParamType;
		void		*paramStore;
		BOOL		bRequired;
		BOOL		bLoaded;
	} SParamTable;

	virtual const SParamTable* GetParamTable()=0;

	long SetString(char** pszDest, char* pszSource);

	virtual void TranslateErrno (CString& rs_msg, int vi_errno);

	long	CalcPassword(CString& sPwd);

	virtual void rtrim(char* vszArg);	// DMT 4/28/99

    const char* valid_bool(const long l);    // DRC 11/10/00
    const char* valid_str(const char* c);


public:
	CParamFile(const char*);
	CParamFile();
	const char*  GetName() { return m_szFileName; }
	const char* GetInput() { return m_szReadBuf; }
	long Open(const int mode);
	long Open(const char* filename, const int mode = ios::in|ios::nocreate);
	long Load();
	void Load(CCmdArgs&);
	void Remove();

	long		initSelf(char* szFileName);

	virtual ~CParamFile();
	friend ostream& operator << (ostream& s, CParamFile& pf);
	
};


#endif
