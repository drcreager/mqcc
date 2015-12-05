//---------------------------------------------------------------------+
//  File:     CFileModificationSrvc.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration of the CFileModificationSrvc class.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
 #pragma once
#else
 #pragma options once
#endif

#include "Any2.h"
#include "CFile.h" 
#include "CBuffer.h"
#include "CSysCmd.h"
#include "ExtSrvcsParamFile.h"
#include "DataHeader.h"
#include "FileNameSrvc.h"
#include <stdlib.h>
#include <errno.h>



class CFileModificationSrvc : public CAny2  
{
protected:
	CExtSrvcsParamFile*	   m_pcParms;
	CFileNameSrvc*         m_pcFNameSrvc;
	CSysCmd*               m_pcSysCmd;
	CFile*	 m_pFile1;
	CFile*	 m_pFile2;
	CBuffer* m_pBuf1;
	CBuffer* m_pBuf2;
	CString  m_szWork;
	long	 m_lRC;


public:
	virtual long  Archive(     char*        a_szFileName);
	              CFileModificationSrvc(CExtSrvcsParamFile* a_pcParms);
	virtual       ~CFileModificationSrvc();
	virtual long  CopyToMQS(   CDataHeader* a_pcFileHdr);
//	virtual long  CopyToHost(  CDataHeader* a_pcFileHdr);
    virtual long  Copy();
    virtual long  Delete(      char*        a_szFileName);
	virtual long  ExtractHdr(  CDataHeader* a_pcFileHdr);
    virtual long  getBlkCount( CFile*       a_pcFile);
    virtual long  getRcrdCount(CFile*       a_pcFile);
	virtual long  initSrcFile( char*        a_szFileName );
    virtual long  initDstFile( char*        a_szFileName );
    virtual long  initSelf(    char*        a_szSrcFileName,   
		                       char*        a_szDstFileName);
	virtual long  InsertHdr(   CDataHeader* a_pcFileHdr);
    virtual long  MakeDir(     char*        a_szPathName);   
    virtual long  Rename(      CString*     a_szOldFileName,   
		                       CString*     a_szNewFileName);
    virtual long  Rename(      char*        a_szOldFileName,   
		                       char*        a_szNewFileName);
	virtual long  termSrcFile();
    virtual long  termDstFile();
    virtual long  termSelf();

};
