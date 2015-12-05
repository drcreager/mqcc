/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CString.C 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CString.cpp
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Implementation of the CString class. 
//
//  History: 
//  03/28/96  DRC Created original code.
//
//  12/02/98  DMT Added additional methods and constructors to emulate 
//                some of the methods available in the new ANSI C++ 
//                standard "string" class.  These additions are indicated
//                via comments.
//                
//                Also, changed the >> operator to use "getline" instead
//                of "get".
//
//  12/04/98  DMT in "pos" function, changed < in for to <=, wasn't 
//                catching matching character if it was the last in
//                the string.
//
//  02/03/99  DMT operator=
//                changed "ll_strLen < strlen" to "<= strlen"
//
//                if a new value happened to be the same length as the
//                current buffer, the string wasn't resizing and 
//                wouldn't be null terminated after the assignment.
//
//  02/28/99  DMT Added operator << methods for concatenating CStrings
//                and longs onto CStrings.
//
//  03/03/99  DMT Constructor(char*) 
//                changed "ll_strLen < strlen" to "<= strlen"
//
//                if initial value happened to be the same length as the
//                default buffer size, the string wasn't resizing and 
//                wouldn't be null terminated after the assignment.
//
//  03/17/99  DMT Added check on operator = to make sure rs_Arg1 is 
//                not NULL, otherwise strlen(NULL) will blow up.
//
//  03/18/99  DMT If called for a NULL string, strlen blows up. Added
//                check on CString(char *)
//
//  09/21/99  DRC Added support for mix case comparison to pos
//
//  10/04/99  DRC Added support for operator[]
//
//  01/10/01  DRC Added the grep methods
//---------------------------------------------------------------------+
#include "CString.h"
#include <ctype.h>

//---------------------------------------------------------------------+
//  Method:   len
//  Desc:     Exposes the length attribute of the Cstring class. 
//---------------------------------------------------------------------+
ulong CString::len (void) {
return ll_strLen;
}

//---------------------------------------------------------------------+
//  Method:   vlen
//  Desc:     Exposes the value length attribute of the Cstring class. 
//---------------------------------------------------------------------+
ulong CString::vlen (void) {
return ll_vLen;
}



//---------------------------------------------------------------------+
//  Method:   length
//  Desc:     Exposes the value length attribute of the Cstring class. 
//
//  12/01/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
ulong CString::length (void) {
return ll_vLen;
}


//---------------------------------------------------------------------+
//  Method:   set_vlen
//  Desc:     Resets the value length attribute of the Cstring class. 
//---------------------------------------------------------------------+
ulong CString::set_vlen(void) {
ll_vLen = strlen(lp_string);
return ll_vLen;
}

//---------------------------------------------------------------------+
//  Method:   set_vlen
//  Desc:     Resets the value length attribute of the Cstring class. 
//---------------------------------------------------------------------+
ulong CString::set_vlen(ulong al_length) {
ll_vLen = al_length;
return ll_vLen;
}

//---------------------------------------------------------------------+
//  Method:   copy
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::copy (char* as_arg, char* rs_Arg1){
fill(as_arg,0);
if (ll_strLen < strlen(rs_Arg1)) {
	strncpy(as_arg,rs_Arg1,ll_strLen);
	ll_vLen = ll_strLen;
}
else {
	strncpy(as_arg,rs_Arg1,strlen(rs_Arg1));
	ll_vLen = strlen(rs_Arg1);
} 
return;
}

/*
//---------------------------------------------------------------------+
//  Method:   copy
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::copy (char* as_arg, CString* rs_Arg1){
fill();
if (ll_strLen < rs_Arg1->ll_strLen) 
{
	strncpy(as_arg,rs_Arg1->lp_string,ll_strLen);
	ll_vLen = ll_strLen;
}
else
{
	strncpy(as_arg,rs_Arg1->lp_string,rs_Arg1->ll_strLen);
	ll_vLen = rs_Arg1->ll_strLen;
}
endIf
return;
}*/

//---------------------------------------------------------------------+
//  Method:   copy
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::copy (char* as_arg, CString* rs_Arg1){
copy(as_arg,rs_Arg1->getPointer());
}



