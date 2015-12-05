//---------------------------------------------------------------------+
//  File:     CBuffer.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CBuffer
//            Class.
// 
//            The CBuffer class represents a Buffer of memory that
//            receives data from a message on a queue, or to
//            contains data to be put into a message on a queue.
// 
//  Dev Log:
//
//  01)  Fixed length blocking is working fine but there is a bug 
//       that is causing bytes to be dropped at the end of blocks 
//       in variable length blocking.          D. Creager 11/2000
// 
//  History: 
//  03/12/99 DMT Created original version.
//  01/13/00 DRC Added the assignment operator (operator=)
//  09/15/00 DRC Reduced this class to make it more general and 
//               therefore more reusable.  Eliminating any references 
//               the CLog class. 
//  09/15/00 DRC Added the Cursor and related methods:
//               clear, initSelf(long,bool,int), getCursor, getAvail,
//               isEndOfBuffer, isFilled, isBlocked, next, RFill, RTrim
//  11/10/00 DRC Added support for Variable Length Blocking. 
//
//---------------------------------------------------------------------+
#include "CBuffer.h"


//---------------------------------------------------------------------+
//  Method:   Allocate
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     This method will dynamically allocate a block of memory
//
//  Args:     pointer(the memory address on output)
//            long(length of the area)
//
//  Returns:  nothing.
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::Allocate(char** a_szBuffer, long a_lSize, BOOL a_bReset) {


if(isValidPointer(*a_szBuffer)) {
	try {
		delete [] *a_szBuffer;
	} /* End Try */

	catch (...) {
		*a_szBuffer = NULL;
	} /* End Catch */
} /* End If */

*a_szBuffer  = Allocate(a_lSize, a_bReset);
if (a_bReset){
	m_szCursor   = m_szBuffer;
	m_szLastRcrd = NULL;
} /* End If */
} /* End Allocate */






//---------------------------------------------------------------------+
//  Method:   Allocate
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will dynamically allocate a block of memory
//
//  Args:     long(length of the area)
//
//  Returns:  pointer to the allocated memory.
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CBuffer::Allocate(long a_lSize, BOOL a_bReset) {
//
//  Allocate and initialize a buffer area
//
char* l_szBuffer = NULL;
try {
	l_szBuffer = new char[a_lSize + 1];
	memset(l_szBuffer, '\0', a_lSize + 1);
	if (a_bReset) m_lSize    = a_lSize;
} /* End Try */

catch (...) {
	l_szBuffer = NULL;
	m_lSize    = 0;
} /* End Catch */

return l_szBuffer;
} /* End Allocate */







//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Removed all CLog references
//---------------------------------------------------------------------+
CBuffer::CBuffer() : CAny() {
m_lSize		   = 0;
m_lDataLen     = 0;
m_iIOBlkDir    = CBUF_IN_BOUND;
m_lRcrdLen     = 0;
m_bBlocked     = false;
m_bFixedLength = false;
m_iLastCC      = CBUF_NO_ERROR;
m_szBuffer	   = NULL;
m_szWrkBuf     = NULL;
m_szCursor	   = NULL;
m_szLastRcrd   = NULL;
} /* End CBuffer */




//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CBuffer::~CBuffer() {
termSelf();
} /* End ~CBuffer */





//---------------------------------------------------------------------+
//  Method:   expand
//  Author:   Daniel R. Creager
//  Written:  November 2000
//
//  Desc:     This method expands the size of the current buffer by the
//            amount specified by the caller.
//
//  Args:     Handle(address of the current buffer pointer)
//            long(Current size of the buffer)
//            long(Number of Bytes to add to the buffer)
//
//  Returns:  nothing
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//  11/10/00 DRC Added clearing m_lDataLen.
//---------------------------------------------------------------------+
void CBuffer::expand(char** a_hBuffer, long a_lSize, long a_lAmount) {
char* l_szBuffer = NULL;

if(isValidPointer(*a_hBuffer)) {
	//  Allocate a larger buffer
	l_szBuffer = Allocate(a_lSize + a_lAmount, false);

	//  Copy over the contents of the current buffer
	memcpy(l_szBuffer, *a_hBuffer, a_lSize);

	//  Release the old buffer
	delete [] *a_hBuffer;

	//  Reset the Buffer pointer
	*a_hBuffer = l_szBuffer;
} /* End If */
} /* End expand */






