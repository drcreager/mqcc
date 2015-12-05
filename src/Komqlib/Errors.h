//---------------------------------------------------------------------+
//  File:     CErrors.h
//  Author:   David Tamkun
//  Written:  December 1998
//  Desc:     Declaration file for the CErrors class.
//
//            Represents a Collection of Errors.
//
//            The maximum number of errors that can be contained is
//            #defined as MAXERRORS below.
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
//
//  History: 
//  12/28/98  DMT Created original code.
//  01/21/99  DMT Renamed CError to CError2 to avoid 
//                conflict with CError class in common library.  This 
//                is an issue on AIX.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif



#ifndef __ERRORS_H__
#define __ERRORS_H__

#include "Error.h"





//---------------------------------------------------------------------+
//  Define the Maximum Number of Errors that the collection can 
//  contain.
//---------------------------------------------------------------------+
#define MAXERRORS	100






//---------------------------------------------------------------------+
//  Class:    CErrors
//---------------------------------------------------------------------+
class CErrors 
{

  public:
      CErrors(void);
      CErrors(const CErrors &right);

      ~CErrors();

      virtual CErrors& operator= (const CErrors& right);
	  
	  //virtual long AddError (long vl_error_number, long vl_severity, 
	  //             const CString& vs_error_msg);

	  virtual long AddError (long vl_error_number, long vl_severity, 
		             const CString& vs_error_msg,
					 const CString& vs_location,
					 const CString& vs_context);


      virtual long GetFirstError (int& rl_handle, long& rl_error_number, 
					long& rl_severity, CString& rs_error_msg,
					CString& rs_location, 
					CString& rs_context) const;

      virtual BOOL HasError (void) const;

      virtual long GetNextError (int& rl_handle, long& rl_error_number, 
					long& rl_severity, CString& rs_error_msg,
					CString& rs_location, 
					CString& rs_context) const;

      virtual int GetErrorCount (void) const;

      virtual void ClearErrors (void);

   private:
      CError2* iu_errors[MAXERRORS];
      int ii_num_errors;
};

#endif