//---------------------------------------------------------------------+
//  Method:   grep
//  Desc:     This method performs pattern matching on strings.
//
//---------------------------------------------------------------------+
BOOL CString::grep(char* a_szSubject, 
				   char* a_szPattern, 
				   char* a_szDlmtrs){
//
//  declare local variables
//
BOOL    l_bResult = true;        // Assume the pattern will match
CString l_szPattern;             // local copy of Pattern
char*   l_pSCursor;              // Subject Cursor
char*   l_pPCursor;              // Pattern Cursor
BOOL    l_bWildcardPfx  = false; // Explicit Wildcard Prefix present     
BOOL    l_bWildcardSfx  = false; // Explicit Wildcard Suffix present


//
//  Validate the input values
//
if (a_szSubject == NULL){       //  No Subject string provided
	l_bResult = false;          //  Can't possibly match -> false
	goto exitPt;
} /* End If */

if (a_szPattern == NULL){       //  No Pattern string provided
	goto exitPt;                //  always matches -> true
} /* End If */

//
//  No Wildcard provided or in the Pattern
//     then the entire string must match
//
if ((a_szDlmtrs == NULL) 
||  (strstr(a_szPattern, a_szDlmtrs) == NULL)){
    l_bResult = (strnicmp(a_szSubject,a_szPattern,strlen(a_szSubject)) == 0);
	goto exitPt;
}  /* end If */


if ( (strlen(a_szPattern) == 1)       // Pattern is completely wildcarded
&&   (*a_szPattern == *a_szDlmtrs)) { // so everything matches -> true
	goto exitPt;
} /* End If */

//
//  Copy the pattern to prevent modification by strtok
//  and check for an explicit wildcard at the end of the pattern
//
l_szPattern = a_szPattern;
l_pPCursor  = l_szPattern.getPointer();
l_bWildcardPfx = (*l_pPCursor == *a_szDlmtrs);
l_pPCursor  = l_szPattern.getPointer() + l_szPattern.vlen() - 1;
l_bWildcardSfx = (*l_pPCursor == *a_szDlmtrs);

//
//  Initialize the two working cursors.
//
l_pSCursor  = a_szSubject;
l_pPCursor  = l_szPattern.getPointer();

//
//  Determine if all pattern segments match this string
//
l_pPCursor = strtok(l_pPCursor, a_szDlmtrs);
//
//  on no_explicit_wildcard prefix the  
//  the initial pattern segment must START the string.
//
if (!l_bWildcardPfx) {
	l_bResult = (strstr(l_pSCursor, l_pPCursor) == a_szSubject);
	if (!l_bResult) goto exitPt;
} /* end if */

while (l_pPCursor != NULL){
	l_pSCursor = strstr(l_pSCursor, l_pPCursor);
	//
	//  on NO_MATCH, indicate false and exit function
	//
	if (l_pSCursor == NULL) {
		l_bResult = false;
		goto exitPt;
	} /* end if */
	//
	//  skip past the string segment that was already matched
	//
	l_pSCursor += strlen(l_pPCursor);
	//
	//  isolate the next pattern segment to match
	//
	l_pPCursor = strtok(NULL, a_szDlmtrs);
} /* end while */

//
//  explicit wildcard suffix is necessary if 
//  the entire string has not been processed.
//
if (!l_bWildcardSfx) {
	l_bResult = (strlen(l_pSCursor) <= 0);
} /* end if */

exitPt:
	return l_bResult;
}


//---------------------------------------------------------------------+
//  Method:   grep
//  Desc:     This method performs pattern matching on strings.
//
//---------------------------------------------------------------------+
BOOL CString::grep(char* a_szPattern){
	return grep(m_Buf1, a_szPattern, "*");
} /* end grep */

//---------------------------------------------------------------------+
//  Method:   trim
//  Desc:     Eliminates leading and/or trailing spaces from the value 
//            of CString. 
//---------------------------------------------------------------------+
char* CString::trim (void){

int		li_pos = 1;
uint	li_len;
		
//
//  determine the position of the value within the string
//
while (*(lp_string+li_pos-1) <= ls_fillChar) li_pos++;

//
//  determine the length of the value within the string
//
li_len = ll_vLen;
while (*(lp_string+li_len-1) <= ls_fillChar) li_len--;
li_len = li_len - li_pos + 1;

return mid (li_pos, li_len);
}

//---------------------------------------------------------------------+
//  Method:   pos
//  Desc:     Determines the presence and position of a sub-string 
//            within this CString. 
//
//  12/04/98  DMT Changed < in for to <=.  Wasn't catching matching
//                character if it was the last in the string.
//
//  09/21/99  DRC Added an option to the parameter to support both 
//                Upper case and mixed case comparisons. 
//---------------------------------------------------------------------+
int CString::pos (int vi_pos, char* rs_Arg1, int vi_option){

uint li_indx;
int  li_len    = strlen(rs_Arg1);
int  li_result = 0;

//
//  Check for a comparison which is beyond the length of the string
//
if((uint) (vi_pos - 1 + li_len) > ll_vLen) goto NotFnd;

//
//  Scan the string for a substring without regard to case
//
// DMT 12/4/98 
//for (li_indx = vi_pos - 1; li_indx < (ll_vLen - li_len); li_indx++){
for (li_indx = vi_pos - 1; li_indx <= (ll_vLen - li_len); li_indx++){
	if (vi_option == 0){
		li_result = strnicmp((lp_string + li_indx),rs_Arg1,li_len);
	}
	else {
		li_result = strncmp((lp_string + li_indx),rs_Arg1,li_len);
	}
	if (li_result == 0) goto Matched;
}
NotFnd: 
	return 0;

Matched:
	return li_indx + 1;
}

