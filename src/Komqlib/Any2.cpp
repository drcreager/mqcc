//---------------------------------------------------------------------+
//  File:     CAny2.cpp
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     This file contains the implementation of the CAny2
//            Class.
//
//            This class is derived from CAny and serves as an 
//            ancestor for classes with an Errors Collection. 
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
#include "Any2.h"





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CAny2::CAny2(void) {

	is_InstanceName = "Unspecified Object";

}





//---------------------------------------------------------------------+
//  Method:   Copy Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:  NOT IMPLEMENTED!!!!!
//
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CAny2::CAny2(const CAny2 &right) {

}





//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CAny2::~CAny2(void) {

}





//---------------------------------------------------------------------+
//  Method:   GetFirstError
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Returns attributes of the first error in the objects
//            errors collection.  Also initializes a handle to be used
//            for subsequent calls to GetNextError.
//
//  Args:     O  ri_handle        Handle to be used on subsequent calls
//                                GetNextError, allows caller to step
//                                through all errors.
//
//            O  rl_error_number  Error number of the error.
//
//            O  rl_severity      Severity of the error. 1 = Warning,
//                                2 = Severe Error.
//
//            O  rs_error_msg     Error Message Text
//
//  Returns:  1 if the errors collection contains an error
//            0 if there are no errors.
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
long CAny2::GetFirstError (int& ri_handle, 
						   long& rl_error_number, long& rl_severity,
		             CString& rs_error_msg,
					 CString& rs_location,
					 CString& rs_context) const {

	return iu_errors.GetFirstError(ri_handle, rl_error_number, 
		                           rl_severity, rs_error_msg,
								   rs_location, rs_context);
}





//---------------------------------------------------------------------+
//  Method:   GetNextError
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Returns attributes of the next error in the object's
//            errors collection.  ri_handle is used by the Errors 
//            collection to determine what the "next" error is and
//            must be initialized by calling GetFirstError before
//            calling GetNextError.
//
//  Args:     IO  ri_handle       Handle that indicates what the "next"
//                                error is.  Must be initialized by an
//                                initial call to GetFirstError and is
//                                modified by subsequent calls to 
//                                GetNextError.  Allows caller to step
//                                through all errors in the collection.
//
//            O  rl_error_number  Error number of the error.
//
//            O  rl_severity      Severity of the error. 1 = Warning,
//                                2 = Severe Error.
//
//            O  rs_error_msg     Error Message Text
//
//  Returns:  1 if the errors collection contains an error
//            0 if there are no errors or if ri_handle is invalid.
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
long CAny2::GetNextError (int& ri_handle, 
						  long& rl_error_number, long& rl_severity,
		             CString& rs_error_msg,
					 CString& rs_location,
					 CString& rs_context) const
{
	return iu_errors.GetNextError(ri_handle, rl_error_number, 
		                           rl_severity, rs_error_msg,
								   rs_location, rs_context);
}





//---------------------------------------------------------------------+
//  Method:   GetErrorCount
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Returns the number of errors in the object's
//            errors collection. 
//
//  Args:     None
//
//  Returns:  the number of errors in the object's
//            errors collection. 
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
long CAny2::GetErrorCount (void) const
{
	return iu_errors.GetErrorCount();
}





//---------------------------------------------------------------------+
//  Method:   CopyErrors
//  Author:   David M Tamkun
//  Written:  January 1999
//
//  Desc:     Copies Errors from the specified Objects Collection into
//            This Objects Collection. 
//
//  Args:     Pointer to an object that's a descendant of CAny2.
//
//  Returns:  Always Returns 0
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
long CAny2::CopyErrors(CAny2* ru_object) {


	int		li_handle		= 0;
	long	ll_error_number	= 0;
	long	ll_severity		= 0;
	CString	ls_error_msg(132);
	CString ls_location(132);
	CString	ls_context(132);

	if(ru_object->GetErrorCount() >= 1) {

		ru_object->GetFirstError(li_handle,   ll_error_number,
			                     ll_severity, ls_error_msg,
								 ls_location, ls_context);

		iu_errors.AddError(ll_error_number,
			               ll_severity, ls_error_msg,
						   ls_location, ls_context);

		while(ru_object->GetNextError(li_handle,   ll_error_number,
			                           ll_severity, ls_error_msg,
								       ls_location, ls_context)) {

			iu_errors.AddError(ll_error_number,
			                   ll_severity, ls_error_msg,
							   ls_location, ls_context);

		}

	}
	return 0;
}