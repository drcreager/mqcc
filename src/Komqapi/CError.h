/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CError.h 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CError.h
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     Declaration file for the CError Class. 
//
//  History: 
//  11/10/98 DMT Created original version.
//  12/09/98 DMT Renumbered KO-Specific Error Numbers.  Used to start
//               at 3000.  Now starting at 65536.  Also, added 
//               65556 through 65557. 
//  12/10/98 DMT Moved constants from CError.h to KOConstants.h
//  01/14/99 CAH Added Copy Constructor and Assignment Operator
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CERROR_H__
#define __CERROR_H__

#include "CString.h"
#include "cmqc.h"
#include "KOConstants.h"


//---------------------------------------------------------------------+
//  Class:    CError
//---------------------------------------------------------------------+
class CError {
public:
	long		il_reason_code;
	long		il_comp_code;
	CString*    is_error_context;


public:
	virtual long Init( void );
	virtual long GetErrorCodes( long& rl_completion_code, long& rl_reason_code);
	virtual long GetErrorContext( CString& rs_context);
	virtual void ClearErrorCodes( void );
	virtual long GetReasonCodeName( long vl_reason_code, CString* rs_name  );

	virtual void CheckError( BOOL vb_assert, long vl_comp, long vl_reason, 
			char* vs_context);

	virtual long TermSelf( void );
	CError& operator=(CError&);		// assignment operator
	CError(CError&);				// copy constructor
	CError( void );
	~CError( void );

protected:

	//-----------------------------------------------------------------+
	// Static Table for resolving Error Codes to Names
	//
	// Since it's static, one instance is shared by all instances of
	// this class.
	//-----------------------------------------------------------------+
	
	// Definition of each "row"
	typedef struct _sErrorTable
	{
		long		lErrorNumber;
		const char*	szErrorText;
	} SErrorTable;

	// Array of "rows"
	static SErrorTable sErrorTable[];

};


#endif