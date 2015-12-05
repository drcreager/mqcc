/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21	@(#) CKeyField.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CKeyField.cpp
//  Author:   Dale Thompson
//  Written:  September 1999
//  Desc:     Implementation of the CKeyField class.  This class 
//            encapsulates the knowledge of the MQ Series Key Fields
//            (e.g. MessageId and CorrelationId). 
//
//            This class maintains a copy of the value provided by the 
//            caller to ensure its ability to cache translations.  It 
//            maintains a before translation image of the key value in 
//            CString::m_Buf2 and an after translation image in 
//            CString::m_Buf1.
//
//  Dev Log: 
//  01)  Add Copy Constructor
//  02)  Add Assignment Operator
//
//  History: 
//  09/21/99  Created original code.
//  10/04/99  DRC Modified isChanged() to include length as a parm.
//---------------------------------------------------------------------+
#ifndef _CKeyField
#define _CKeyField
#include "CKeyField.h"
#endif

//---------------------------------------------------------------------+
//  Method:   isDataRcpt
//  Desc:     Determines if a Data format receipt is desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 .1.. ....
//                   +--------->  Data Receipt Indicator 
//                                1 = Data Receipt Desired
//---------------------------------------------------------------------+
int CKeyField::isDataRcpt(MQBYTE24* rs_Arg1){
return ((*(rs_Arg1[0]) & DataRcpt) == DataRcpt);
}

int CKeyField::isDataRcpt(){
return ((m_BitFields->getPointer()[0] & DataRcpt) == DataRcpt);
}

//---------------------------------------------------------------------+
//  Method:   setDataRcpt
//  Desc:     Set Data format receipt as desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 .1.. ....
//                   +--------->  Data Receipt Indicator 
//                                1 = Data Receipt Desired
//---------------------------------------------------------------------+
void CKeyField::setDataRcpt(){
m_BitFields->getPointer()[0] |= DataRcpt;
}

//---------------------------------------------------------------------+
//  Method:   clrDataRcpt
//  Desc:     Clear Data format receipt as desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 .1.. ....
//                   +--------->  Data Receipt Indicator 
//                                1 = Data Receipt Desired
//---------------------------------------------------------------------+
void CKeyField::clrDataRcpt(){
m_BitFields->getPointer()[0] &= ~DataRcpt;      // Data Receipt = Off
}
//---------------------------------------------------------------------+
//  Method:   isLogRcpt
//  Desc:     Determines if a Log format receipt is desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 1... ....
//                  +---------->  Log Receipt Indicator 
//                                1 = Log Receipt Format Desired
//---------------------------------------------------------------------+
int CKeyField::isLogRcpt(MQBYTE24* rs_Arg1){
return ((*(rs_Arg1[0]) & LogRcpt) == LogRcpt);
}

int CKeyField::isLogRcpt(){
return ((m_BitFields->getPointer()[0] & LogRcpt) == LogRcpt);
}


//---------------------------------------------------------------------+
//  Method:   setLogRcpt
//  Desc:     Set Log format receipt as desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 1... ....
//                  +---------->  Log Receipt Indicator 
//                                1 = Log Receipt Desired
//---------------------------------------------------------------------+
void CKeyField::setLogRcpt(){
m_BitFields->getPointer()[0] |= LogRcpt;
}

//---------------------------------------------------------------------+
//  Method:   clrLogRcpt
//  Desc:     Clear Log format receipt as desired.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 1... ....
//                  +---------->  Log Receipt Indicator 
//                                1 = Log Receipt Desired
//---------------------------------------------------------------------+
void CKeyField::clrLogRcpt(){
m_BitFields->getPointer()[0] &= ~LogRcpt;      // Log Receipt = Off
}


//---------------------------------------------------------------------+
//  Method:   isNLSKey
//  Desc:     Determines if the current key value is in NLS format.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 ...1 ....
//                     +------->  Key Format Indicator 
//                                0 = NLS format
//                                1 = Pre-NLS format
//---------------------------------------------------------------------+
int CKeyField::isNLSKey(char rs_Arg1){
//
// evaluate the assigned bit field and return the inverse of its setting 
//
return (!(rs_Arg1 & PreNLSFmt));
}

int CKeyField::isNLSKey(MQBYTE24* rs_Arg1){
return isNLSKey(*(rs_Arg1[0]));
}

int CKeyField::isNLSKey(){
return isNLSKey(m_BitFields->getPointer()[0]);
}


//---------------------------------------------------------------------+
//  Method:   setNLSKey
//  Desc:     Set NLSKey format message.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 ...1 ....
//                     +------->  Key Format Indicator 
//                                0 = NLS Key Format
//                                1 = Pre-NLS Key Format
//---------------------------------------------------------------------+
void CKeyField::setNLSKey(){
m_BitFields->getPointer()[0] &= ~PreNLSFmt;      // set NLSKey = On
}

//---------------------------------------------------------------------+
//  Method:   setOldKey
//  Desc:     Set Pre-NLS key format message.
// 
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 ...1 ....
//                     +------->  Key Format Indicator 
//                                0 = NLS Key Format
//                                1 = Pre-NLS Key Format
//---------------------------------------------------------------------+
void CKeyField::setOldKey(){
m_BitFields->getPointer()[0] |= PreNLSFmt;      // set NLSKey = Off
}


//---------------------------------------------------------------------+
//  Method:   encode
//  Desc:     Translate the buffer contents from the Host character set
//            into a neutral representation called the NLS character set
//
//            For performance purposes, when the parameter value 
//            matches the previous translation, the previous result 
//            is returned without re-executing then HostToNLS method. 
//
//            Since NULL KeyFields indicate any value is valid it is 
//            erroneous to translate a KeyField that begins with NULL. 
//---------------------------------------------------------------------+
void CKeyField::encode(MQBYTE24* rs_Arg1){
if (isChanged(m_Buf2, rs_Arg1)) {
	operator=(rs_Arg1);
	if ( *(rs_Arg1)[0] > '\0'){
		HostToNLS();
	}
	setNLSKey();
}
getBuf(rs_Arg1, m_Buf1);

}


//---------------------------------------------------------------------+
//  Method:   decode
//  Desc:     Translate the buffer contents from a neutral 
//            representation called the NLS character set into the Host 
//            character set 
//
//            For performance purposes, when the parameter value 
//            matches the previous translation, the previous result 
//            is returned without re-executing then NLSToHost method. 
//---------------------------------------------------------------------+
void CKeyField::decode  (MQBYTE24* rs_Arg1){

/*
if (isNLSKey()) {
	if (isChanged(m_Buf1, rs_Arg1)){ 
		operator=(rs_Arg1);
		NLSToHost();
	}
	getBuf(rs_Arg1, m_Buf2);
}
*/

if (isChanged(m_Buf1, rs_Arg1)){ 
	operator=(rs_Arg1);
	NLSToHost();
}
getBuf(rs_Arg1, m_Buf2);


}

//---------------------------------------------------------------------+
//  Method:   cancel 
//  Desc:     cancel the previous encoding and future translations by 
//            copying the pre-translated value over the translation and
//            reseting the object's key format indicator.  This way when
//            subsequent encodings are requested, they are filled from 
//            the cache and a single call to MQGet will find the value.  
//---------------------------------------------------------------------+
void CKeyField::cancel  (MQBYTE24* rs_Arg1){

memcpy(m_Buf1, m_Buf2, vlen());
getBuf(rs_Arg1, m_Buf1);
setOldKey();                       // set NLSKey = Off
}

//---------------------------------------------------------------------+
//  Method:   isChanged
//  Desc:     Determines if the passed parameter value has changed 
//            from the current contents of this object.
//
//            This method compares the before translation image that is 
//            maintained in CString::m_Buf2 with the key value provided
//            by the caller, to determine if the key value has changed 
//            and should therefore be re-translated. 
// 
//  Returns:  0      = False = No change has occured
//            Not(0) = True  = A change has occured            
//---------------------------------------------------------------------+
int CKeyField::isChanged(char*		rs_Arg1, 
						 MQBYTE24*	rs_Arg2){

return memcmp(rs_Arg1, (char*) rs_Arg2, ll_vLen);
}


//---------------------------------------------------------------------+
// NLS Character Set
//
//            A character set neutral encoding that uses the relative 
//            position of a character within the English language 
//            alphabet to represent the character.  Similar to the way 
//            your childhood secret code ring worked.  The letter A is 
//            replaced by a binary value of 1 because it is in the first
//            position of English language alphabet.
//
//            The full NLS Character Set is 64 characters consists of:
//            1)  Upper Case English language alphabet
//            2)  Lower Case English language alphabet
//            3)  Roman numeric digits 0 - 9
//            4)  Hyphen "-"
//            5)  Underscore "_" 
//
//            Note: Position is used rather than offset to avoid the 
//                  occurance of the value x'00' which has special 
//                  meaning in C language programming as a string 
//                  terminator. 
//---------------------------------------------------------------------+


//---------------------------------------------------------------------+
//  Method:   HostToNLS
//  Desc:     Translate the buffer contents from the Host character set
//            into the NLS character set described above.
//
//            This method has been optimized to enhance its performance
//            as follows: 
//            1)  Repetitive characters are translated from a cache
//            2)  Search ranges are limited to a max of 13 characters
//                which is 1/5th of the full character set.
//---------------------------------------------------------------------+
void CKeyField::HostToNLS  (int vi_Initial = 0){
int  l_Result;
char l_Buf[2] = "";
char l_Prev[2] = "";
int  l_Pos = 1;

//
//  Range check the parameter
//
if (vi_Initial < 0)  vi_Initial = 0;
if (vi_Initial > 23) vi_Initial = 23;

//
//  Browse the field translating each character from ASCII into the 
//  NLS character set.
//
m_CurrTBuf = m_Buf1;
for (int i = vi_Initial; i <= 23; i++){
	l_Buf[0] = *(m_Buf1+i);          //  Isolate a single character

	//
	//  On Repetitive_Characters, skip the search
	//
	if (l_Buf[0] == l_Prev[0]){
		l_Result = l_Prev[1];
	}
	else {

		//
		//  Set the initial search position for this character in order 
		//  to limit the search length to a maximum of 13 characters. 
		//
		if     ((l_Buf[0] <  'A') || (l_Buf[0] == '_')) l_Pos = 53; 
		else if (l_Buf[0] >= 'm') l_Pos = 39; 
		else if (l_Buf[0] >= 'a') l_Pos = 27; 
		else if (l_Buf[0] >= 'M') l_Pos = 13;
		else l_Pos = 1;

		//
		//  Determine the relative Position of this character
		//  and cache the result to save repetitive searches.
		//
		l_Result = m_NLSChrSet->pos(l_Pos, l_Buf, 1);
		l_Prev[0] = l_Buf[0];
		l_Prev[1] = l_Result;
	}

	//
	// On Invalid_Character use "_" as a default translation
	//
	if ((l_Result <= 0) || (l_Buf[0] == '\0')){
		*(m_Buf1+i) = (char) 63;    
	}
	else {
		*(m_Buf1+i) = (char) l_Result;
	}
}
setNLSKey();                       // set NLSKey = On
} /* End of HostToNLS Method */

//---------------------------------------------------------------------+
//  Method:   HostToNLS
//  Desc:     Translate the buffer contents from the Host character set
//            into the NLS character set described above.
//---------------------------------------------------------------------+
void CKeyField::HostToNLS  (void){
HostToNLS(0);
}

//---------------------------------------------------------------------+
//  Method:   NLSToHost
//  Desc:     Translate the buffer contents from the NLS character set,
//            described above, into ASCII.
//
//            This method uses a method of indirect character addressing 
//            to pick individual characters out of the reference string.
//
//            Because the values it receives from the caller represents 
//            positions in the NLS Characterset, they can easily be 
//            converted into offsets.  The offset is then used to 
//            indirectly reference the specific ASCII character from 
//            within the NLSChrSet string. 
//
//            Indirect character addressing is so effiecient that 
//            this method does not utilize the cache techniques in HostToNLS.
//            The cache mgt instructions produce more overhead than 
//            re-translating each character using this method. 
//---------------------------------------------------------------------+
void CKeyField::NLSToHost  (int vi_Initial = 0){
char* l_NLSChrSet;

//
//  Range check the parameter
//
if (vi_Initial < 0)  vi_Initial = 0;
if (vi_Initial > 23) vi_Initial = 23;

//
//  Browse the field translating each character from the 
//  NLS character set into ASCII.
//
l_NLSChrSet = m_NLSChrSet->getPointer();
m_CurrTBuf  = m_Buf2;                 
for (int i = vi_Initial; i <= 23; i++){
	//
	//   The following statements are functionally equivilent to the single,
	//   albeit complex statement, which generates 47.37% (10/19-1) fewer 
	//   machine language instructions.
	//
	//   char* BufAddr = m_Buf2 + i;
	//   int   CharPos = *(BufAddr);
	//   int   offset  = CharPos - 1;
	//   char* NewChrPos = l_NLSChrSet+offset;
	//   *(m_Buf2+i) = *NewChrPos;
	//
	*(m_Buf2+i) = *(l_NLSChrSet+*(m_Buf2 + i) - 1);
}
setOldKey();                       // set NLSKey = Off
} /* End of NLSToHost Method */

//---------------------------------------------------------------------+
//  Method:   NLSToHost
//  Desc:     Translate the buffer contents from the NLS character set,
//            described above, into ASCII.
//---------------------------------------------------------------------+
void CKeyField::NLSToHost  (void){
NLSToHost(0);
}

//---------------------------------------------------------------------+
//  Method:   =  (Inbound Assignment)
//  Desc:     Copy a MQBYTE24 field into this object.  
//
//  09/22/99  DRC Added to facilitate handling MQBYTE24 datatype.
//---------------------------------------------------------------------+
void CKeyField::operator=  (MQBYTE24* rs_Arg1){
if (ll_strLen != 25) resize(25);

set_vlen(24);
memcpy(m_Buf1, rs_Arg1, vlen());
memcpy(m_Buf2, rs_Arg1, vlen());
}

//---------------------------------------------------------------------+
//  Method:   getBuf
//  Desc:     Copy this object into a MQBYTE24 field.  
//
//  09/22/99  DRC Added to facilitate handling MQBYTE24 datatype.
//---------------------------------------------------------------------+
void CKeyField::getBuf (MQBYTE24* rs_Arg1, char* rs_Arg2){
memcpy(rs_Arg1, rs_Arg2, vlen());
}


//---------------------------------------------------------------------+
//  Method:   getBitFlds
//  Desc:     Provide a pointer to the BitField string.  
//
//  10/18/99  DRC Created original method.
//---------------------------------------------------------------------+
CString* CKeyField::getBitFlds (void){
return m_BitFields;
}



//---------------------------------------------------------------------+
//  Method:   setBitFlds
//  Desc:     Set the BitField string from a caller provided source.  
//
//  10/18/99  DRC Created original method.
//---------------------------------------------------------------------+
void CKeyField::setBitFlds (CString* as_Arg){
*m_BitFields = as_Arg;
}



//---------------------------------------------------------------------+
//  Method:   <<  (Insertion Operator)
//  Desc:     TBD. 
//  1)  copy au_arg1->m_Buf1 to m_Buf2 (ref: CString::resize)
//  2)  Translate the m_Buf2  copy of the string using NLSToHost()
//  3)  Pass the pointer of m_Buf2  to the ostream operator
//---------------------------------------------------------------------+
ostream&  operator<< ( ostream& au_stream, CKeyField& au_arg1){
	au_stream << au_arg1.getPointer();
	return au_stream;
}

//---------------------------------------------------------------------+
//  Method:   >>  (Extraction Operator)
//  Desc:     TBD. 
//  1)  Invoke CString::operator>>
//  2)  Invoke HostToNLS(as_arg->m_Buf1, as_arg->ll_vLen)
//---------------------------------------------------------------------+
istream&  operator>> ( istream& au_stream, CKeyField& au_arg1){
	au_stream.get(au_arg1.getPointer(),au_arg1.len());
	au_arg1.set_vlen();
	return au_stream;
}


//---------------------------------------------------------------------+
//  Method:   Constuctor
//  Desc:     Build a specialized String that holds a 24 byte byte 
//            array and has an extra character for a null terminator.  
//---------------------------------------------------------------------+
CKeyField::CKeyField() : CString(25) {

MQBYTE24 ls_InitValue = "<--------First-------->";

m_NLSChrSet = new CString(NLS_CHAR_SET);
m_BitFields = new CString("\0\0\0\0\0\0");
m_BitFields->getPointer()[0] |= PreNLSFmt;        // default: Old Format
operator=(&ls_InitValue);
return;
}

//---------------------------------------------------------------------+
//  Method:   Destructor
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CKeyField::~CKeyField(void){
delete m_NLSChrSet;
delete m_BitFields;
m_NLSChrSet = (CString*) NULL;
m_BitFields = (CString*) NULL;
m_CurrTBuf  = (char*) NULL;
return;
}
