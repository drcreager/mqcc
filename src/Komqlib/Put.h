//---------------------------------------------------------------------+
//  File:     CPut.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CPut class.
//
//            This class represents the generic "Put" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CPutBatch (Mode 1) and CPutSingle (Mode 2)
//            algorithms.
//
//            The model here is that the Transfer is File to Queue.
//
//  History: 
//  03/22/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __PUT_H__
#define __PUT_H__
#include "Transfer.h"
#include <time.h>
#include "CSysCmd.h"




//---------------------------------------------------------------------+
//  Class:    CPut derived from CTransfer
//---------------------------------------------------------------------+
class CPut : public CTransfer {

private:
	CSysCmd* m_SysCmd;

	// m_cFile is now an input file		(assume data)
	// m_cQue is now an output queue	(assume ctrl)


protected:

	virtual long CheckParms();					// now more specific;
	virtual long OpenInput(BOOL& val);			// Opens Input File;
	virtual long OpenOutput(BOOL& val);			// Opens Output Queue;

	virtual long EstablishRestart();
	
	virtual long ReceiveData(); 
												// Gets Message off File

	virtual long SendData();					// Writes Data to Queue
	virtual long CloseInput(BOOL& val);			// Closes Input File;
	virtual long CloseOutput(BOOL& val);		// Closes Output Queue;
	virtual long initExecute();
	virtual long termExecute();
	virtual long GenerateBatchID(CString& v_cBatchID);


public:

	CPut();
	virtual ~CPut();
	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
	virtual long termSelf();



} ;

#endif

