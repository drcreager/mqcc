//---------------------------------------------------------------------+
//  File:     CBuffer.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CBuffer class.
//
//            The CBuffer class represents a Buffer of memory that
//            receives data from a message on a queue, or to
//            contains data to be put into a message on a queue.
//
//  History: 
//  03/12/99 DMT Created original code.
//  01/13/00 DRC Added the assignment operator (operator=)
//  09/15/00 DRC Reduced this class to make it more general and 
//               therefore more reusable.  Eliminating any references 
//               the CLog class.
//  11/10/00 DRC The Unix style record/block delimiter (LF) is used
//               within the buffer's internal representation.   
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include "CAny.h"
#include <string.h>

//
//  LastCC constants
//
enum{
	CBUF_NO_ERROR,
	CBUF_ERR_ALLOCATE_FAILED
};

//
//  Delimiter Direction constants relative to the Buffer
//
enum{
	CBUF_IN_BOUND,
	CBUF_OUT_BOUND
};
#define CR '\r'
#define LF '\n'
#define CRLF '\r\n'
#define StrTrm '\0'


//---------------------------------------------------------------------+
//  Class:    CBuffer
//---------------------------------------------------------------------+
class CBuffer : public CAny {

protected:
	long	m_lSize;
	long    m_lRcrdLen;
	long    m_lDataLen;
	BOOL	m_bBlocked;
	BOOL	m_bFixedLength;
	char*	m_szBuffer;
	char*	m_szWrkBuf;
	char*   m_szCursor;
	char*   m_szLastRcrd;
	int	    m_iLastCC;
	int		m_iIOBlkDir;



public:

	void  Allocate(   char**     a_szBuffer, 
		              long       a_lSize,
					  BOOL       a_bReset);
	char* Allocate(   long       a_lSize,
					  BOOL       a_bReset);

	CBuffer();
	virtual ~CBuffer();

	void  clear();
	void  CnvrtDlmtrs();
	long  CrlfToLf(   char**     a_hBuffer, 
		              long       a_lSize, 
					  long       a_lDataLen);
    void  expand(     char**     a_szBuffer,
		              long       a_lSize,
					  long       a_lAmount);
	char* getFirst();
	char* getCursor();
	char* getPointer();
	long  getAvail();
	long  getDataLen();
	long  getRcrdLen();
	long  getSize();

    void  LfToSz(     char*      a_szBuffer,
		              long       a_lDataLen);
    void  SzToLf(     char*      a_szBuffer,
		              long       a_lDataLen);
	long  LfToCrlf(   char**     a_hBuffer, 
		              long       a_lSize, 
					  long       a_lDataLen);
	long  initSelf(   long       vl_size);
 	long  initSelf(   long       vl_size,
		              BOOL       a_bBlocked,
					  long       a_lRcrdLen);
	BOOL  isBlocked();
	BOOL  isFixed();
	BOOL  isEndOfBuffer();
	BOOL  isFilled();
	char* next();
	char* next(		  long		 a_RcrdLen,
		              BOOL       a_InputMode);
	void  RFill(      int        a_FillChr);
	char* RTrim(	  char*      a_pArg,
			          long		 a_RcrdLen);
	char* RTrim(	  char*      a_pArg);
	void  setBlocked( BOOL       a_bArg);
	void  setDataLen( long       a_lArg);
	void  setEndOfBuffer();
	void  setFixedLen(BOOL       a_bArg);
	void  setIOBlockingDir(int   a_iArg);
    void  operator=  (CBuffer*   a_pArg);
	void  operator=  (char*      a_pArg);
	long  termSelf();
};

#endif

