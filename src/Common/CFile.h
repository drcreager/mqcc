//---------------------------------------------------------------------+
//  File:     CFile.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CFile class.
//
//            This class represents a file that can be used as an
//            input for Put programs and an Output for Get Programs.
//  History: 
//  03/12/99 DMT Created original code.
//  09/15/00 DRC Reduced this class to make it more general and 
//               therefore more reusable.  Eliminating any references 
//               the CLog class.
//           DRC Added the "Blocked" indicator. 
//  11/10/00 DRC The Unix style record/block delimiter (LF) is used
//               within the buffer's internal representation.   
//  02/21/01 DRC Added the is_open() method for Unix compatability.
//               Microsoft fstream.h offers this method by wrappering
//               a filebuf method for us, however this method was not 
//               wrappered in the AIX implementation of fstream.h.
//  04/02/02 DRC Added parameter to READ and provided an additional READ
//               for backward compatability.  The new parameter makes 
//               clearing the buffer optional as a performance boost 
//               when scanning a file for MaxRcrdLen.
//---------------------------------------------------------------------+
#ifdef _WIN32
	#pragma once
#else
	#pragma options once
#endif

#ifndef __CFILE_H__
#define __CFILE_H__
#include <fstream.h>
#include <iomanip.h>
#include "CString.h"
#include "CBuffer.h"

//
//  LastCC constants
//
enum{
	CFILE_NO_ERROR,
	CFILE_EOF,
	CFILE_ERR_OPEN_FAILED,
	CFILE_ERR_READ_FAILED,
	CFILE_ERR_READ_TRUNCATED,
	CFILE_ERR_LINE_TERMINATOR_MISSING,
	CFILE_ERR_WRITE_FAILED,
	CFILE_ERR_REPOSITION_FAILED,
	CFILE_ERR_REMOVE_FAILED
};

//---------------------------------------------------------------------+
//  Class:    CFile
//---------------------------------------------------------------------+
class CFile : public fstream
{
protected:
	static const int m_iReadMax;
	int              m_iOpenMode;
	BOOL			 m_bWritten;
	CString			 m_szFileName;
	BOOL			 m_bOpen;
	BOOL             m_bWasOpen;
	int				 m_iLastCC;
	long			 m_lMaxRcrdLen;
    long             m_lRcrdCnt;
    long             m_lBlkCnt;

public:
	CFile();
	CFile(  char* a_szName, int a_iMode);
	const char*  GetName();
	virtual ~CFile();
	long	Advance(CBuffer& cBuffer, long vl_lines);
	long	Close();
	long    getMaxRcrdLen();
	long    getBlkCount();
	int		getLastCC();
	long    getRcrdCount();
	void    initSelf() ;
	long	initSelf(char* szName);
	int     is_open() { return rdbuf()->is_open(); } //DRC 02/21/01
	long    setRcrdCount();
	long	termSelf();
	long	Open(int vi_mode);
	long	Write(CBuffer& cBuffer);
	long	Write(CBuffer& cBuffer, BOOL a_ClrBufInd);
	long	Read(CBuffer& cBuffer);
	long    Read(CBuffer& cBuffer, BOOL a_ClrBufInd);
	long	Remove();	

	friend  ostream&    operator<< ( ostream& au_stream, CFile& au_arg1);

};
#endif
