/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/10/07	@(#) CCollection.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CCollection.cpp
//  Author:   Daniel R Creager
//  Written:  October 1999
//  Desc:     Implementation of the CCollection class. 
//
//  Dev Log: 
//       01)  It would be a good idea to support multiple concurrent 
//            cursors. To do this will require developing a notification 
//            mechanism so that CCollection can notify all instances of 
//            CCursor whenever an addition or deletion occurs and the 
//            CCursor can take the appropriate action to preserve its 
//            integrity.                             D. Creager 
//
//  History: 
//  10/05/99  DRC Created original code.
//---------------------------------------------------------------------+
#include "CCollection.h"



//---------------------------------------------------------------------+
//  Method:   add
//  Desc:     Add the item provided by the caller at the end of the
//            collection  
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CCollection::add(CItem* a_Item){

//
//  On First_Entry, Initialize the Collection 
//
if (isValidPointer(mp_Last)){
	//
	//  Construct the pointer chain links within the new Item
	//
	a_Item->appendLink(mp_Last);

}
else {
	initHeader(a_Item, mp_Last);
} /* End If     */

mp_Last                  = a_Item;
ml_Count++;
return 0;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   add
//  Desc:     Add the data element provided by the caller at the end of 
//            the collection. 
//  Args:     Address of the new data element.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CCollection::add(void* a_Arg){
return add(new CItem(a_Arg));
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   bottom
//  Desc:     Reset the cursor to the last item of the collection and
//            establish a backward processing direction. 
//  Args:     None.
//  Returns:  The last data element in the collection.
//---------------------------------------------------------------------+
void* CCollection::bottom(){
if (mp_isChanged) resetHeader();
return mp_Cursor->bottom(mp_Last, ml_Count);
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   CCollection
//  Desc:     Default Constructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CCollection::CCollection(){
mp_Cursor      = new CCursor;
mp_PrevCursor  = new CCursor;
mp_First       = NULL;
mp_Last        = NULL;
ml_Count       = 0;
mp_isChanged   = False;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   ~CCollection
//  Desc:     Destructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CCollection::~CCollection(){
delete mp_Cursor;
delete mp_PrevCursor;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   clear
//  Desc:     Release all items from the collection. 
//  Args:     None.
//  Returns:  Return Code.
//---------------------------------------------------------------------+
long CCollection::clear(){
mp_Cursor->top(mp_First, ml_Count);
while (!mp_Cursor->endOfList()){ 
	remove(); 
}  /* End While */

clearHeader();
return 0;
}  /* End Method */







//---------------------------------------------------------------------+
//  Method:   clearHeader
//  Desc:     Null out all pointers in the collection header.
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CCollection::clearHeader(){
mp_First   = NULL;
mp_Last    = NULL;
mp_Cursor->top(mp_First, ml_Count);
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   count
//  Desc:     Returns the number of items in the collection. 
//  Args:     None.
//  Returns:  The number of items in the collection.
//---------------------------------------------------------------------+
long CCollection::count(){
return ml_Count;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   emptyList
//  Desc:     Indicates an empty list. 
//  Args:     None.
//  Returns:  Boolean indicator. 
//---------------------------------------------------------------------+
BOOL CCollection::emptyList(){
return (ml_Count == 0);
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   endOfList
//  Desc:     Indicates end of the List. 
//  Args:     None.
//  Returns:  Boolean indicator. 
//---------------------------------------------------------------------+
BOOL CCollection::endOfList(){
return mp_Cursor->endOfList();
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   get
//  Desc:     Retrieve the item currently pointed to by the cursor. 
//  Args:     None.
//  Returns:  current data element in the collection.
//---------------------------------------------------------------------+
void* CCollection::get(){
return mp_Cursor->getData();
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:  initHeader
//  Desc:    Initialize the header portion of this collection.
//  Args:    Address of the new item.
//           Address of the exiting item.
//  Returns: Nothing
//---------------------------------------------------------------------+
void CCollection::initHeader(CItem* a_Item, CItem* a_Location){
a_Location      = a_Item;
mp_First        = a_Item;
mp_Last         = a_Item;
a_Item->mp_Prev = a_Item;
a_Item->mp_Next = a_Item;
mp_Cursor->top(mp_First, ml_Count);
} /* End Method */



//---------------------------------------------------------------------+
//  Method:   insert
//  Desc:     Add the item provided by the caller into the collection 
//            immediately preceding the current cursor position. 
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CCollection::insert(CItem* a_Item){

//
//  On First_Entry, Initialize the Collection 
//
if (isValidPointer(mp_Cursor->get())){
	//
	//  Construct the pointer chain links within the new Item
	//
	a_Item->insertLink(mp_Cursor->get());
}
else {
	initHeader(a_Item, mp_Cursor->get());
} /* End If     */


mp_isChanged = True;
ml_Count++;
return 0;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   insert
//  Desc:     Add the data element provided by the caller into the 
//            collection immediately preceding the current cursor 
//            position. 
//  Args:     Address of the new data element.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
long CCollection::insert(void* a_Arg){
return insert(new CItem(a_Arg));
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   next
//  Desc:     Logically reposition the cursor by resetting the cursor
//            to the next item of the collection. 
//  Args:     None.
//  Returns:  The next data element in the collection.
//---------------------------------------------------------------------+
void* CCollection::next(){
return mp_Cursor->next();
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   operator[]
//  Desc:     Select a specific item from the collection. 
//  Args:     unsigned int Index of the desired value.
//  Returns:  The specified data element from the collection.
//---------------------------------------------------------------------+
void* CCollection::operator[] (int a_Indx) const{
CCursor l_Cursor;

if (a_Indx >= ml_Count){
	return NULL;
}
else {
	l_Cursor.top(mp_First, ml_Count); 
	for (; a_Indx > 0; a_Indx--){
		l_Cursor.next();
	}
	return l_Cursor.getData();
}
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   popCursor
//  Desc:     Restore the previous collection processing position
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CCollection::popCursor(){
	*mp_Cursor = *mp_PrevCursor;
} /* End popCursor */





//---------------------------------------------------------------------+
//  Method:   prev
//  Desc:     Logically reposition the cursor by resetting the cursor
//            to the previous item of the collection. 
//  Args:     None.
//  Returns:  The previous data element in the collection.
//---------------------------------------------------------------------+
void* CCollection::prev(){
return mp_Cursor->prev();
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   pushCursor
//  Desc:     Store the current collection processing position 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CCollection::pushCursor(){
	*mp_PrevCursor = *mp_Cursor;
} /* End pushCursor */




//---------------------------------------------------------------------+
//  Method:   remove
//  Desc:     Delete the item indicated by the current cursor from the 
//            collection and reset the cursor. 
//  Args:     None.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//  History: 
//  02/09/2001 DRC Corrected logic by repositioning the Cursor BEFORE
//                 erasing the previous cursor's internal links.
//                 Changes flaged by Chng01
//---------------------------------------------------------------------+
#define _Chng01_
long CCollection::remove(){

long ll_RC;
CCursor lp_Cursor;

//
//  Validate the contents of the Cursor
//
if (isValidPointer(mp_Cursor->get())) {
#ifdef _Chng01_
	//
	//  Hold the address of the release candidate and 
	//  reposition the Collection's Cursor
	//
	lp_Cursor = *mp_Cursor;
	mp_Cursor->next();

	//
	//  Modify forward and backward links in the existing pointer chain
	//
	lp_Cursor.get()->removeLink();
	mp_isChanged = True;

#else
	//
	//  Modify forward and backward links in the existing pointer chain
	//
	mp_Cursor->get()->removeLink();
	mp_isChanged = True;

	//
	//  Hold the address of the release candidate and 
	//  reposition the Collection's Cursor
	//
	lp_Cursor = *mp_Cursor;
	mp_Cursor->next();
#endif
	
	//
	//  Release the storage for this linked item
	//
	delete lp_Cursor.get();
	ml_Count--;
	ll_RC = 0;

    #ifdef _temp_
	//
	//  When appropriate, set the endOfList indicator
	//
	if (emptyList()){
		ml_EndOfList = EndOfList;
        clearHeader();
	}
	else {
		ml_EndOfList = InList;
	}   /* End If  */
    #endif
}
else {
    #ifdef _temp_
	ml_EndOfList = EndOfList;
    #endif 
	clearHeader();
	ll_RC = -1;
}  /* End If */

return ll_RC;
#undef _Chng01_
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   remove
//  Desc:     Delete the item with an address matching the address 
//            provided by the caller from the collection. 
//  Args:     Address of the item to be removed.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//            04     = Item not found in collection
//---------------------------------------------------------------------+
long CCollection::remove(CItem* a_Item){

CCursor lp_Cursor;
long  ll_RC = 4;               //  Assume a NotFound error.

//
//  Temporarily reset the collection cursor
//  Browse the Collection for an Item that matches the address of a_Item
//
lp_Cursor = *mp_Cursor;
mp_Cursor->top(mp_First, ml_Count);
while (!mp_Cursor->endOfList()){
	if (mp_Cursor->get() == a_Item){
	//
	//  remove the located item and jump to endOfList
	//
		ll_RC = remove();
		mp_Cursor->bottom(mp_Last, ml_Count);
	}
	else {
		mp_Cursor->next();
	}  /* End If */

}   /* End While */

//
//  restore the original cursor setting
//
*mp_Cursor = lp_Cursor;
return ll_RC;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   resetHeader
//  Desc:     Reset the collection header information. 
//---------------------------------------------------------------------+
void CCollection::resetHeader(void){
mp_isChanged    = False;
if (isValidPointer(mp_Last) && isValidPointer(mp_Last->mp_Next))
	mp_First        = mp_Last->mp_Next;
else
	mp_First	    = NULL;
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   top
//  Desc:     Reset the cursor to the first item of the collection and
//            establish a forward processing direction. 
//  Args:     None.
//  Returns:  first data element in the collection.
//---------------------------------------------------------------------+
void* CCollection::top(){
if (mp_isChanged) resetHeader();
return mp_Cursor->top(mp_First, ml_Count);
}  /* End Method */
