/*
 *  Copyright 2000, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 00/01/10	@(#) CBufferList.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CBufferList.cpp
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Implementation of the CBufferList class. 
//
//  History: 
//  01/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "BufferList.h"


//---------------------------------------------------------------------+
//  Method:   CBufferList
//  Desc:     Default Constructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CBufferList::CBufferList(CLog* a_pcLog){
m_pElement = NULL;
m_pcLog    = a_pcLog;
}  /* End Method */





//---------------------------------------------------------------------+
//  Method:   ~CBufferList
//  Desc:     Destructor Method
//  Args:     None.
//---------------------------------------------------------------------+
CBufferList::~CBufferList(){
m_pElement = NULL;
m_pcLog    = NULL;
}  /* End Method */	






//---------------------------------------------------------------------+
//  Method:   clear
//  Desc:     Extend the ancestral clear method by releasing the 
//            buffer holding the data as well as the CItem that 
//            references it. 
//
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//  History: 
//  09/20/00  DRC Removed references to the log object.
//---------------------------------------------------------------------+
long CBufferList::clear(){
//
//  declare local variables
//
CBuffer*	l_pElement;	
	
//
//  allocate a work buffer
//
l_pElement  = new CBuffer();
//l_pElement->initSelf(KO_RR_BUFFER_SIZE, m_pcLog);
l_pElement->initSelf(KO_RR_BUFFER_SIZE);

//
//  Empty the list 
//
top(l_pElement);
while (!endOfList() && !emptyList()){
   pop(l_pElement);
}  /* End While  */
clearHeader();

//
//  Release the work buffer and return to caller
//
l_pElement->termSelf();
delete l_pElement;
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
long CBufferList::pop(CBuffer* a_Item){
//
//  declare local variables
//
long ll_RC = -1;

//
//  on_successful, 
//  - copy the string into the caller's buffer
//  - release the string buffer from the item
//  - remove the item entry from the list
//    (Note:  Remove increments the cursor)
//
if (isValidPointer(m_pElement)) {
	*a_Item = (CBuffer*) m_pElement;
#ifdef _Temp01_
	delete    m_pElement;
	ll_RC = CCollection::remove();
#else
	ll_RC = CCollection::remove();
	delete    m_pElement;
#endif
}  /* End If */

//
//  invoke the ancestral get method to retrieve the current string
//
if (endOfList() || emptyList()){
	m_pElement = NULL;
}
else {
	m_pElement = (CBuffer*) CCollection::get();
}
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
long CBufferList::push(CBuffer* a_Item){
//
//  allocate a copy of the passed buffer 
//
m_pElement = new CBuffer();
m_pElement->operator=(a_Item);



//
//  invoke the ancestral add method to copy the string into the list
//
return CCollection::add(m_pElement);
}  /* End Method */








//---------------------------------------------------------------------+
//  Method:   top
//  Desc:     Repostition the collection to the first item.  
//  Args:     Address of the new item.
//  Returns:  Return Code 
//            00     = Successful function
//            Not 00 = Unsuccessful function
//---------------------------------------------------------------------+
void CBufferList::top(CBuffer*	a_Arg){

//
//  invoke the ancestral top method to retrieve the first element
//
m_pElement = (CBuffer*) CCollection::top();
*a_Arg     = m_pElement;
}  /* End Method */