//---------------------------------------------------------------------+
//  Method:   clear
//  Author:   Daniel R. Creager
//  Written:  September 2000
//
//  Desc:     This method erases the current contents of this buffer,
//            resets the datalength, and resets the cursor.
//
//  Args:     none.
//
//  Returns:  nothing
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//  11/10/00 DRC Added clearing m_lDataLen.
//---------------------------------------------------------------------+
void CBuffer::clear(){
memset(m_szBuffer, '\0', m_lSize);
m_lDataLen   = 0;
m_szCursor   = m_szBuffer;
m_szLastRcrd = NULL;
}






//---------------------------------------------------------------------+
//  Method:   CnvrtDlmtrs
//  Author:   Daniel R. Creager
//  Written:  November 2000
//
//  Desc:     This method converts the record/buffer delimiters within 
//            the current buffer between internal and external 
//            representations based upon the direction specified by the 
//            caller.
//
//  Note:     The Unix style record/block delimiter (LF) is used
//            within the buffer's internal representation.  
//
//  Args:     None. 
//
//  m_iIOBlkDir
//            IN_BOUND  = from Host to Buffer representation.
//            OUT_BOUND = from Buffer to Host representation.
//
//  Returns:  nothing
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::CnvrtDlmtrs(){
	switch (m_iIOBlkDir){
	case CBUF_IN_BOUND:
		LfToSz(m_szBuffer, m_lDataLen);
        /*
		#ifdef _WIN32
		m_lDataLen =  CrlfToLf(&m_szBuffer, m_lSize, m_lDataLen);
		#endif
		*/
		break;

	case CBUF_OUT_BOUND:
		SzToLf(m_szBuffer, m_lDataLen);
		/*
        #ifdef _WIN32
		m_lDataLen   = LfToCrlf(&m_szBuffer, m_lSize, m_lDataLen);
		m_szCursor   = m_szBuffer;
		m_szLastRcrd = NULL;
		m_lSize      = a_lSize
		#endif
		*/
	} /* End Switch */
} /* End CnvrtDlmtrs */





//---------------------------------------------------------------------+
//  Method:   CrlfToLf
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will convert the Windows style record / block 
//            delimiters (CRLF) into Unix style delimiters (Lf).
//
//  Args:     pointer(address of the buffer to convert)
//            long(length of the buffer)
//            long(length of the data in the buffer)
//
//  Returns:  long(New length of the data in the buffer)
//  
//  Dev Log:  
//  01)  Still needs testing.  This is unproven code.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CBuffer::CrlfToLf(char** a_hBuffer, long a_lSize, long a_lDataLen) {
register char* l_Src = *a_hBuffer; 
register char* l_Dst = *a_hBuffer;


for (long l_lBufCount = a_lDataLen; l_lBufCount > 0; l_lBufCount--){
	if ((*l_Src+00 == CR) 
	&&  (*l_Src+01 == LF)){
		l_Src++;              // Skip past the Carriage Return
		a_lDataLen--;         // Reduce the size of the Data buffer
	} /* End If */
	*(l_Dst++) = *(l_Src++);  // Copy the character from src to dst
} /* End For */

return a_lDataLen;
} /* End CrlfToLf */






//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Removed all CLog references
//  11/10/00 DRC Changed reallocation comparison from LT to NE 
//---------------------------------------------------------------------+
long CBuffer::initSelf(long vl_size) {

	long ll_return = KOCC_GOOD;


	ll_return = CAny::initSelf();

	if (ll_return == KOCC_GOOD) {
		if ((!isValidPointer(m_szBuffer)) 
		||  (m_lSize != vl_size)) {
			Allocate(&m_szBuffer, vl_size, true);
		} /* End If */
		clear();

	} /* End If */

	if (m_szBuffer == NULL) {
		ll_return  = KOCC_FAIL;
		m_iLastCC  = CBUF_ERR_ALLOCATE_FAILED;
	} /* End If */

	//
	//  Initialize the buffer cursor
	//
	return ll_return;

}  /* end initSelf */



long CBuffer::initSelf(long vl_size, BOOL a_bBlocked, long a_lRcrdLen) {
long l_RC  = initSelf(vl_size);
m_lRcrdLen = a_lRcrdLen;
//
// Set Blocking and Record Format
//
setBlocked(a_bBlocked);
setFixedLen((a_lRcrdLen > 0));
return l_RC;
}






