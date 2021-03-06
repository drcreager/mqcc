//---------------------------------------------------------------------+
//  File:     CErrors.cpp
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     This file contains the implementation of the CErrors
//            Class.
//  
//            Represents a Collection of Errors.
//
//  Dev Log:  Currently, collection contains an array of pointers to 
//            CError objects.  This array has a hardcoded upper 
//            limit.  In the future, change this to a linked list
//            to remove this constraint.
//
//  History: 
//  12/28/98  DMT Created original version.
//  01/21/99  DMT Renamed CError to CError2 to avoid 
//                conflict with CError class in common library.  This 
//                is an issue on AIX.
//---------------------------------------------------------------------+

#include "Errors.h"





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
CErrors::CErrors(void) : ii_num_errors(0) {

	int li_counter = 0;

	for(li_counter = 0; li_counter < MAXERRORS; li_counter++) {
		iu_errors[li_counter] = NULL;
	} 
}






//---------------------------------------------------------------------+
//  Method:   Copy Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:  NOT FULLY IMPLEMENTED!!!!!
//
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CErrors::CErrors(const CErrors &right) : ii_num_errors(0) {

	int li_counter = 0;

	for(li_counter = 0; li_counter < MAXERRORS; li_counter++) {
		iu_errors[li_counter] = NULL;
	} 

	*this = right;
}






//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Calls ClearErrors to free all allocated Error Objects.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CErrors::~CErrors() {

	ClearErrors();
}





//---------------------------------------------------------------------+
//  Method:   Assignment Operator
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
CErrors& CErrors::operator= (const CErrors& right) {
	int     li_error_handle = 0;
	long    ll_error_number = 0;
	long    ll_severity     = 0;
	CString ls_error_msg(255);
	CString ls_location(255);
	CString ls_context(255);


	if(this == &right) { 
		// Make sure we aren't trying to set this class 
		// equal to itself.
		return *this;
	}
	else {
		// Delete Any Existing Errors
		ClearErrors();

		long l_test = 0;

		if(right.GetFirstError(li_error_handle, ll_error_number, 
							   ll_severity, ls_error_msg,
							   ls_location, ls_context)) {

			this->AddError(ll_error_number, ll_severity, ls_error_msg,
				           ls_location, ls_context);

			while(right.GetNextError(li_error_handle, ll_error_number, 
				                     ll_severity, ls_error_msg,
									 ls_location, ls_context)) {

				this->AddError(ll_error_number, ll_severity, 
							   ls_error_msg,
							   ls_location, ls_context);
			}
		}

		return *this;
	}
}





//---------------------------------------------------------------------+
//  Method:   AddError
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Adds an Error to the Error Collection
//
//            Not that if this fails, we cannot add an Error to the 
//            Error Collection because the call will probably fail
//            again.
//
//  Args:     I long	Error Number
//            I long	Error Severity, suggest: 1 = warning, 2 = severe
//            I CString	Error Message
//
//  Returns:  1 if successful, 
//            0 if an Error Occurs, ie Memory Allocation Fails or
//              the collection is "Full".
//  
//  Dev Log:  In other classes 0 = good, > 0 = problem.  This  is
//            consistent with other classes, but handy for nesting
//            in an if or while statement.
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
long CErrors::AddError (long vl_error_number, long vl_severity, 
						const CString& vs_error_msg,
						const CString& vs_location,
						const CString& vs_context)
{
	int      li_index = 0;
	CError2* lu_error = NULL;

	while((iu_errors[li_index] != NULL) &&
		  (li_index             < MAXERRORS)) {
		li_index += 1;
	}

	if(li_index < MAXERRORS) {
		// free "slot" for an error

		lu_error = new CError2;

		if (lu_error != NULL) {

			iu_errors[li_index] = lu_error;

			iu_errors[li_index]->il_error_number = vl_error_number;
			iu_errors[li_index]->il_severity     = vl_severity;
			iu_errors[li_index]->is_error_msg    = vs_error_msg;
			iu_errors[li_index]->is_location     = vs_location;
			iu_errors[li_index]->is_context      = vs_context;


			ii_num_errors += 1;

			return 1;
		}
		else {
			// memory allocation failed
			return 0;
		}
	}
	else {
		// Collection is FULL
		return 0;
	}
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
long CErrors::GetFirstError (int& ri_handle,    long& rl_error_number, 
							 long& rl_severity, 
							 CString& rs_error_msg,
							 CString& rs_location,
							 CString& rs_context) const {

	ri_handle = 0;

	if(iu_errors[ri_handle] != NULL) {
		rl_error_number = iu_errors[ri_handle]->il_error_number;
        rl_severity     = iu_errors[ri_handle]->il_severity;
        rs_error_msg    = iu_errors[ri_handle]->is_error_msg;
		rs_location     = iu_errors[ri_handle]->is_location;
		rs_context      = iu_errors[ri_handle]->is_context;

		return 1;
	}
	else {
		return 0;
	}  
}





//---------------------------------------------------------------------+
//  Method:   HasError
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Returns True if the collection contains an Error, False
//            if not.
//
//  Args:     None
//
//  Returns:  True if the errors collection contains an error
//            False if there are no errors.
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
BOOL CErrors::HasError (void) const
{
	return (ii_num_errors > 0);
 
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
long CErrors::GetNextError (int& ri_handle,    long& rl_error_number, 
							long& rl_severity, 
							CString& rs_error_msg,
							CString& rs_location,
							CString& rs_context) const {

	ri_handle += 1;

	if((ri_handle < 0) ||
	   (ri_handle >= MAXERRORS)) {
		return 0;
	}
	else {
		if(iu_errors[ri_handle] != NULL) {
			rl_error_number = iu_errors[ri_handle]->il_error_number;
			rl_severity     = iu_errors[ri_handle]->il_severity;
			rs_error_msg    = iu_errors[ri_handle]->is_error_msg;
			rs_location     = iu_errors[ri_handle]->is_location;
			rs_context      = iu_errors[ri_handle]->is_context;

			return 1;
		}
		else {
			return 0;
		}  
	}
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
int CErrors::GetErrorCount (void) const {

	return ii_num_errors;
}





//---------------------------------------------------------------------+
//  Method:   ClearErrors
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Removes all errors in the Errors Collection and resets
//            Error Counter.
//
//            NOTE!!! This is called by the Destructor, so it MUST
//                    free all allocated memory.
//
//  Args:     None
//
//  Returns:  Nothing 
//  
//  Dev Log: 
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
void CErrors::ClearErrors (void) {

 	int li_counter = 0;

	for(li_counter = 0; li_counter < MAXERRORS; li_counter++) {

		if(iu_errors[li_counter] != NULL) {
			delete iu_errors[li_counter];
			iu_errors[li_counter] = NULL;
		}
	}

	ii_num_errors		= 0;
}


