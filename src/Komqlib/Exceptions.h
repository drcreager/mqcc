/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) Exceptions.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     Exceptions.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for multiple Exception Classes all 
//            derived from CExceptionBase
//
//            *** This class is JUNK in the 2.0.0x versions. ***
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



#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__
#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#include <errno.h>	// DMT 02/23/99
//#include "Que.h"
//#include "QueMgr.h"
//#include "CtlQue.h"
//#include "CtlFile.h"
//#include "DataFile.h"
//#include "StartupParamFile.h"
#include "Log.h"
class CParamFile;
//class CQueMgr;
//class CCtlQue;
//class CQue;
//class CDataFile;
//class CCtlFile;
//class CLogFile;		// DMT 02/22/99


//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExceptionBase
{
private:
	const char*	m_szLastError;
protected:
//	CExceptionBase(CExceptionBase&);
	char*	m_szMsg;
public:
	CExceptionBase();
	CExceptionBase(const char* szLastErr);
	virtual ~CExceptionBase();
	virtual char* GetMsg() = 0;
	friend ostream& operator << (ostream &os, CExceptionBase& ex);
	const char* GetLastError() { return m_szLastError; }
};






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcBadRecCount : public CExceptionBase
{
public:
	virtual char* GetMsg();
	CCtlQue& m_cCtlQue;
	CExcBadRecCount(CCtlQue& q);
};*/






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
//
// No param file on command line.
//
class CExcBadCmdLineNoParamFile : public CExceptionBase
{
public:
	virtual char* GetMsg();
	CExcBadCmdLineNoParamFile() : CExceptionBase() {}
};






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadCmdLine : public CExceptionBase
{
private:
	const char* m_szCmdLine;
public:
	virtual char* GetMsg();
	CExcBadCmdLine(const char* msg, const char* szCmdLine) 
		: CExceptionBase(msg) { m_szCmdLine = szCmdLine; }
};





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadParam : public CExceptionBase
{
private:
	char*	m_szBadLine;
public:
	virtual char* GetMsg();
	CParamFile& m_cParamFile;
	CExcBadParam(const char* msg, CParamFile& pf) 
		: CExceptionBase(msg), m_cParamFile(pf) { }
};





//---------------------------------------------------------------------+
//  Class:    Cxxx
//
// 02/23/99 DMT Added m_lLastErrno member.
//---------------------------------------------------------------------+
class CExcBadParamFile : public CExceptionBase
{
private:
	CParamFile&	m_cParamFile;
	long		m_lLastErrno;
public:
	virtual char* GetMsg();
	CExcBadParamFile(char* msg, long err, CParamFile& p) 
		: CExceptionBase(msg), m_cParamFile(p) { m_lLastErrno = err; }
};





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcBadQueOpen : public CExceptionBase
{
private:
	CQue&	m_cQue;
public:
	virtual char* GetMsg();
	CQue&	GetQ() { return m_cQue; }
	CExcBadQueOpen(CQue& q) : m_cQue(q) { }
};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcBadQueMgr : public CExceptionBase
{
	CQueMgr& m_cQM;
public:
	virtual char* GetMsg();
	long	m_lRetCode;
	CExcBadQueMgr(CQueMgr& qm, long retCode ) : CExceptionBase(), m_cQM(qm){ m_lRetCode = retCode; }
};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcBadGet : public CExceptionBase
{
public:
	virtual char* GetMsg();
	CQue&	m_cQue;
	CExcBadGet(CQue& q) : CExceptionBase(), m_cQue(q) {}
};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcReqParamMissing : public CExceptionBase
{
private:
	CParamFile& m_cParamFile;
public:
	virtual char* GetMsg();
	CExcReqParamMissing(CParamFile& p) : CExceptionBase(), m_cParamFile(p) {}

};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcCQueMgrNull : public CExceptionBase
{
private:
	CQue&	m_cQue;
public:
	CExcCQueMgrNull(CQue& q) : CExceptionBase(), m_cQue(q) {}
	virtual char* GetMsg();
};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcNoMoreMsgs : public CExceptionBase
{
private:
	CQue&	m_cQue;
public:
	CQue& GetQue() { return m_cQue; }
	CExcNoMoreMsgs(CQue& q) : CExceptionBase(), m_cQue(q) {}
	virtual char* GetMsg();
};*/





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcNoMoreCtlMsgs : public CExcNoMoreMsgs
{
public:
	CExcNoMoreCtlMsgs(CQue& q) : CExcNoMoreMsgs(q) {}
};*/






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcNoMoreDataMsgs : public CExcNoMoreMsgs
{
public:
	CExcNoMoreDataMsgs(CQue& q) : CExcNoMoreMsgs(q) {}
};*/







