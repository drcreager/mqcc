//---------------------------------------------------------------------+
//  File:     DeadSingle.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CDeadSingle class.
//
//            This class represents the Dead Letter Queue Transfer
//            (Mode 1) algorithm and is derived from CDeadLetter.
//  
//            The model here is that the Transfer is Queue to Queue, 
//            from the Dead Letter Queue to an appropriate 
//            Destination Queue, depending on the source messages type.
//
//  History: 
//  03/12/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DEADSINGLE_H__
#define __DEADSINGLE_H__
#include "DeadLetter.h"

//---------------------------------------------------------------------+
//  Class:    CDeadSingle derived from CDeadLetter
//---------------------------------------------------------------------+
class CDeadSingle : public CDeadLetter {

private:


protected:

	virtual long CheckParms();

public:

	CDeadSingle();
	virtual ~CDeadSingle();
	virtual long Execute();


} ;

#endif