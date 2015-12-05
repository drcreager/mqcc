/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/10/07	@(#) CItem.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CItem.cpp
//  Author:   Daniel R Creager
//  Written:  October 1999
//  Desc:     Implementation of the CItem class. 
//
//  History: 
//  10/05/99  DRC Created original code.
//---------------------------------------------------------------------+
#include "CItem.h"


//---------------------------------------------------------------------+
//  Method:   appendLink
//  Desc:     append myself into a doubly linked chain 
//            after a specified link. 
//  Args:     None.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CItem::appendLink(CItem* a_Cursor){

long ll_RC = 0;

if (isValidPointer(a_Cursor->mp_Next)) {
	if (isValidPointer(a_Cursor->mp_Next->mp_Prev)){
		//
		//  Construct the pointer chain links within me
		//
		mp_Next = a_Cursor->mp_Next;
		mp_Prev = a_Cursor->mp_Next->mp_Prev;

		//
		//  Modify forward and backward links to append me into the 
		//  existing pointer chain
		//
		a_Cursor->mp_Next = this;
		mp_Next->mp_Prev  = this;
	}  
	else {
		ll_RC = -2;
	}  /* End If  */
}
else {
	ll_RC = -1;
}  /* End If  */

return ll_RC;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   CItem  (Constructor)
//  Desc:     Sets up the pointers for this item 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CItem::CItem(void* a_Item){
mp_Item  = a_Item;
mp_Next  = NULL;
mp_Prev  = NULL;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   ~CItem  (Destructor)
//  Desc:     Clear the pointers in this item 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CItem::~CItem(){
mp_Item  = NULL;
mp_Next  = NULL;
mp_Prev  = NULL;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   insertLink
//  Desc:     insert myself into a doubly linked chain 
//            before a specified link. 
//  Args:     None.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CItem::insertLink(CItem* a_Cursor){

long ll_RC = 0;

if (isValidPointer(a_Cursor->mp_Prev)) { 
	if (isValidPointer(a_Cursor->mp_Prev->mp_Next)){
		//
		//  Construct the pointer chain links within me
		//
		mp_Prev = a_Cursor->mp_Prev;
		mp_Next = a_Cursor->mp_Prev->mp_Next;

		//
		//  Modify forward and backward links to insert me into the existing 
		//  pointer chain
		//
		a_Cursor->mp_Prev = this;
		mp_Prev->mp_Next  = this;
	}  
	else {
		ll_RC = -2;
	}  /* End If  */
}  
else {
	ll_RC = -1;
}  /* End If  */

return ll_RC;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   operator=  (Assignment Operator)
//  Desc:     Give a pointer to the data element back to the caller. 
//  Args:     An item pointer.
//  Returns:  Data Pointer
//---------------------------------------------------------------------+
void* CItem::operator= (CItem& a_Item){
return a_Item.mp_Item;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   removeLink
//  Desc:     Unlink myself from a doubly linked chain. 
//  Args:     None.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CItem::removeLink(){

long ll_RC = 0;

if (isValidPointer(mp_Next)){
	if (isValidPointer(mp_Prev)){
		//
		//  Modify forward and backward links in the 
		//  existing pointer chain
		//
		mp_Next->mp_Prev  = mp_Prev;
		mp_Prev->mp_Next  = mp_Next;
		mp_Prev           = NULL;
		mp_Next           = NULL;
	}  
	else {
		ll_RC = -2;
	}  /* End If  */  
}
else {
	ll_RC = -1;
}  /* End If  */

return ll_RC;
}  /* End Method */



