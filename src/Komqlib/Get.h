//---------------------------------------------------------------------+
//  File:     CGet.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CGet class.
//
//            This class represents the generic "Get" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CGetBatch (Mode 1) and CGetSingle (Mode 2)
//            algorithms.
//
//            The model here is that the Transfer is Queue to File.
//
//  History: 
//  03/12/99 DMT Created original code.
//  03/02/00 DRC Added extension of DoBatchInterval
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __GET_H__
#define __GET_H__
#include "Transfer.h"




//---------------------------------------------------------------------+
//  Class:    CGet derived from CTransfer
//---------------------------------------------------------------------+
class CGet : public CTransfer {

private:

	// m_cFile is now an output file	(assume data)
	// m_cQue is now an input queue		(assume ctrl)


protected:

	virtual long CheckParms();		      // now more specific;
	virtual long CloseInput(BOOL& val);	  // Closes Input Queue;
	virtual long CloseOutput(BOOL& val);  // Closes Output File;
    virtual long DoBatchInterval();       // now more specific;
	virtual long initExecute();
	virtual long OpenInput(BOOL& val);	  // Opens Input Queue;
	virtual long OpenOutput(BOOL& val);	  // Opens Output File;
	virtual long ReceiveData();           // Gets Message off Queue
	virtual long SendData();			  // Writes Data to File
	virtual long termExecute();


public:
	CGet();
	virtual ~CGet();
	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
	virtual long termSelf();



} ;

#endif
