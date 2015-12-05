//---------------------------------------------------------------------+
//  File:     CBlockingSrvc.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CBlockingSrvc class.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "FileModificationSrvc.h"
#include "DataHeader.h"
#include "CFileList.h"
#include <stdio.h>

class CBlockingSrvc : public CFileModificationSrvc  {
protected:
    CDataHeader* m_pcDataHdr;

public:
	CBlockingSrvc(CExtSrvcsParamFile* a_pcParms);
	virtual ~CBlockingSrvc();
	virtual long Block(  char* a_szFileName, long            a_lBlkLen, 
		                 long  a_lRcrdLen,   CFileList*      a_pcFileList,
						 BOOL  a_bReplace);
	virtual long Block(  char* a_szFileName, BOOL  a_bReplace);
	virtual long Block(  char* a_szFileName, CFileList*      a_pcFileList,
						 BOOL  a_bReplace);
	virtual long Block(  char* a_szFileName, long a_lBlkLen, 
		                 long a_lRcrdLen,    BOOL  a_bReplace);
	virtual long Deblock(char* a_szFileName, long a_lBlkLen, 
		                 long a_lRcrdLen,    BOOL  a_bReplace,    BOOL  a_bRename);
	virtual long Deblock(char* a_szFileName, BOOL  a_bReplace,    BOOL  a_bRename);
	virtual long GetFileAttributes(
		                 char* a_szFileName, CDataHeader* a_pcDataHdr);
	virtual long getRcrdCount();
	virtual char* getTempFileName();
	virtual long initSelf(char* a_szSrcFileName, char* a_szDstFileName);
	virtual char* setTempFileName(char* a_szDir);
};

