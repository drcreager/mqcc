//---------------------------------------------------------------------+
//  File:     DeadReport.h
//  Author:   David M. Tamkun
//  Written:  April 1999
//  Desc:     Declaration file for the CDeadReport class.
//
//            This class represents the Dead Letter Diagnostic
//            Report (Mode 2) algorithm and is derived from
//            CDeadLetter.
// 
//            The model here is that the "Transfer" is Queue to 
//            Collection Class (for Reporting).  Note that the input
//            is a browse and is not destructive.
//
//  History: 
//  03/12/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DEADREPORT_H__
#define __DEADREPORT_H__
#include "DeadLetter.h"
#include "CtlFile.h"
#include "DeadBatches.h"

//---------------------------------------------------------------------+
//  Class:    CDeadReport derived from CDeadLetter
//---------------------------------------------------------------------+
class CDeadReport : public CDeadLetter {

private:


protected:

	CCtlFile		m_cCtlFile;
	CDeadBatches	m_cDeadBatches;

	virtual long CheckParms();
	virtual long OpenInput(BOOL& rb_input_open);
	virtual long initExecute();
	virtual long OpenOutput();
	virtual long SendData();
	virtual long CloseOutput(BOOL &val);
	virtual long termExecute();
	

public:

	CDeadReport();
	virtual ~CDeadReport();
	virtual long Execute();


} ;

#endif