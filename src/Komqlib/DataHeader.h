//---------------------------------------------------------------------+
//  File:     DataHeader.h
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Declaration file for the CDataHeader class.  
//            CDataHeader encapsulates the knowledge of how to construct
//            and subsequently interpret the XML based header information 
//            that is inserted into/removed from data files modified by 
//            any of the File Modification Services.
//
//  History: 
//  11/10/00 DRC Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#define FWD_SLASH  '/'
#define BKWD_SLASH '\\'
#define WIN_DLMTR  BKWD_SLASH 
#define UNIX_DLMTR FWD_SLASH 

#include "Any2.h"
#include "CBuffer.h"
#include "CCollection.h"
#include "CFileList.h"
#include "CItem.h"
#include "CString.h"
#include "CStringList.h"
#include "Errors.h"
#include "FileNameSrvc.h"
#include "ExtSrvcsParamFile.h"
#include "KOConstants.h"
#include <time.h>



typedef enum {
	TypeLong,
	TypeString,
	TypeGroup,
	TypeEndGrp,
	TypeList,
	TypeEndLst,
	TypeBegMap,
	TypeEndMap,
	TypeEndTbl
} EDataType;



class CDataHeader : public CAny2 {

typedef struct _sXMLDocLayout {
	char*       szTagName;
	EDataType	eDataType;
	void*		pDataValue;
} SXMLDocLayout;

private:
	static SXMLDocLayout sXMLDocLayout[];
	static const char*   m_szOS;
	static const char*   m_szProgram;
	static const char*   m_szVersion;
	static const char*   m_szSetName;
	static const char*   m_szSetClass;
	static       long    m_lFileSeq;
	static const char*   m_szSetFileName;
	static const char*   m_szXfrTime;
	static const char*   m_szSrcFileName;
	static const char*   m_szSrcFileClass;
	static const char*   m_szFileFormat;
	static       long    m_lBlkLen;
	static       long    m_lRcrdLen;
	static       long    m_lRcrdCnt;

protected:
    const SXMLDocLayout*	GetDocLayout();
	CFileList*				m_pcFileList;
    char					m_szToday[20];
	char*					m_pCursor;
	long					m_lXfrLen;
	BOOL					m_bEOF;
	CString					m_XMLDoc;
	CString					m_szWork;
	CCollection				m_Stack;
	CString					m_szStack;
	CFileNameSrvc*			m_pcFNameSrvc;

public:
	CDataHeader(   CExtSrvcsParamFile* a_pcParms);
	virtual ~CDataHeader();
	virtual char*  chngFileDlmtrs(char* m_szFileName);
      virtual void   clear();
      virtual char*  ConvDlmtrs(char* a_szData);
	BOOL eof();
	long getline(char*, long);
	//CDataHeader& operator<< (CDataHeader&, char*);

	virtual char*  getConvertedFileName(char* a_szFileName);
	virtual char*  getOS();
	virtual char*  getProgram();
	virtual char*  getVersion();
	virtual char*  getSetName();
	virtual char*  getSetClass();
	virtual long   getFileSeq();
	virtual char*  getSetFileName();
	virtual CFileList*  getSetList();
	virtual char*  getXfrTime();
	virtual char*  getSrcFileName();
	virtual char*  getSrcFileClass();
	virtual char*  getFileFormat();
	virtual long   getBlkLen();
	virtual long   getRcrdLen();
	virtual long   getRcrdCnt();

	virtual BOOL   isBlocked();
	virtual BOOL   isLocalOS();
	virtual BOOL   isFileSetComplete();
	virtual BOOL   isHeaderComplete();
	virtual void   LoadXML(void);
	virtual char*  ParseXML(char*		a_pcBuffer,
						  char*			a_pcQualifier, 
		                  CStringList*	a_pcResult,
						  char*         a_szQualDlmtr);

	virtual void   Pop();
	virtual void   Push();
	virtual long   Read(CBuffer& cBuffer);
	virtual void   setOS(char* a_szArg);
	virtual void   setProgram(char* a_szArg);
	virtual void   setVersion(char* a_szArg);
	virtual void   setSetName(char* a_szArg);
	virtual void   setSetClass(char* a_szArg);
	virtual void   setSetList(CFileList* a_pcArg);
	virtual void   setXfrTime(char* a_szArg);
	virtual void   setSrcFileName(char* a_szArg);
	virtual void   setSrcFileClass(char* a_szArg);
	virtual void   setFileFormat(char* a_szArg);
	virtual void   setBlkLen(long a_lArg);
	virtual void   setRcrdLen(long a_lArg);
	virtual void   setRcrdCnt(long a_lArg);
	virtual long   setString( char** pszDest, 
		                     char*  pszSource);

	virtual void   setXMLAttribute(CString* a_szEntry);
	virtual void   UnloadXML(void);
	virtual long   Write(CBuffer& cBuffer);


};
