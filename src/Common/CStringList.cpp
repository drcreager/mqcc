/*
 *  Copyright 2000, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 00/01/10	@(#) CStringList.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CStringList.cpp
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Implementation of the CStringList class. 
//
//  History: 
//  01/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "CStringList.h"


//---------------------------------------------------------------------+
//  Method:   clear
//  Desc:     tbd  
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CStringList::clear(){
//
//  allocate a work buffer
//
mp_Element = new CString(25);

//
//  Empty the list 
//
top();
while (!endOfList()){
   pop(mp_Element);
}  /* End While  */

//
//  Release the work buffer and return to caller
//
delete mp_Element;
return 0;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   pop
//  Desc:     Add the item provided by the caller at the end of the
//            collection  
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CStringList::pop(CString* a_Item){
//
//  declare local variables
//
long ll_RC = -1;

//
//  invoke the ancestral next method to retrieve a string
//
mp_Element = (CString*) CCollection::next();

//
//  on_successful, 
//  - copy the string into the caller's buffer
//  - remove the string from the list
//
if (mp_Element != NULL) {
	*a_Item = mp_Element;
	ll_RC = CCollection::remove();
}  /* End If */

return ll_RC;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   push
//  Desc:     Add the item provided by the caller at the end of the
//            collection  
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CStringList::push(char* a_Item){
//
//  allocate a copy of the passed string 
//
mp_Element = new CString(strlen(a_Item));
*mp_Element = a_Item;

//
//  invoke the ancestral add method to copy the string into the list
//
return CCollection::add(mp_Element);
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   push
//  Desc:     Add the item provided by the caller at the end of the
//            collection  
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CStringList::push(CString* a_Item){
//
//  allocate a copy of the passed string 
//
mp_Element = new CString(a_Item->len());
*mp_Element = a_Item;

//
//  invoke the ancestral add method to copy the string into the list
//
return CCollection::add(mp_Element);
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   CStringList
//  Desc:     Default Constructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CStringList::CStringList(){
mp_Element = NULL;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   ~CStringList
//  Desc:     Destructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CStringList::~CStringList(){
}  /* End Method */	