//---------------------------------------------------------------------+
//  Method:   pos
//  Desc:     Determines the presence and position of a sub-string 
//            within this CString.
//---------------------------------------------------------------------+
int CString::pos (int vi_pos, char* rs_Arg1){

return this->pos(vi_pos,rs_Arg1, 0);
}
	
//---------------------------------------------------------------------+
//  Method:   pos
//  Desc:     Determines the presence and position of a CString within 
//            this CString. 
//---------------------------------------------------------------------+
int CString::pos (char* rs_Arg1){

return this->pos(1,rs_Arg1, 0);
}



//---------------------------------------------------------------------+
//  Method:   find
//  Desc:     Determines the presence and position of a character within 
//            this CString.
//
//  Returns:  -1 if not found, or index of position, where first 
//            character is at position 0.
// 
//  12/02/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//
//                Note that "string" indexes its characters starting 
//                at 0, CString starts at 1.  So, we subtract 1 from
//                whatever pos returns.
//---------------------------------------------------------------------+
int CString::find(char vs_char) {
	return this->pos(1, &vs_char, 0) - 1;
}



//---------------------------------------------------------------------+
//  Method:   pos
//  Desc:     Determines the presence and position of a CString within 
//            this CString. 
//---------------------------------------------------------------------+
int CString::pos (int vi_pos, CString* rs_Arg1){

return this->pos(vi_pos,rs_Arg1->getPointer(), 0);
}

//---------------------------------------------------------------------+
//  Method:   pos
//  Desc:     Determines the presence and position of a CString within 
//            this CString. 
//---------------------------------------------------------------------+
int CString::pos (CString* rs_Arg1){

return this->pos(1,rs_Arg1->getPointer(), 0);
}

//---------------------------------------------------------------------+
//  Method:   match
//  Desc:     Determines the presence and position of a character
//            of a sub-string within this CString. 
//---------------------------------------------------------------------+
int CString::match (char* rs_Arg1){

return strcspn(lp_string,rs_Arg1);
}

//---------------------------------------------------------------------+
//  Method:   match
//  Desc:     Determines the presence and position of a character
//            of a CString within this CString. 
//---------------------------------------------------------------------+
int CString::match (CString* rs_Arg1){

return this->pos(rs_Arg1->getPointer());
}

//---------------------------------------------------------------------+
//  Method:   mid
//  Desc:     Returns a substring based upon an index and a length.
//---------------------------------------------------------------------+
char* CString::mid (int vi_pos, uint vi_len){

if (vi_len > ll_strLen) vi_len = ll_strLen;
if (vi_pos == 0) vi_pos = 1;
 
fill(lp_temp,32);

strncpy(lp_temp,(lp_string + vi_pos - 1),vi_len);
if (vi_len < ll_strLen) *(lp_temp + vi_len) = NULL;
return lp_temp;
}




//---------------------------------------------------------------------+
//  Method:   substr
//  Desc:     Returns a substring based upon an index and a length.
//
//            NOTE:  the ANSI string class considers itself to be
//                   0 based, the first character is in position 0.
//                   the original methods in this class are 1 based.
//                   So, we need to add 1 to the position before
//                   calling the original "mid" function.
//
//  12/01/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
char* CString::substr(int vi_pos, uint vi_len){

	return this->mid(vi_pos + 1, vi_len);
}





//---------------------------------------------------------------------+
//  Method:   substr
//  Desc:     Returns a pointer to the first occurence of a substring
//            within this object. The search does not include 
//            terminating null characters.
//
//            NULL is returned if the search argument does not appear
//            in string. If the search argument points to a string of
//            zero length, the function returns string. 
//
//  11/10/00  DRC Added to wrapper the strstr function 
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
char* CString::StrStr(char* a_szSubj, char* a_szArg){
return strstr(a_szSubj, a_szArg);
}






