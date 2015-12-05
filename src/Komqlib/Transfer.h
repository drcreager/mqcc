//---------------------------------------------------------------------+
//  File:     Transfer.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CTransfer class.
//
//            This class represents the generic "Transfer" 
//            algorithm.  It is the
//            ancestor of the CPut, CGet, and CDeadLetter
//            algorithms.
//
//            At this level, it is not known if the transfer is 
//            file to queue or queue to file.
//
//  History: 
//  03/12/99 DMT Created original code.
//  09/20/00 DRC Adjusted CFile references to CMsgFile.
//  11/09/00 DRC Added the GetStatus method.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "Service.h"
#include "StartupParamFile.h"
#include "QueMgr.h"
#include "MsgFile.h"
#include "Que.h"
//#include "Log.h"
#include "CBuffer.h"
#include "RestartFile.h"





//---------------------------------------------------------------------+
//  Class:    CTransfer
//---------------------------------------------------------------------+
class CTransfer : public CService {

private:


protected:

    CStartParamFile*    m_pcParms;

	/*
	CLog*				m_pcLog;
    */
	CQueMgr			    m_cQueMgr;
	CMsgFile		    m_cFile;
	CQue				m_cQue;
	CBuffer			    m_cBuffer;
	CRestartFile		m_cRestart;
	CString			    m_cStatus;


	// Descendents populate these members before calling Execute
	// Execute never knows if it's being called in a Data Pass or
	// a Control Pass.
	long                m_lBatchInterval;
	CString             m_sFileName;
	CString             m_sQueueName;
	long                m_lExpectedMsgs;
	long                m_lUncommittedMsgs;
	long				m_lTotalMsgsProcessed;
	long				m_lMsgsProcessed;
	long                m_lSynchpoints;
	long				m_bRestart;
	CString             m_sBatchID;
	long				m_lCorrelID;
	

	// If On, sequences Gets and Puts
	// If off and m_lCorrelID = 0, not used to match Gets
	// If off and value > 0, used to match get or used for
	//                       all PUTs.
	BOOL				m_bSequence;

	// Off suppresses logging status message with each synchpoint.
	// Turned off for Dead Letter Processing.
	BOOL				m_bLogSynchpoint;

	// Off suppresses use of Restart File
	// Turned off for Dead Letter Processing
	BOOL				m_bEnableRestart;


	BOOL                m_bAutoBalance;


	virtual BOOL  CanDoFinalCommit();
	virtual long  CheckParms();
	virtual long  CloseInput(BOOL& rb_input_open) = 0;
	virtual long  CloseOutput(BOOL& rb_output_open) = 0;
	virtual long  CommitWork();
	virtual long  DoBatchInterval();
	virtual long  EstablishRestart();
	virtual long  initExecute() = 0;
	virtual long  OpenInput(BOOL& rb_input_open) = 0;
	virtual long  OpenOutput(BOOL& rb_output_open) = 0;
	virtual long  ReceiveData() = 0;
	virtual long  SendData() = 0;
	virtual long  ShutdownRestart();
	virtual long  termExecute() = 0;


public:
	CTransfer();
	virtual ~CTransfer();

	virtual char* GetStatus();
	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
	virtual long termSelf();

	virtual long Execute();



};

#endif