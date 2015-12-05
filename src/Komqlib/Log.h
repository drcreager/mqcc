//---------------------------------------------------------------------+
//  File:     CLog.h
//  Author:   David Tamkun
//  Written:  December 1998
//  Desc:     Declaration file for the CLog class.
//
//            The class logs information to the console and or log
//            file.  At on time, it also supported "logging" to 
//            an error queue.
//  
//
//  TechNote: Specialized setup requirements exist for this class due 
//            to the use of cross-aggregation.  Refer to the documentation
//            in RespBatch.h for more details.    DRC, April 2000
//
//  History: 
//  12/28/98  DMT Created original code.
//  02/12/00  DRC Added RespBatch pointer and the setRespBatch method.
//  02/21/01  DRC Modified inheritence from ofstream to CFile.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __LOG_H__
#define __LOG_H__

//#include <fstream.h>
#include "CFile.h"
#include <errno.h>
#include <time.h>
#include "Any2.h"
#include "CString.h"

class CRespBatch;

#define LOGCONSOLE	1
#define LOGFILE		2
#define LOGDEFAULT  3
#define LOGQUEUE	4

//---------------------------------------------------------------------+
//  Class:    CLog derived from CFile and CAny2
//---------------------------------------------------------------------+
class CLog : public CFile, public CAny2
{


  public:
	  virtual long Banner(    CString* a_sAppName, 
				              CString* a_sAppVersion, 
				              CString* a_sAppDesc,
				              int      a_iEntryType); 
                   CLog();
                   CLog(const CLog &right);
                   ~CLog();

	  virtual long Commit();
	  virtual long initSelf(  CString& vs_filename);
	  virtual long initSelf(  char*    vs_filename);
	  virtual long LogError ( long     vl_error_number, long vl_severity,
		                      CString& vs_msg, 
							  char*	   vs_location,
							  char*    vs_context,
							  int      vi_entry_type = LOGDEFAULT);
	  
	  virtual long LogError ( long     vl_error_number, long vl_severity,
		                      char*    sz_msg, 
							  char*	   vs_location,
							  char*    vs_context,
							  int      vi_entry_type = LOGDEFAULT);

	  virtual long LogErrors( CAny2&   cAny2,
							  int      vi_entry_type = LOGDEFAULT);

              CLog& operator=(const CLog &right);
	  virtual void  setRespBatch(CRespBatch* a_pcRespBatch);
	  virtual long  termSelf( void);
	  virtual void  TranslateErrno (CString& rs_msg, 
		                      int vi_errno);

	          void  WrapLine( CString& s_out, 
				              char*    sz_in, 
							  uint     i_lw, 
							  char*    sz_lp);

      virtual long WriteLog ( CString& rs_entry, 
		                      int      vi_entry_type = LOGDEFAULT);

      virtual long WriteLog ( char*    rs_entry, 
		                      int      vi_entry_type = LOGDEFAULT);

//	  virtual long initQueue(KOMQAPI* vu_mqapi, 
//	                         CString& vs_queuename);
//	  virtual long termQueue();

	  virtual long WriteRecord(CString& rs_record);
	  virtual long WriteRecord(char*    vs_record);

  protected:
      BOOL		   m_bOpen;
	  CString	   m_sFileName;
      CRespBatch*  m_pcRespBatch;



  private:
//	  CQueue2	   m_cQueue;
	  BOOL		   m_bQueueOpen;

};

#endif