//---------------------------------------------------------------------+
//  Method:   substr
//  Desc:     Returns a pointer to the first occurence of a substring
//            within this object. The search does not include 
//            terminating null characters.
//
//            NULL is returned if the search argument does not appear
//            in string. If the search argument points to a string of
//            zero length, the function returns string. 
//
//  11/10/00  DRC Added to wrapper the strstr function 
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
char* CString::StrStr(char* a_szArg){
return StrStr(m_Buf1, a_szArg);
}






//---------------------------------------------------------------------+
//  Method:   mid
//  Desc:     Returns a substring based upon an index.
//---------------------------------------------------------------------+
char* CString::mid (int vi_pos){

return this->mid(vi_pos, (int) ll_vLen);
}





//---------------------------------------------------------------------+
//  Method:   substr
//  Desc:     Returns a substring based upon an index.
//
//            NOTE:  the ANSI string class considers itself to be
//                   0 based, the first character is in position 0.
//                   the original methods in this class are 1 based.
//                   So, we need to add 1 to the position before
//                   calling the original "mid" function.
//
//  12/01/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
char* CString::substr(int vi_pos){

	return this->mid(vi_pos + 1, (int) ll_vLen);
}




//---------------------------------------------------------------------+
//  Method:   fill
//  Desc:     set the full length of the string to a caller specified 
//            value. 
//---------------------------------------------------------------------+
void CString::fill (char vc_FillChar){
ls_fillChar = vc_FillChar;
this->fill();
}

//---------------------------------------------------------------------+
//  Method:   fill
//  Desc:     set the full length of the string to the default value. 
//---------------------------------------------------------------------+
void CString::fill (void){
this->fill(lp_string,ls_fillChar);
this->fill(lp_temp,ls_fillChar);
ll_vLen = 0;
}

//---------------------------------------------------------------------+
//  Method:   fill
//  Desc:     set the full length of a buffer to the default value. 
//  History:
//  10/04/99 DRC Replaced pointer arithmetic with relative indexing
//               for AIX compatability.
//---------------------------------------------------------------------+
void CString::fill (char* vs_buffer, char vs_fillChar){
ulong ll_indx;

for (ll_indx = 0; ll_indx < ll_strLen; ll_indx++){
	vs_buffer[ll_indx] = vs_fillChar;
}
vs_buffer[ll_indx - 1] = NULL;
return;
}

//---------------------------------------------------------------------+
//  Method:   getBStr
//  Desc:     return a Basic string. 
//---------------------------------------------------------------------+
_bstr_t* CString::getBStr  (void){
return &ls_bstr;
}



//---------------------------------------------------------------------+
//  Method:   getPointer
//  Desc:     return a pointer the string value. 
//---------------------------------------------------------------------+
char* CString::getPointer (void){
return lp_string;
}
	



//---------------------------------------------------------------------+
//  Method:   c_str
//  Desc:     return a pointer the string value. 
//
//  12/01/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
char* CString::c_str (void){
return this->getPointer();
}



//---------------------------------------------------------------------+
//  Method:   replace
//  Desc:     find all occurances of a string and replace them. 
//---------------------------------------------------------------------+
int   CString::replace (char* rs_Search, char* rs_Replacement){

//
//  declare local variables
//
int			li_pos = 0;
int			li_count = 0;
int			li_srchLen;
int			li_replLen;
uint		li_newLen;
CString*	ls_prefixSect;
CString*	ls_suffixSect;

//
//  initialize for transaction processing
//
li_srchLen = strlen(rs_Search);
li_replLen = strlen(rs_Replacement);
ls_prefixSect = new CString(len());           // dynamically allocate work space
ls_suffixSect = new CString(len());
li_pos = pos(rs_Search);                      // locate the search string

//
//  process all appropriate changes
//
while (li_pos > 0){
	li_count++;                                // count the replacement
	*ls_prefixSect = mid(1,li_pos - 1);        // extract what comes before
	*ls_suffixSect = mid(li_pos + li_srchLen); // extract what comes after
	
	//
	//  check for replacement resulting in buffer overflow
	//
	li_newLen = ls_prefixSect->vlen() + li_replLen + ls_suffixSect->vlen() + 1;
	if ((this->len() < li_newLen)) this->resize(li_newLen);

	//
	//  replace the variable with the new value
	//
	operator= (ls_prefixSect);                // reload  what came  before
	operator+=(rs_Replacement);               // append  the replacement str
	operator+=(ls_suffixSect);                // append  what came after
	li_pos += li_replLen;                     // skip past the replacement value
	li_pos = pos(li_pos,rs_Search);
}

//
//  terminate transaction processing
//
delete ls_prefixSect, ls_suffixSect;          // clean up the work space
return li_count;
}