//---------------------------------------------------------------------+
//  Method:   LfToCrlf
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will convert Unix style record / block 
//            delimiters (Lf) into Windows style delimiters (CRLF).
//
//  Args:     handle(address of the buffer to convert)
//            long(length of the buffer)
//            long(length of the data in the buffer)
//
//  Returns:  long(New length of the data in the buffer)
//  
//  Dev Log:
//  01)  Still need to allow for the posibility that there is 
//       not enough room in the Buffer for the extra characters. 
//  02)  Still needs testing.  This is unproven code.
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CBuffer::LfToCrlf(char** a_hBuffer, long a_lSize, long a_lDataLen) {
register char* l_Src = *a_hBuffer; 
register char* l_Dst = *a_hBuffer;

for (long l_lBufCount = a_lDataLen; 
     ((l_lBufCount > 0) || (a_lDataLen >= a_lSize));
	 l_lBufCount--){

	if (*l_Src == LF){
		*(l_Dst++) = CR;      // Insert a Carriage Return
		l_Dst++;
		a_lDataLen++;         // Increase the size of the Data buffer
	} /* End If */
	*(l_Dst++) = *(l_Src++);  // Copy the character from src to dst
} /* End For */

//
// Enlarge the buffer by 1024 bytes and try again
//
if (a_lDataLen >= a_lSize) {
	expand(a_hBuffer, a_lSize, a_lSize + 1024);
	a_lDataLen = LfToCrlf(a_hBuffer, a_lSize + 1024, a_lDataLen);
} /* End If */

return a_lDataLen;
} /* End LfToCrlf */






//---------------------------------------------------------------------+
//  Method:   LfToSz
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will convert Line Feed delimited records and/or 
//            blocks into null terminated C strings.
//
//  Args:     pointer(address of the buffer to convert)
//            long(length of the data in the buffer)
//
//  Returns:  Nothing.
//  
//  Dev Log:
//  01)  Still need to allow for the posibility that there is 
//       not enough room in the Buffer for the extra characters. 
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::LfToSz(char* a_szBuffer, long a_lDataLen) {
for (; a_lDataLen > 0; a_lDataLen--){
	(*a_szBuffer == LF) ? *a_szBuffer = StrTrm : 0 ;
	a_szBuffer++;
} /* End For */
} /* End LfToSz */






//---------------------------------------------------------------------+
//  Method:   SzToLf
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will convert null terminated C strings into 
//            Line Feed delimited records and/or blocks .
//
//  Args:     pointer(address of the buffer to convert)
//            long(length of the data in the buffer)
//
//  Returns:  Nothing.
//  
//  Dev Log:
//  01)  Still need to allow for the posibility that there is 
//       not enough room in the Buffer for the extra characters. 
//  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::SzToLf(char* a_szBuffer, long a_lDataLen) {
for (; a_lDataLen > 0; a_lDataLen--){
	(*a_szBuffer == StrTrm) ? *a_szBuffer = LF : 0 ;
	a_szBuffer++;
} /* End For */
} /* End SzToLf */






//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CBuffer::termSelf() {

	long ll_return = KOCC_GOOD;

	m_lSize = 0;
	
	try {
		if(isValidPointer(m_szBuffer)) {
			delete [] m_szBuffer;
			m_szBuffer = NULL;
		}
	} /* end Try */

	catch (...){
		ll_return = KOCC_FAIL;
	} /* end catch */

	try {
		if(isValidPointer(m_szWrkBuf)) {
			delete [] m_szWrkBuf;
			m_szWrkBuf = NULL;
		}
	} /* End Try */

	catch (...){
		ll_return = KOCC_FAIL;
	} /* end catch */
	
	if (ll_return == KOCC_GOOD) ll_return = CAny::termSelf();
	return ll_return;
}














//---------------------------------------------------------------------+
//  Method:   getAvail
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     This method will dynamically calculate the amount of 
//            space that is still available for use within the buffer.
//
//  Args:     none
//
//  Returns:  long(Number of Available butes)
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
long CBuffer::getAvail() {
//return m_lSize - strlen(m_szBuffer);
return m_lSize - m_lDataLen;
}




//---------------------------------------------------------------------+
//  Method:   getDataLen
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will return the amount of space that has been 
//            used within the buffer.
//
//  Args:     none
//
//  Returns:  long(Number of Used butes)
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CBuffer::getDataLen() {
return 	m_lDataLen;
}




//---------------------------------------------------------------------+
//  Method:   getRcrdLen
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will return the logical record length for this
//            buffer.
//
//  Args:     none
//
//  Returns:  long(Logical Record Length)
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CBuffer::getRcrdLen() {
return 	m_lRcrdLen;
}




//---------------------------------------------------------------------+
//  Method:   getPointer
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
char* CBuffer::getPointer() {
return m_szBuffer;
}






//---------------------------------------------------------------------+
//  Method:   getCursor
//  Author:   Daniel R. Creager
//  Written:  September 2000
//
//  Desc:     This method returns the current cursor position within 
//            this i/o buffer.
//
//  Args:     none.
//
//  Returns:  char*(current cursor position)
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CBuffer::getCursor() {
return m_szCursor;
}







