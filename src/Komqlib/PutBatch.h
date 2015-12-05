//---------------------------------------------------------------------+
//  File:     PutBatch.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CPutBatch class.
//
//            This class represents the "Put Batch" 
//            algorithm (Mode 1).  It is a descendent of CPut.
//
//            The model here is that the Transfer is File to Queue and
//            involves a Data Queue and a Control Queue.
//
//  History: 
//  03/12/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __PUTBATCH_H__
#define __PUTBATCH_H__

#include "Put.h"
#include "CtlFile.h"




//---------------------------------------------------------------------+
//  Class:    CPutBatch derived from CPut
//---------------------------------------------------------------------+
class CPutBatch : public CPut {

private:

	// m_cFile is now an input file		(assume data)
	// m_cQue is now an output queue	(assume ctrl)


protected:

	virtual long CheckParms();
	virtual BOOL CanDoFinalCommit();
	virtual long GetControlMsg();
	virtual long ShutdownRestart();
//	virtual long ReceiveData();
	virtual long SendData();					// Writes Data to Queue
	virtual long CheckFileExistence();

public:

	CPutBatch();
	virtual ~CPutBatch();
	virtual long Execute();

//	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
//	virtual long termSelf();


} ;


#endif