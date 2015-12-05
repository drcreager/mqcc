/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: %E%	%A%
 */
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CSTRING_H__
#define __CSTRING_H__
//-----------------------------------------------------------------------+
//  File:     CString.h
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Declaration file for the CString class. 
//
//  History: 
//  03/28/96  DRC Created original code.
//
//  12/02/98  DMT Added additional methods and constructors to emulate 
//                some of the methods available in the new ANSI C++ 
//                standard "string" class.  These additions are indicated
//                via comments.
//                
//                Also, changed the >> operator to use "getline" instead
//                of "get".  
//
//  02/28/99  DMT Added operator << methods for concatenating CStrings
//                and longs onto CStrings.
//  11/10/00  DRC Added the strstr method.
//-----------------------------------------------------------------------+
#ifndef _CAny
#define _CAny
#include "CAny.h"
#endif

#ifndef _string
#define _string
#include <string.h>
#endif

#include <stdio.h>	// DMT 02/28/99

#ifdef _WIN32
#ifndef _comdef
#define _comdef
#include <comdef.h>
#endif
#endif

#ifndef _iostream
#define _iostream
#include <iostream.h>
#endif
#ifndef _WIN32
typedef char*	_bstr_t;
extern "C" int strnicmp(const char*, const char*, size_t);
#endif

//-----------------------------------------------------------------------+
//  Class:    CString
//-----------------------------------------------------------------------+
class CString : public CAny 
{
public: 
char*	lp_string;                      // Pointer to the String  Value
char*   m_Buf1;                         // Alias for lp_string:
char	ls_fillChar;                    // Fill Character	
ulong	ll_strLen;                      // String Length Attribute 
ulong	ll_vLen;                        // Value  Length Attribute 
char*	lp_temp;                        // Pointer to a temporary Value
char*   m_Buf2;                         // Alias for lp_temp:
_bstr_t ls_bstr;                        // Basic String

protected:
	virtual void		copy(char* as_arg, char* rs_Arg1);
	virtual void		copy(char* as_arg, CString* rs_Arg1);

public:
    virtual BOOL        grep(char* a_szSubject, 
				             char* a_szPattern, 
				             char* a_szDlmtrs);
    virtual BOOL        grep(char* a_szPattern);
	virtual ulong		len (void);
	virtual ulong		vlen (void);
	virtual ulong		length (void);						// DMT 12/1/98
	virtual ulong		set_vlen(void);
	virtual ulong		set_vlen(ulong al_length); 
	virtual char*		trim (void);
	virtual int			pos (int vi_pos, char* rs_Arg1,     // DRC 09/21/99
		                int vi_option);
	virtual int			pos (int vi_pos, char* rs_Arg1);
	virtual int			pos (char* rs_Arg1);
	virtual int         find(char vs_char);					// DMT 12/1/98
	virtual int			pos (int vi_pos, CString* rs_Arg1);
	virtual int			pos (CString* rs_Arg1);	
	virtual int 		match (char* rs_Arg1);
	virtual int 		match (CString* rs_Arg1);
	virtual char*		mid (int vi_index, uint vi_len);
	virtual char*       substr(int vi_pos, uint vi_len);	// DMT 12/1/98
	virtual char*		mid (int vi_index);
	virtual char*       substr(int vi_pos);					// DMT 12/1/98
	virtual char*       StrStr(char* a_szArg);
	virtual char*       StrStr(char* a_szSubj, char* a_szArg);
	virtual char*       toLower();
	virtual char*       toUpper();	
	virtual void		fill (char vc_FillChar);
	virtual void		fill (void);
	virtual void		fill (char* lp_string, char vs_fillChar);
	virtual _bstr_t*	getBStr    ( void );
	virtual char*		getPointer ( void );
	virtual char*       c_str (void);						// DMT 12/1/98
	virtual int			replace    (char* rs_Search, char* rs_Replacement);
	virtual void		resize (long al_newSize);
	virtual void		operator=  (CString* rs_Arg1);
	virtual void		operator=  (const CString& rs_Arg1);// DMT 12/2/98
	virtual void		operator>> (CString* rs_Arg1);
	virtual void		operator=  (char*    rs_Arg1);
	virtual void		operator>> (char*    rs_Arg1);
	virtual CString*    operator+  (CString* rs_Arg1);
	virtual char*		operator+  (char*    rs_Arg1);
	virtual void		operator+= (CString* rs_Arg1);
	virtual void		operator+= (const CString& rs_Arg1);// DMT 12/2/98
	virtual void		operator+= (char*    rs_Arg1);
	virtual void        operator+= (long     vl_Arg);
	virtual BOOL		operator== (char*    rs_Arg1);
	virtual int         compare(int vi_index, int vi_length, 
								CString& rs_string);		// DMT 12/1/98
	virtual BOOL 		operator== (CString* rs_Arg1);
	virtual BOOL		operator!= (char*    rs_Arg1);
	virtual BOOL 		operator!= (CString* rs_Arg1);
	virtual char& 		operator[] (ulong vl_Indx);         // DRC 10/04/99
	friend  ostream&    operator<< ( ostream& au_stream, CString& au_arg1);
	friend  istream&    operator>> ( istream& au_stream, CString& au_arg1);
    CString(ulong vl_size);
    CString();												// DMT 12/1/98
	CString(char* rs_Arg1);									// DMT 12/1/98
    CString(const CString& rs_Arg1);						// DMT 12/2/98

    ~CString(void);

	virtual CString& operator<< ( const CString& au_right );// DMT 02/28/99
	virtual CString& operator<< ( const long vl_long);		// DMT 02/28/99

//	virtual CString& operator<< ( CString& au_left, char* rs_arg);
//	virtual CString& operator<< ( CString& au_left, long vl_long);

};

#endif // __CSTRING_H__
