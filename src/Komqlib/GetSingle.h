//---------------------------------------------------------------------+
//  File:     GetSingle.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CGetSingle class.
//
//            This class represents the "Get Single Queue" 
//            algorithm (Mode 2).  It is a descendent of CGet.
//
//            The model here is that the Transfer is Queue to File and
//            involves only a Data Queue.
//
//  History: 
//  03/12/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __GETSINGLE_H__
#define __GETSINGLE_H__
#include "Get.h"



//---------------------------------------------------------------------+
//  Class:    CGetSingle
//---------------------------------------------------------------------+
class CGetSingle : public CGet {

private:

	// m_cFile is now an output file	(assume data)
	// m_cQue is now an input queue		(assume ctrl)


protected:

	virtual long CheckParms();

public:

	CGetSingle();
	virtual ~CGetSingle();
	virtual long Execute();
} ;

#endif