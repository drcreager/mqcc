//---------------------------------------------------------------------+
//  File:     PutSingle.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CPutSingle class.
//
//            This class represents the "Put Single Queue" 
//            algorithm (Mode 1).  It is a descendent of CPut.
//
//            The model here is that the Transfer is File to Queue and
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

#ifndef __PUTSINGLE_H__
#define __PUTSINGLE_H__
#include "Put.h"



//---------------------------------------------------------------------+
//  Class:    CPutSingle derived from CPut
//---------------------------------------------------------------------+
class CPutSingle : public CPut {

private:

	// m_cFile is now an output file	(assume data)
	// m_cQue is now an input queue		(assume ctrl)


protected:

	virtual long CheckParms();

public:

	CPutSingle();
	virtual ~CPutSingle();
	virtual long Execute();

	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
//	virtual long termSelf();


} ;

#endif