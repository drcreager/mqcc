/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21     @(#) CKeyField.h 1.0@(#)
 */
//-----------------------------------------------------------------------+
//  File:     CKeyField.h
//  Author:   Daniel R Creager
//  Written:  September 1999
//  Desc:     Declaration file for the CKeyField class. 
//
//  History: 
//  09/21/99  Created original code.
//  10/04/99  DRC Modified isChanged() to include length as a parm.
//-----------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CString
//#define _CString
#include "CString.h"
//#endif

//#ifndef _cmqc
//#define _cmqc
#include "cmqc.h"
//#endif

//#ifndef NLS_CHAR_SET
#define NLS_CHAR_SET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-"
//#endif


//-----------------------------------------------------------------------+
//  Class:    CKeyField
//-----------------------------------------------------------------------+
class CKeyField : public CString {
protected: 
CString* m_NLSChrSet;           // -> Reference Character Set
char*    m_CurrTBuf;            // -> Current Translation Buffer
CString* m_BitFields;           // -> Contains object state info

enum BitField{bit0=1,  bit1=2,  bit2=4,  bit3=8, 
              bit4=16, bit5=32, bit6=64, bit7=128};

enum Byte0   {byt0bit0=1,   byt0bit1=2,  byt0bit2=4, byt0bit3=8, 
              PreNLSFmt=16, byt0bit5=32, DataRcpt=64, LogRcpt=128};


public:
    virtual int      isDataRcpt ();
    virtual int      isDataRcpt (MQBYTE24* rs_Arg1);
    virtual void     setDataRcpt();
    virtual void     clrDataRcpt();
    virtual int      isLogRcpt  ();
    virtual int      isLogRcpt  (MQBYTE24* rs_Arg1);
    virtual void     setLogRcpt ();
    virtual void     clrLogRcpt ();
    virtual int      isNLSKey   ();
    virtual int      isNLSKey   (char      rs_Arg1);
    virtual int      isNLSKey   (MQBYTE24* rs_Arg1);
    virtual void     setNLSKey  ();
    virtual void     setOldKey  ();


    virtual void     encode     (MQBYTE24* rs_Arg1);
    virtual void     decode     (MQBYTE24* rs_Arg1);
    virtual void     cancel     (MQBYTE24* rs_Arg1);
    virtual int      isChanged  (char*     rs_Arg1, 
		                         MQBYTE24* rs_Arg2);
    virtual void     HostToNLS  (void);
    virtual void     HostToNLS  (int       vi_Initial);
    virtual void     NLSToHost  (void);
    virtual void     NLSToHost  (int       vi_Initial);
    virtual void     operator=  (MQBYTE24* as_Arg1);
    virtual void     getBuf     (MQBYTE24* rs_Arg1, 
		                         char*     rs_Arg2);
    virtual CString* getBitFlds (void);
    virtual void     setBitFlds (CString*  as_Arg1);
    CKeyField();
    ~CKeyField(void);
};


