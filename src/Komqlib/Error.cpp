//---------------------------------------------------------------------+
//  File:     CError.cpp
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     This file contains the implementation of the CError2
//            Class.
//
//            This class represents an Error in the Errors Collection
//            class in one of the Utility (get/put/dlq) programs.
//  
//  Dev Log: 
//  History: 
//  12/28/98  DMT Created original version.
//  01/21/99  DMT Renamed this class from CError to CError2 to avoid 
//                conflict with CError class in common library.  This 
//                is an issue on AIX.
//---------------------------------------------------------------------+
#include "Error.h"





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
CError2::CError2() : il_error_number(0), 
                     il_severity(0) {
}




//---------------------------------------------------------------------+
//  Method:   Copy Constructor
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
CError2::CError2(const CError2 &right) : il_error_number(0), 
                                      il_severity(0) {

	il_error_number = right.il_error_number;
    il_severity     = right.il_severity;
    is_error_msg    = right.is_error_msg;
	is_location		= right.is_location;
	is_context		= right.is_context;
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
CError2::~CError2() {

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
CError2& CError2::operator=(const CError2 &right) {

	//  Make sure we aren't trying to set an object equal to itself.
	if(this == &right) {
		return *this;
	}

	// copy members
	il_error_number = right.il_error_number;
    il_severity     = right.il_severity;
    is_error_msg    = right.is_error_msg;

	return *this;
}


