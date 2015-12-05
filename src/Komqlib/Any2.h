//---------------------------------------------------------------------+
//  File:     CAny2.h
//  Author:   David Tamkun
//  Written:  December 1998
//  Desc:     Declaration file for the CAny2 abstract class.
//
//            This class is derived from CAny and serves as an 
//            ancestor for classes with an Errors Collection. 
//
//  History: 
//  12/28/98  DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __ANY2_H__
#define __ANY2_H__
#include "CAny.h"
#include "Errors.h"
#include "KOConstants.h"


//---------------------------------------------------------------------+
//  Class:    CAny2 derived from CAny
//---------------------------------------------------------------------+
class CAny2 : public CAny
{
  public:
      CAny2(void);
      CAny2(const CAny2 &right);
      ~CAny2(void);


      virtual long GetFirstError (int& ri_handle,
		             long& rl_error_number, long& rl_severity,
		             CString& rs_error_msg,
					 CString& rs_location,
					 CString& rs_context) const;

      virtual long GetNextError (int& ri_handle,
		             long& rl_error_number, long& rl_severity,
		             CString& rs_error_msg,
					 CString& rs_location,
					 CString& rs_context) const;

      virtual long GetErrorCount (void) const;

	  virtual long CopyErrors(CAny2* iu_object);

  protected:
	  // Consider Pointer to Class?
      CErrors	iu_errors;
	  CString	is_InstanceName;

};
#endif


