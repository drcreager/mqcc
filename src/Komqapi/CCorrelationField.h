/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21     @(#) CCorrelationField.h 1.0@(#)
 */
//-----------------------------------------------------------------------+
//  File:     CCorrelationField.h
//  Author:   Daniel R Creager
//  Written:  September 1999
//  Desc:     Declaration file for the CCorrelationField class. 
//
//            A CorrelationField is a KeyField which has been segmented 
//            into two sub-fields.  
//
//            char BitFields[6];   Untranslated - Bit values assigned
//            char MsgSeq[18];     Translated   - Numeric digits only
//
//            Assigned Bit Values are as follows: 
//            Bit   7654 3210
//            Byte0 .... ....
//                  |||| |||+-->  Unassigned Default is x'00'
//                  |||| ||+--->  Unassigned Default is x'00'
//                  |||| |+---->  Unassigned Default is x'00'
//                  |||| +----->  Unassigned Default is x'00'
//                  |||+------->  Key Format Indicator 
//                  |||           0 = NLS format
//                  |||           1 = Pre-NLS format
//                  ||+-------->  Reserved for future use
//                  |+--------->  1 = Data Format Receipt Requested
//                  +---------->  1 = Log  Format Receipt Requested
//
//            Byte1-5             Unassigned Default is x'00'           
//
//  History: 
//  09/21/99  Created original code.
//  10/04/99  DRC Added an overide for isChanged() to allow more 
//            class specific behaviors.
//-----------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CKeyField
//#define _CKeyField
#include "CKeyField.h"
//#endif

#define KEY_MASK 0xAA
//-----------------------------------------------------------------------+
//  Class:    CCorrelationField
//-----------------------------------------------------------------------+
class CCorrelationField : public CKeyField {
public:
    virtual void     encode     (MQBYTE24*	rs_Arg1);
    virtual void     decode     (MQBYTE24*	rs_Arg1);
    virtual void     getBuf     (MQBYTE24*	rs_Arg1, 
		                         char*		rs_Arg2);
    virtual void     invertBits (char*      rs_Arg1);
	virtual void     HostToNLS  ();
	virtual void     NLSToHost  ();
#ifdef _temp001_
    virtual void     operator=  (MQBYTE24*	rs_Arg1);
#endif

    CCorrelationField();
    ~CCorrelationField(void);
};