//---------------------------------------------------------------------+
//  Method:   resize
//  Desc:     Reset the string size to the amount specified
//
//  History: 
//  11/10/00 DRC Added resetting of m_Buf1 and m_Buf2.
//---------------------------------------------------------------------+
void  CString::resize (long al_newSize){

//
//  declare local variables with current pointer settings
// 
char*	lp_pString = lp_string;   // Pointer to the prev String Value
char*	lp_pTemp   = lp_temp;     // Pointer to the prev temp   Value

//
//  Reallocate the string
//  Reset the string pointer attributes
//
ll_strLen = al_newSize;
lp_string = new char [ll_strLen]; 
lp_temp   = new char [ll_strLen]; 
m_Buf1    = lp_string;                // DRC 11/10/00                   
m_Buf2    = lp_temp;                  // DRC 11/10/00 
fill(lp_string,ls_fillChar); 
fill(lp_temp,ls_fillChar); 

//
//  Copy the contents of the previous string into the new storage
//
copy(lp_string,lp_pString);         // reload the string value
copy(lp_temp,lp_pTemp);             // reload the temp value

//
//  Release space for the previous string
//
delete[] lp_pString;
delete[] lp_pTemp;
}

//---------------------------------------------------------------------+
//  Method:   >>  (Outbound Assignment)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::operator>> (CString* rs_Arg1){
rs_Arg1->fill();
if (ll_strLen < rs_Arg1->ll_strLen) 
{
	strncpy(rs_Arg1->lp_string,lp_string,ll_strLen);
	rs_Arg1->ll_vLen = ll_strLen;
}
else
{
	strncpy(rs_Arg1->lp_string,lp_string,rs_Arg1->ll_strLen);
	rs_Arg1->ll_vLen = rs_Arg1->ll_strLen;
}
}

//---------------------------------------------------------------------+
//  Method:   =  (Inbound Assignment)
//  Desc:     TBD. 
//
//  DMT 02/03/99 changed "ll_strLen < strlen" to "<= strlen"
//
//               if a new value happened to be the same length as the
//               current buffer, the string wasn't resizing and 
//               wouldn't be null terminated after the assignment.
//
//  DMT 03/17/99 Added check to make sure rs_Arg1 is not NULL.
//               strlen(NULL) will blow up.
//---------------------------------------------------------------------+
void CString::operator= (char* rs_Arg1){

if (rs_Arg1 != NULL) {
	if (ll_strLen <= strlen(rs_Arg1)) 
		resize(strlen(rs_Arg1)+1);

	copy(lp_string,rs_Arg1);
	ls_bstr = lp_string; 
}
else {
	fill();
}

return;
}

//---------------------------------------------------------------------+
//  Method:   =  (Inbound Assignment)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::operator= (CString* rs_Arg1){
operator= (rs_Arg1->getPointer());
}




//---------------------------------------------------------------------+
//  Method:   =  (Inbound Assignment)
//  Desc:     TBD. 
//
//  12/02/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
void CString::operator= (const CString& rs_Arg1){
operator= (rs_Arg1.lp_string);
}



//---------------------------------------------------------------------+
//  Method:   >>  (Outbound Assignment)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::operator>> (char* rs_Arg1){
if (ll_strLen < strlen(rs_Arg1)) 
	strncpy(rs_Arg1,lp_string,ll_vLen);

else
	strncpy(rs_Arg1,lp_string,strlen(rs_Arg1));

return;
}

//---------------------------------------------------------------------+
//  Method:   +=  (Concatenation)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::operator+= (char* rs_Arg1){

if (ll_strLen <= ll_vLen + strlen(rs_Arg1)) 
	resize(strlen(lp_string) + strlen(rs_Arg1) + 1);

strcat(lp_string,rs_Arg1);
set_vlen();
}

//---------------------------------------------------------------------+
//  Method:   +=  (Concatenation)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
void CString::operator+= (CString* rs_Arg1){
operator+=(rs_Arg1->getPointer());
}


/*
if (ll_strLen >= (strlen(lp_string) + strlen(rs_Arg1->lp_string))) 
{
	strcat(lp_string,rs_Arg1->lp_string);
	set_vlen();
}
return;
}
*/






//---------------------------------------------------------------------+
//  Method:   +=  (Concatenation)
//  Desc:     TBD.
//
//  12/02/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
void CString::operator+= (const CString& rs_Arg1){
operator+=(rs_Arg1.lp_string);
}






//---------------------------------------------------------------------+
//  Method:   +=  (Concatenation)
//  Desc:     TBD.
//
//  04/12/2002 DRC Added to emulate method of the "string" class
//                 available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
void CString::operator+= (long vl_Arg){
	char lpc_string[32];	// is big enough to hold a long 

	sprintf(lpc_string, "%d", vl_Arg);
	operator+=(lpc_string);
} // end operator+=



