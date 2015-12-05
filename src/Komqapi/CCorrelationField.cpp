/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21	@(#) CCorrelationField.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CCorrelationField.cpp
//  Author:   Dale Thompson
//  Written:  September 1999
//  Desc:     Implementation of the CCorrelationField class. 
//
//
//  Dev Log: 
//  01)  Add Copy Constructor
//  02)  Add Assignment Operator
//
//  History: 
//  09/21/99  Created original code.
//---------------------------------------------------------------------+
#ifndef _CCorrelationField
#define _CCorrelationField
#include "CCorrelationField.h"
#endif


//---------------------------------------------------------------------+
//  Method:   encode
//  Desc:     Translate the buffer contents from the Host character set
//            into a neutral representation called the NLS character set 
//---------------------------------------------------------------------+
void CCorrelationField::encode(MQBYTE24* rs_Arg1){

if (isChanged(m_Buf2, rs_Arg1)) {
	CKeyField::operator=(rs_Arg1);
	if ( *(rs_Arg1)[0] > '\0'){
		HostToNLS();
	}
	else {
		m_CurrTBuf  = m_Buf1;
	}
}
getBuf(rs_Arg1, m_Buf1);
}


//---------------------------------------------------------------------+
//  Method:   decode
//  Desc:     Translate the buffer contents from the Host character set
//            into a neutral representation called the NLS character set 
//---------------------------------------------------------------------+
void CCorrelationField::decode(MQBYTE24* rs_Arg1){

if (isNLSKey(rs_Arg1)) {
	if (isChanged(m_Buf1, rs_Arg1)){ 
		CKeyField::operator=(rs_Arg1);
		NLSToHost();
	}
	getBuf(rs_Arg1, m_Buf2);
}
} /* End of decode Method */


//---------------------------------------------------------------------+
//  Method:   HostToNLS
//  Desc:     Extend the Ancestral method with specialized behaviors 
//            for this class. 
//---------------------------------------------------------------------+
void CCorrelationField::HostToNLS(){
CKeyField::HostToNLS(6);
//
//  Set the Message Indicators from the current object state
//
for (int i=0; i<6; i++) *(m_CurrTBuf+i) = m_BitFields->getPointer()[i];
invertBits(m_CurrTBuf);
}


//---------------------------------------------------------------------+
//  Method:   NLSToHost
//  Desc:     Extend the Ancestral method with specialized behaviors 
//            for this class. 
//---------------------------------------------------------------------+
void CCorrelationField::NLSToHost(){
int i;

CKeyField::NLSToHost(6);
//
//  Set the Message Indicators in the current object state
//
invertBits(m_CurrTBuf);
for (i=0; i<6; i++) m_BitFields->getPointer()[i] = *(m_CurrTBuf+i);

//
//  Restore the leading zeros in the user's copy of this field
//
for (i=0; i<6; i++) *(m_CurrTBuf+i) = '0';
}


//---------------------------------------------------------------------+
//  Method:   invertBits
//  Desc:     This method reverses the current bit field settings.
//
//            The bitField can easily contain a byte with all indicators
//            turned off.  This makes it a null which is used as the 
//            termination character for strings and can be confusing 
//            to various library methods (e.g. strncmp, strcpy, etc), 
//            debuggers and file editors. 
// 
//            This method addresses this problem by reversing (inverting)
//            the setting of each individual bit in the field.  This 
//            action virtually guarantees the absence of null characters
//            within the bit field of the correlation id.
//
//  History: 
//  02/25/2000 DRC changed algorithm to leave the Key Format bits alone.
//---------------------------------------------------------------------+
void  CCorrelationField::invertBits (char* rs_Arg1){
for (int i=0; i<6; i++) rs_Arg1[i] = rs_Arg1[i]^KEY_MASK;
}


#ifdef _temp001_
//---------------------------------------------------------------------+
//  Method:   =  (Inbound Assignment)
//  Desc:     Copy a MQBYTE24 field into this object.  
//
//  09/22/99  DRC Added to facilitate handling MQBYTE24 datatype.
//---------------------------------------------------------------------+
void CCorrelationField::operator=  (MQBYTE24* rs_Arg1){
CKeyField::operator=(rs_Arg1);

//
//  Reset this object's indicators
//
clrLogRcpt();
clrDataRcpt();
setOldKey();

if (isNLSKey(rs_Arg1)) {
	setNLSKey();
	if (isLogRcpt(rs_Arg1))  setLogRcpt();
	if (isDataRcpt(rs_Arg1)) setDataRcpt();

}  /*  End If */

}  /*  End of Operator= Method */
#endif

//---------------------------------------------------------------------+
//  Method:   getBuf
//  Desc:     Translate the buffer contents from the Host character set
//            into a neutral representation called the NLS character set 
//---------------------------------------------------------------------+
void CCorrelationField::getBuf(MQBYTE24* rs_Arg1, char* rs_Arg2){

CKeyField::getBuf(rs_Arg1, rs_Arg2);
}


//---------------------------------------------------------------------+
//  Method:   Constuctor
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CCorrelationField::CCorrelationField(){
}

//---------------------------------------------------------------------+
//  Method:   Destructor
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CCorrelationField::~CCorrelationField(void){
}
