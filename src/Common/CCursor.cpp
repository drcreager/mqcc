/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/10/07	@(#) CCursor.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CCursor.cpp
//  Author:   Daniel R Creager
//  Written:  October 1999
//  Desc:     Implementation of the CCursor class. 
//
//  Dev Log: 
//       01)  The cursor recognizes EndOfList based upon a count that
//            is established when the TOP or BOTTOM methods are invoked.  
//            If items are added or deleted after TOP | BOTTOM then the 
//            determination of EndOfList may be inaccurate based upon 
//            the current positioning of the CURSOR when the insertion
//            or deletion occured.  This needs to be enhanced at some 
//            time in the future.      D. Creager     
//
//  History: 
//  10/05/99  DRC Created original code.
//---------------------------------------------------------------------+
#include "CCursor.h"



//---------------------------------------------------------------------+
//  Method:   bottom
//  Desc:     Reset the cursor to the last item of the collection and
//            establish a backward processing direction. 
//  Args:     None.
//  Returns:  The last data element in the collection.
//---------------------------------------------------------------------+
void* CCursor::bottom(CItem* a_InitPos, long a_Count){
mp_isForward      = False;
return resetCursor(a_InitPos, a_Count);
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   CCursor
//  Desc:     Default Constructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CCursor::CCursor(){
mp_Current      = NULL;
ml_Count       = 0;
mp_isForward   = True;
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   ~CCursor
//  Desc:     Destructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CCursor::~CCursor(){
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   endOfList
//  Desc:     Indicates the end of the list has been hit. 
//  Args:     None.
//  Returns:  Boolean indicator.  True = Beyond Last Item in the list.
//---------------------------------------------------------------------+
BOOL CCursor::endOfList(){
return (ml_Count == 0);
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   get
//  Desc:     Retrieve the item pointed to currently. 
//  Args:     None.
//  Returns:  current data element in the collection.
//---------------------------------------------------------------------+
CItem* CCursor::get(){
return mp_Current;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   getData
//  Desc:     Retrieve the data item pointed to currently. 
//  Args:     None.
//  Returns:  current data element in the collection.
//---------------------------------------------------------------------+
void* CCursor::getData(){
return mp_Current->mp_Item;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   next
//  Desc:     Logically reposition the cursor by resetting the cursor
//            to the next item of the collection. 
//  Args:     None.
//  Returns:  The next data element in the collection.
//---------------------------------------------------------------------+
void* CCursor::next(){
operator++();
return getData();
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   operator++
//  Desc:     Physically increment the cursor in the current 
//            navigational direction.
//  Args:     None
//  Returns:  Reset Cursor
//---------------------------------------------------------------------+
void CCursor::operator++ (void){
ml_Count--;
if (!endOfList()) {
   if (mp_isForward) mp_Current = mp_Current->mp_Next;
   else              mp_Current = mp_Current->mp_Prev;
}  /* End If */
}  /* End Method */


//---------------------------------------------------------------------+
//  Method:   operator--
//  Desc:     Physically decrement the cursor in the current 
//            navigational direction.
//  Args:     None
//  Returns:  Reset Cursor
//---------------------------------------------------------------------+
void CCursor::operator-- (void){
ml_Count++;
if (!endOfList()) {
   if (mp_isForward) mp_Current = mp_Current->mp_Prev;
   else              mp_Current = mp_Current->mp_Next;
}  /* End If */
}  /* End Method */


//---------------------------------------------------------------------+
//  Method:   prev
//  Desc:     Logically reposition the cursor by resetting the cursor
//            to the previous item of the collection. 
//  Args:     None.
//  Returns:  The previous data element in the collection.
//---------------------------------------------------------------------+
void* CCursor::prev(){
operator--();
return getData();
}  /* End Method */



//---------------------------------------------------------------------+
//  Method:   resetCursor
//  Desc:     Reset the collection header information. 
//---------------------------------------------------------------------+
void* CCursor::resetCursor(CItem* a_InitPos, long a_Count){
if (a_Count == 0){
   mp_Current   = NULL;
   return NULL;
}
else {
   ml_Count     = a_Count;
   mp_Current   = a_InitPos;
   return mp_Current->mp_Item;
}  /* End If     */
}  /* End Method */




//---------------------------------------------------------------------+
//  Method:   top
//  Desc:     Reset the cursor to the first item of the collection and
//            establish a forward processing direction. 
//  Args:     None.
//  Returns:  first data element in the collection.
//---------------------------------------------------------------------+
void* CCursor::top(CItem* a_InitPos, long a_Count){
mp_isForward     = True;
return resetCursor(a_InitPos, a_Count);
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   set
//  Desc:     Set the item pointed to by the current position. 
//  Args:     CItem* the item to set in the cursor
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CCursor::set(CItem* a_Item){
mp_Current = a_Item;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   setData
//  Desc:     Set the data item pointed to by the current position. 
//  Args:     Void* the data element to set in the cursor's item
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CCursor::setData(void* a_Item){
mp_Current->mp_Item = a_Item;
}  /* End Method */