//---------------------------------------------------------------------+
//  Method:   +  (Concatenation)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
char* CString::operator+ (char* rs_Arg1){
static CString ls_temp(1024);

ls_temp = this;
return strcat(ls_temp.lp_string,rs_Arg1);
}



//---------------------------------------------------------------------+
//  Method:   +  (Concatenation)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CString* CString::operator+ (CString* rs_Arg1){
static CString ls_temp(1024);

ls_temp = this;
strcat(ls_temp.lp_string,rs_Arg1->lp_string);
return &ls_temp; 
}

//---------------------------------------------------------------------+
//  Method:   ==  (Comparison Operator)
//  Desc:     Perform a string comparison.  
//  Args:     CString*(Value to be compared against this string)
//---------------------------------------------------------------------+
BOOL CString::operator== (CString* rs_Arg1) {
return operator==(rs_Arg1->lp_string);
}

//---------------------------------------------------------------------+
//  Method:   ==  (Comparison Operator)
//  Desc:     Perform a string comparison.  
//  Args:     CString*(Value to be compared against this string)
//  Returns:  BOOL(Equality)    True  = CString is equal     to Argument 
//                              False = CString is not equal to Argument 
//
//  Notes:    The return values from strncmp are as follows: 
//  Int(Return Code)            < 0 = CString is less than Argument
//                              = 0 = CString is equal to  Argument
//                              > 0 = CString is greater than Argument
//---------------------------------------------------------------------+
BOOL CString::operator== (char* rs_Arg1) {
int li_result;

if (ll_vLen <= strlen(rs_Arg1)) 
	li_result = strncmp(lp_string,rs_Arg1,ll_vLen);
else
	li_result = strncmp(lp_string,rs_Arg1,strlen(rs_Arg1));

if (li_result == 0) 
	return True;
else
	return False;
}




//---------------------------------------------------------------------+
//  Method:   compare
//  Desc:     TBD.
//
//  12/02/98  DMT Added to emulate method of the "string" class
//                available in the New ANSI C++ libraries.
//---------------------------------------------------------------------+
int CString::compare(int vi_index, int vi_length, CString& rs_string) {

CString ls_this(100);
int		li_result = 0;

//ls_this = this->mid(vi_index); // index + 1?
ls_this = this->substr(vi_index); // index + 1?

if(ls_this.vlen() <= rs_string.vlen()) {
	li_result = strncmp(ls_this.getPointer(), rs_string.getPointer(), ls_this.vlen());
}
else {
	li_result = strncmp(ls_this.getPointer(), rs_string.getPointer(), rs_string.vlen());
}

return li_result;

}





//---------------------------------------------------------------------+
//  Method:   !=  (Comparison Operator)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
BOOL CString::operator!= (CString* rs_Arg1) {
return !operator==(rs_Arg1); 
}


//---------------------------------------------------------------------+
//  Method:   []  (Index Operator)
//  Desc:     return an indexed pointer the string value. 
//---------------------------------------------------------------------+
char& CString::operator[] (ulong vl_Indx){
if ((vl_Indx >= 0) && (vl_Indx < ll_strLen))
	return lp_string[vl_Indx];
else 
	return lp_string[ll_strLen - 1];
}


//---------------------------------------------------------------------+
//  Method:   !=  (Comparison Operator)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
BOOL CString::operator!= (char* rs_Arg1) {
return !operator==(rs_Arg1); 
}

//---------------------------------------------------------------------+
//  Method:   <<  (Insertion Operator)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
ostream&  operator<< ( ostream& au_stream, CString& au_arg1){
	au_stream << au_arg1.getPointer();
	return au_stream;
}




//---------------------------------------------------------------------+
//  Method:   >>  (Extraction Operator)
//  Desc:     TBD. 
//
//  12/02/98  DMT Changed Existing Version to call "getline" instead of
//                "get".  Note that the maximum line length that will
//                be extracted from the stream depends on the current
//                length of this string.
//---------------------------------------------------------------------+
istream&  operator>> ( istream& au_stream, CString& au_arg1){
	//au_stream.get(au_arg1.getPointer(),au_arg1.len());
	au_stream.getline(au_arg1.getPointer(),au_arg1.len()); //DMT 12/2/98
	au_arg1.set_vlen();
	return au_stream;
}




