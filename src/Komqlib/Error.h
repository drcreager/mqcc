//---------------------------------------------------------------------+
//  File:     CError.h
//  Author:   David Tamkun
//  Written:  December 1998
//  Desc:     Declaration file for the CError2 class.
//
//            This class represents an Error in the Errors Collection
//            class in one of the Utility (get/put/dlq) programs.
//  
//            Represents an individual error.
//
//            Whenever an object encounters an error, it adds an error
//	          to it's error collection and returns a return code to
//	          its caller indicating that that there is an error.
//
//	          When the caller gets the return code, it calls another
//	          method on the callee asking it to return its Errors, ie
//	          GetFirstError, GetNextError, etc.  This process
//	          continues across layers, so if an error occurrs several
//	          layers deep, they all get returned to the highest layer.
//
//  History: 
//  12/28/98  DMT Created original code.
//  01/21/99  DMT Renamed this class from CError to CError2 to avoid 
//                conflict with CError class in common library.  This 
//                is an issue on AIX.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif


#ifndef __ERROR_H__
#define __ERROR_H__

//#ifndef _CString
//#define _CString
#include "CString.h"
//#endif



//---------------------------------------------------------------------+
//  Class:    CError2 - no ancestor
//---------------------------------------------------------------------+
class CError2 
{
  public:
      CError2(void);
      CError2(const CError2 &right);
      ~CError2(void);

      virtual CError2& operator=(const CError2 &right);

      long il_error_number;
      long il_severity;
      CString is_error_msg;
	  CString is_location;
	  CString is_context;
};

#endif
