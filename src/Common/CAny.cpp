/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CAny.C 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CAny.cpp
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Implementation of the CAny class. 
//
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
#include "CAny.h"

//---------------------------------------------------------------------+
//  Method:   CAny::CAny
//  Desc:     Class Constructor Method. 
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
CAny::CAny(void)
{
return;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   CAny::CAny
//  Desc:     Class Destructor Method. 
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
CAny::~CAny(void)
{
return;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   CAny::initSelf
//  Desc:     TBD. 
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//  03/20/00  DRC Added the isValidPointer method.
//---------------------------------------------------------------------+
long CAny::initSelf(void)
{
return 0;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   isValidPointer
//  Desc:     Validate the contents of a pointer.
//  Args:     aPointer.
//  Returns:  BOOL   True  = Pointer is valid 
//                   False = Pointer is invalid 
//---------------------------------------------------------------------+
BOOL CAny::isValidPointer(const void* a_pArg){
#define _Method01_
#ifdef  _Method01_ 
	return (((const unsigned int) a_pArg != NULL) 
	&&  ((const unsigned int) a_pArg != 0xdddddddd)
	&&  ((const unsigned int) a_pArg != 0xcdcdcdcd));

#endif
#ifdef  _Method02_ 
BOOL lb_Result;
try {
	//
	//  the compiler will not accept the indirection
	//  operator with a void*.   DRC 3/20/2000
	//
	lb_Result = (*a_pArg == *a_pArg);
}

catch (...) {
	lb_Result = false;
}
return lb_Result;
#endif

}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   CAny::termSelf
//  Desc:     TBD. 
//
//  Parms:    None.
//  Returns:  Nothing.
// 
//  History: 
//  03/28/96  DRC Created original code.
//---------------------------------------------------------------------+
long CAny::termSelf(void)
{
return 0;
}  /* End Method */








