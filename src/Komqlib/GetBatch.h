//---------------------------------------------------------------------+
//  File:     GetBatch.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CGetBatch class.
//
//            This class represents the "Get Batch" 
//            algorithm (Mode 1).  It is a descendent of CGet.
//
//            The model here is that the Transfer is Queue to File and
//            involves a Data Queue and a Control Queue.
//  History: 
//  03/12/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif



#ifndef __GETBATCH_H__
#define __GETBATCH_H__

#include "Get.h"
#include "CtlQue.h"




//---------------------------------------------------------------------+
//  Class:    CGetBatch derived from CGet
//---------------------------------------------------------------------+
class CGetBatch : public CGet {

private:

	// m_cFile is now an output file	(assume data)
	// m_cQue is now an input queue		(assume ctrl)


protected:

	virtual long CheckParms();
	virtual BOOL CanDoFinalCommit();
	virtual long GetControlMsg();
	virtual long ShutdownRestart();
	virtual long CheckFileExistence();

public:

	CGetBatch();
	virtual ~CGetBatch();
	virtual long Execute();
} ;


#endif