//---------------------------------------------------------------------+
//  Method:   Constructor - ulong parameter
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Allocate and initialize storage for string buffers. 
//
//  Args:     unsigned long = length of string to be allocated
//  Returns:  none
//  
//  Dev Log: 
//  History: 
//  03/01/96  DRC Created original version.
//  02/25/00  DRC Added alias pointers to support more generic use of 
//                the two core buffers without confusing mneumonics
//                within descendant code. 
//---------------------------------------------------------------------+
CString::CString(ulong vl_size){
//
//  Allocate space for the string
//  Set the string pointer attribute
//	Set the Alias pointers
//	Set the string length  attribute
//  Initialize the string to nulls
//
lp_string = new char [vl_size]; 
lp_temp   = new char [vl_size];
m_Buf1    = lp_string;                // DRC 2/25/00                   
m_Buf2    = lp_temp;                  // DRC 2/25/00 
ll_strLen = vl_size;
ll_vLen   = 0;
ls_fillChar = '\0';
fill(lp_string,ls_fillChar); 
fill(lp_temp,ls_fillChar); 
return;
}





//---------------------------------------------------------------------+
//  Method:   Constructor - No Parameters
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//
//  Args:     TBD
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  12/02/98  DMT Created original version.  Added to emulate method of
//                the "string" class available in the New ANSI C++ 
//                libraries.
//  02/25/00  DRC Added alias pointers to support more generic use of 
//                the two core buffers without confusing mneumonics
//                within descendant code. 
//---------------------------------------------------------------------+
CString::CString(){

ulong	vl_size = 32;

lp_string = new char [vl_size]; 
lp_temp   = new char [vl_size]; 
m_Buf1    = lp_string;                // DRC 2/25/00                   
m_Buf2    = lp_temp;                  // DRC 2/25/00 
ll_strLen = vl_size;
ll_vLen   = 0;
ls_fillChar = '\0';
fill(lp_string,ls_fillChar); 
fill(lp_temp,ls_fillChar); 
return;
}





//---------------------------------------------------------------------+
//  Method:   Constructor - char* parameter
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     Useful when passing a hardcoded "string" in a function
//            call that has a CString as a parameter.
//
//  Args:     char* pointer to characters containing initial value.
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  12/01/98  DMT Created original version.  Added to emulate method of
//                the "string" class available in the New ANSI C++ 
//                libraries.
//
//  03/03/99  DMT Found bug, if initializing string exactly 32 chars
//                (same size initial size specified by vl_size) there's
//                Terminating NULL.  Changed ll_strLen < strlen to 
//                <= strlen.
//
//  03/18/99  DMT If called for a NULL string, strlen blows up. Added
//                check.
//  02/25/00  DRC Added alias pointers to support more generic use of 
//                the two core buffers without confusing mneumonics
//                within descendant code. 
//---------------------------------------------------------------------+
CString::CString(char* rs_Arg1){

ulong	vl_size = 32;

lp_string = new char [vl_size]; 
lp_temp   = new char [vl_size]; 
m_Buf1    = lp_string;                // DRC 2/25/00                   
m_Buf2    = lp_temp;                  // DRC 2/25/00 
ll_strLen = vl_size;
ll_vLen   = 0;
ls_fillChar = '\0';
fill(lp_string,ls_fillChar); 
fill(lp_temp,ls_fillChar); 

//if(ll_strLen < strlen(rs_Arg1)) {
if(rs_Arg1 != NULL) {
	if(ll_strLen <= strlen(rs_Arg1)) {
		resize(strlen(rs_Arg1) + 1);
	}

	copy(lp_string, rs_Arg1);
}

ls_bstr = lp_string;
return;
}




//---------------------------------------------------------------------+
//  Method:   Copy Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     Useful when passing a CString parameter by value.
//            Ensures Compiler makes a safe "local" copy of the 
//            CString that will be destroyed when function returns.
//
//  Args:     const ref CString - CString this CString should be a
//                                copy of.
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  12/01/98  DMT Created original version.  Added to emulate method of
//                the "string" class available in the New ANSI C++ 
//                libraries.
//  02/25/00  DRC Added alias pointers to support more generic use of 
//                the two core buffers without confusing mneumonics
//                within descendant code. 
//---------------------------------------------------------------------+
CString::CString(const CString& rs_Arg1){

ll_strLen   = rs_Arg1.ll_strLen;
ll_vLen     = rs_Arg1.ll_vLen;
ls_fillChar = rs_Arg1.ls_fillChar;

lp_string = new char [ll_strLen]; 
lp_temp   = new char [ll_strLen]; 
m_Buf1    = lp_string;                // DRC 2/25/00                   
m_Buf2    = lp_temp;                  // DRC 2/25/00 

strncpy(lp_string, rs_Arg1.lp_string, ll_strLen);
strncpy(lp_temp, rs_Arg1.lp_temp, ll_strLen);


ls_bstr = lp_string;
return;
}