//---------------------------------------------------------------------+
//  Method:   getFirst
//  Author:   Daniel R. Creager
//  Written:  September 2000
//
//  Desc:     This method will re-initialize the buffer cursor, 
//            calculate the end_of_buffer address, and return the 
//            initial record.
//
//  Args:     none.
//
//  Returns:  char*(current cursor position)
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CBuffer::getFirst() {
m_szCursor = m_szBuffer;
setEndOfBuffer();
return getCursor();
}






//---------------------------------------------------------------------+
//  Method:   getSize
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CBuffer::getSize() {
	return m_lSize;
}








//---------------------------------------------------------------------+
//  Method:   isEndOfBuffer
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     tbd.
//
//  Args:     none
//
//  Returns:  long(Number of Available butes)
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL  CBuffer::isEndOfBuffer(){
return (m_szLastRcrd == m_szCursor);
} /* End isEndOfBuffer */





//---------------------------------------------------------------------+
//  Method:   isFilled
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     This method determines if the maximum number of fixed 
//            length records have been loaded into this buffer.
//
//  Args:     none
//
//  Returns:  BOOL(TRUE)  = Available space has been used
//            BOOL(FALSE) = Space is still available.
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CBuffer::isFilled() {
long l_Avail = getAvail();

if (l_Avail <= 0) 
   return true;
else
   return (l_Avail < m_lRcrdLen);
}






//---------------------------------------------------------------------+
//  Method:   isBlocked
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     This method determines if this is a buffer of blocked
//            fixed length records. 
//
//  Args:     none
//
//  Returns:  BOOL(TRUE)  = Contents are Blocked
//            BOOL(FALSE) = Contents are unblocked
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CBuffer::isBlocked() {
return (m_bBlocked);
}





//---------------------------------------------------------------------+
//  Method:   isFixed
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method determines if this is a buffer of fixed 
//            length records. 
//
//  Args:     none
//
//  Returns:  BOOL(TRUE)  = Contents are Fixed Length
//            BOOL(FALSE) = Contents are Variable Length
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CBuffer::isFixed() {
return (m_bFixedLength);
}





//---------------------------------------------------------------------+
//  Method:   next
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     Increment the cursor and return the next buffer pointer.
//
//  Args:     record length 
//            processing mode indicator
//
//  Returns:  pointer to the next buffer.
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CBuffer::next(long a_RcrdLen, BOOL a_InputMode){
if (a_InputMode){ 
	m_szLastRcrd = m_szCursor;
} /* End If */
m_szCursor	 = m_szCursor + a_RcrdLen;
return m_szCursor;
} /* End next */




char* CBuffer::next(){
return next(m_lRcrdLen, false);
} /* End next */




//---------------------------------------------------------------------+
//  Method:   operator=
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Assignment operator for the CBuffer class
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/13/00 DRC Created original version.
//  11/10/00 DRC Added setting of DataLen to copy operator
//---------------------------------------------------------------------+
void CBuffer::operator=  (CBuffer* a_Arg){
if (isValidPointer(a_Arg) && isValidPointer(m_szBuffer)){ 
	if (initSelf(a_Arg->getSize()) == KOCC_GOOD) {
		memcpy(m_szBuffer, a_Arg->getPointer(), a_Arg->getDataLen());
		m_lDataLen = a_Arg->m_lDataLen;
	}  /* End If */
}  /* End If */
}  /* End Method */








void CBuffer::operator=  (char* a_pArg){
if (initSelf(strlen(a_pArg) + 1) == KOCC_GOOD) {
    memcpy(m_szBuffer, a_pArg, strlen(a_pArg));
	m_lDataLen = strlen(a_pArg);
}  /* End If */
}  /* End Method */







//---------------------------------------------------------------------+
//  Method:   setBlocked
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     Set the value of this classes Blocked indicator.
//
//  Args:     Bool(New Value)  True  = blocked
//                             False = unblocked
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
void  CBuffer::setBlocked(BOOL a_bArg){
m_bBlocked     = a_bArg;
} /* End setBlocked */





//---------------------------------------------------------------------+
//  Method:   setDataLen
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will set the amount of space that has been 
//            used within the buffer.
//
//  Args:     long(Number of Used butes)
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::setDataLen(long a_lArg) {
m_lDataLen = a_lArg; 	
}




//---------------------------------------------------------------------+
//  Method:   setIOBlockingDir
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     This method will set the Direction of IO Blocking for 
//            the io functions.
//
//  Args:     int(CBUF_IN_BOUND or CBUF_OUT_BOUND)
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CBuffer::setIOBlockingDir(int a_iArg) {
m_iIOBlkDir = a_iArg; 	
}