//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
/*class CExcBadCtlMsg : public CExceptionBase
{
private:
	CCtlQue& m_cCtlQue;
public:
	CExcBadCtlMsg(CCtlQue& q) : CExceptionBase(), m_cCtlQue(q) {}
	virtual char * GetMsg();
};*/






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadParamTable : public CExceptionBase
{
private:
	CParamFile& m_cParamFile;
public:
	CExcBadParamTable(CParamFile& p) : CExceptionBase(), m_cParamFile(p) {}
	virtual char* GetMsg();
};







//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadPut : public CExceptionBase
{
private:
	CQue& m_cQue;
public:
	CExcBadPut(const char* szLastErr, CQue& q) 
		: CExceptionBase(szLastErr), m_cQue(q) {}
	virtual char* GetMsg();
};






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadMsgID : public CExceptionBase
{
private:
	CQue&	m_cQue;
public:
	CExcBadMsgID(const char* msg, CQue& q)
		: CExceptionBase(msg) , m_cQue(q) {}
	virtual char* GetMsg();
};






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadQueName : public CExceptionBase
{
private:
	CQue&	m_cQue;
public:
	CExcBadQueName(char* msg, CQue& q) : CExceptionBase(msg), m_cQue(q) {}
	virtual char* GetMsg();
};





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadRestart : public CExceptionBase
{
private:
	CDataFile&	m_cDataFile;
	long		m_lRestartRecs;
	long 		m_lRecCount;
public:
	virtual char* GetMsg();
	CExcBadRestart(const char* msg, long count, long expected, CDataFile& df) :
		CExceptionBase(msg), 
		m_cDataFile(df), 
		m_lRecCount(count), 
		m_lRestartRecs(expected) {}
};







//---------------------------------------------------------------------+
//  Class:    CExcLogFileOpenError 
//
// 02/22/99	DMT  Created original version.
//---------------------------------------------------------------------+
class CExcLogFileOpenError : public CExceptionBase
{
private:
	CLogFile& 	m_cLogFile;
	long		m_lLastErrno;
public:
	CExcLogFileOpenError(const char* lerr, long err, CLogFile& lf)
		: CExceptionBase(lerr), m_cLogFile(lf) { m_lLastErrno = err; }
	virtual char* GetMsg();
};







//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcDataFileWriteError : public CExceptionBase
{
private:
	CDataFile& 	m_cDataFile;
	long		m_lLastErrno;
public:
	CExcDataFileWriteError(const char* lerr, long err, CDataFile& df)
		: CExceptionBase(lerr), m_cDataFile(df) { m_lLastErrno = err; }
	virtual char* GetMsg();
};






//---------------------------------------------------------------------+
//  Class:    CExcDataFileOpenError 
//
// 02/23/99	DMT  Created original version.
//---------------------------------------------------------------------+
class CExcDataFileOpenError : public CExceptionBase
{
private:
	CDataFile& 	m_cDataFile;
	long		m_lLastErrno;
public:
	CExcDataFileOpenError(const char* lerr, long err, CDataFile& df)
		: CExceptionBase(lerr), m_cDataFile(df) { m_lLastErrno = err; }
	virtual char* GetMsg();
};






//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcCtlFileWriteError : public CExceptionBase
{
private:
	CCtlFile& 	m_cCtlFile;
	long		m_lLastErrno;
public:
	CExcCtlFileWriteError(const char* lerr, long err, CCtlFile& df)
		: CExceptionBase(lerr), m_cCtlFile(df) { m_lLastErrno = err; }
	virtual char* GetMsg();
};







//---------------------------------------------------------------------+
//  Class:    CExcCtlFileOpenError 
//
// 02/23/99	DMT  Created original version.
//---------------------------------------------------------------------+
class CExcCtlFileOpenError : public CExceptionBase
{
private:
	CCtlFile& 	m_cCtlFile;
	long		m_lLastErrno;
public:
	CExcCtlFileOpenError(const char* lerr, long err, CCtlFile& cf)
		: CExceptionBase(lerr), m_cCtlFile(cf) { m_lLastErrno = err; }
	virtual char* GetMsg();
};





//---------------------------------------------------------------------+
//  Class:    Cxxx
//---------------------------------------------------------------------+
class CExcBadCommitOrRollback : public CExceptionBase
{
private:
	CQueMgr& m_cQM;
public:
	CExcBadCommitOrRollback (const char* msg, CQueMgr& qm) 
		: CExceptionBase(msg), m_cQM(qm) {}
	virtual char* GetMsg();
};

//
//------------------------------------------------------------------------ 
//
// A global C style function to check for a real decimal only string.
//
extern "C" BOOL isdec(const char*);
//
//------------------------------------------------------------------------ 

#endif