//---------------------------------------------------------------------+
//  Method:   Destructor - no parameters
//  Author:   Daniel R Creager
//  Written:  March 1996
//  Desc:     Deallocate storage for string buffers. 
//
//  Args:     none
//  Returns:  none
//  
//  Dev Log: 
//  History: 
//  03/01/96  DRC Created original version.
//  02/25/00  DRC Added alias pointers to support more generic use of 
//                the two core buffers without confusing mneumonics
//                within descendant code. 
//---------------------------------------------------------------------+
CString::~CString(void){
//
//  Release space for the string
//	Clear the string length  attribute
//  Clear the string pointer attribute
//
if (lp_string != (char*) NULL) delete[] lp_string;
if (lp_temp != (char*) NULL) delete[] lp_temp;
ll_strLen = 0; 
ll_vLen   = 0;
lp_string = (char*) NULL;
lp_temp   = (char*) NULL;
m_Buf1    = lp_string;                // DRC 2/25/00                   
m_Buf2    = lp_temp;                  // DRC 2/25/00 
return;
}




//---------------------------------------------------------------------+
//  Method:   operator<<
//  Author:   David M Tamkun
//  Written:  February 1999
//  Desc:     Concatenates a CString onto this CString
//
//  Args:     CString string value to concatenate onto this CString
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  02/02/99  DMT Created original version.
//---------------------------------------------------------------------+
CString&  CString::operator<< ( const CString& au_right){

    operator+=(au_right);
	return *this;
}








//---------------------------------------------------------------------+
//  Method:   operator<<
//  Author:   David M Tamkun
//  Written:  February 1999
//  Desc:     Concatenates a long onto this CString by converting it
//            to a string first.
//
//  Args:     long value to concatenate onto this CString
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  02/02/99  DMT Created original version.
//  04/12/02  DRC Modified to move conversion code into operator+= and 
//                simply invoke it from this method.
//---------------------------------------------------------------------+
CString& CString::operator<< ( const long vl_long) {

/*
	char lpc_string[32];	// is big enough to hold a long 

	sprintf(lpc_string, "%d", vl_long);

	operator+=(lpc_string);
*/
	operator+=((long) vl_long);
	return *this;
}


//---------------------------------------------------------------------+
//  Method:   toLower
//  Author:   Daniel R Creager
//  Written:  March 2001
//  Desc:     Converts the string characters to lower case
//
//  Args:     None
//  Returns:  char* address of the converted string value
//  
//  Dev Log: 
//  History: 
//  03/02/01  DRC Created original version.
//---------------------------------------------------------------------+
char* CString::toLower(){
ulong l_lCursor;

//
//  convert the entire value in this string
//
for (l_lCursor = 0; l_lCursor <=  ll_vLen; l_lCursor++)
	*(m_Buf1+l_lCursor) = toupper(*(m_Buf1+l_lCursor));
  
return m_Buf1;
} /* End toLower*/



//---------------------------------------------------------------------+
//  Method:   toUpper
//  Author:   Daniel R Creager
//  Written:  March 2001
//  Desc:     Converts the string characters to upper case
//
//  Args:     None
//  Returns:  char* address of the converted string value
//  
//  Dev Log: 
//  History: 
//  03/02/01  DRC Created original version.
//---------------------------------------------------------------------+
char* CString::toUpper(){
ulong l_lCursor;

//
//  convert the entire value in this string
//
for (l_lCursor = 0; l_lCursor <=  ll_vLen; l_lCursor++)
	*(m_Buf1+l_lCursor) = toupper(*(m_Buf1+l_lCursor));
  
return m_Buf1;
} /* End toUpper*/



#ifndef _WIN32

/*
//
//  Pre Version 2 code
//
extern "C" int strnicmp(const char* lpszStr1, const char* lpszStr2, size_t len)
{
	for (size_t l_len = 0; (char)'\0' != *lpszStr1 &&
						   (char)'\0' != *lpszStr2 &&
							l_len < len &&
							toupper(*lpszStr1) != toupper(*lpszStr2);
	     l_len++, lpszStr1++, lpszStr2++);
	
	return toupper(*lpszStr1) - toupper(*lpszStr2);
}
*/

extern "C" int strnicmp(const char* a_pszStr1, const char* a_pszStr2, size_t a_lLen){
	CString l_szStr1((char*) a_pszStr1);
	CString l_szStr2((char*) a_pszStr2);
	
	return strncmp(l_szStr1.toUpper(), l_szStr2.toUpper(), a_lLen);
} /* end strnicmp */
#endif