//---------------------------------------------------------------------+
//  Method:   setFixedLen
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Set the value of this classes FixedLength indicator.
//
//  Args:     Bool(New Value)  True  = Fixed Length
//                             False = Variable Length
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void  CBuffer::setFixedLen(BOOL a_bArg){
m_bFixedLength = a_bArg;
} /* End setBlocked */






//---------------------------------------------------------------------+
//  Method:   setEndOfBuffer
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     Set the end of buffer address
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
void  CBuffer::setEndOfBuffer(){
char* l_Prev;

//  preserve the current cursor position
l_Prev = m_szCursor;

//  set the end_of_buffer address 
for (m_szCursor = m_szBuffer;
	*(next(m_lRcrdLen, true)) > '\0';){}
m_szLastRcrd = m_szCursor - m_lRcrdLen;

//  restore the cursor position
m_szCursor = l_Prev;
} /* End setEndOfBuffer */





//---------------------------------------------------------------------+
//  Method:   RFill
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     Fill the buffer on the right to force fixed length 
//            record alignment within the buffer and reset the 
//            internal cursor to the start of the next logical 
//            record within the buffer.
//
//  Args:     Fill character
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//  11/10/00 DRC Added incrementing m_lDataLen.
//---------------------------------------------------------------------+
void CBuffer::RFill(int a_FillChr){
long l_ValueLen;
long l_FillLen;

l_ValueLen = strlen(m_szCursor);
 
//
//  When the record is short, right fill with spaces
//
if (l_ValueLen < m_lRcrdLen){
	l_FillLen  = m_lRcrdLen - l_ValueLen;
	memset(m_szCursor+l_ValueLen, 32, l_FillLen);
    next(l_ValueLen + l_FillLen, true);
	m_lDataLen += l_FillLen;
}  /* End If */
//
//  When the record is long, truncate to the right
//
if (l_ValueLen > m_lRcrdLen){
	l_FillLen  = l_ValueLen - m_lRcrdLen; 
	memset(m_szCursor+m_lRcrdLen, '\0', l_FillLen);
	next(m_lRcrdLen, true);
}  /* End If */

if (l_ValueLen == m_lRcrdLen)
	next(m_lRcrdLen, true);

}  /* End RFill */





//---------------------------------------------------------------------+
//  Method:   RTrim
//  Author:   Daniel R Creager
//  Written:  September 2000
//
//  Desc:     Remove trailing spaces from a buffer and replace with 
//            a null terminator.
//
//  Note:     Because CBuffer always allocate one byte more that is 
//            requested and initializes its memory to nulls, a null
//            terminator byte should always be present.
//
//  Args:     Buffer pointer
//            Buffer length
//
//  Returns:  pointer to the modified buffer.
//  
//  Dev Log:  
//  History: 
//  09/15/00 DRC Created original version.
//---------------------------------------------------------------------+
char*  CBuffer::RTrim(char* a_pArg, long a_RcrdLen){

char* l_Cursor;
BOOL  l_bAllocated;

//
//  If there is no space to insert a NULL, 
//     allocate a new buffer area
//     and copy the data into the new buffer
//  else
//     point to the buffer provided
//
if (*(a_pArg + a_RcrdLen - 1) > ' ') {
	Allocate(&m_szWrkBuf, a_RcrdLen, false);
	memcpy(m_szWrkBuf,a_pArg,a_RcrdLen);
	l_bAllocated = true;
}
else {
	m_szWrkBuf = a_pArg;
	l_bAllocated = false;
} /* End If */

//
//  Point the cursor to the last byte of character data in this string
//
for(l_Cursor = (m_szWrkBuf + a_RcrdLen - 1);
    ((l_Cursor > m_szWrkBuf) && (*l_Cursor <= ' '));
	l_Cursor--){}

//
//  If location is in-the-buffer, 
//     then Insert a terminating null
//
if (l_Cursor == m_szWrkBuf){ 
   *(l_Cursor) = '\0';
}
else if ((l_Cursor > m_szWrkBuf) &&  (l_Cursor < (m_szWrkBuf + a_RcrdLen - 1))){
   *(l_Cursor + 1) = '\0';
} /* End If */


if (l_bAllocated){
	return m_szWrkBuf;
}
else {
	m_szWrkBuf = NULL;
	return a_pArg;
} /* End If    */
} /* End RTrim */



char*  CBuffer::RTrim(char* a_pArg){
return RTrim(a_pArg, m_lRcrdLen);
} /* End RTrim */

