//---------------------------------------------------------------------+
//  File:     CDeadLetter.h
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     Declaration file for the CDeadLetter class.
//
//            This class represents the generic "Dead Letter" 
//            algorithm.  It is a descendent of CTransfer and is the
//            ancestor of CDeadSingle (Mode 1) and CDeadReport (Mode 2)
//            algorithms.
//  
//            The model here is that the Transfer is Queue to Queue, 
//            from the Dead Letter Queue to an appropriate 
//            Destination Queue, depending on the source messages type.
//  
//  History: 
//  03/24/99 DMT Created original code.
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __DEADLETTER_H__
#define __DEADLETTER_H__
#include "Transfer.h"
#include <cmqc.h>

//---------------------------------------------------------------------+
//  Class:    CDeadLetter derived from CTransfer
//---------------------------------------------------------------------+
class CDeadLetter : public CTransfer {

private:

	// m_cFile Not used								
	// m_cQue is now an input Dead Letter queue		(assume ctrl)



protected:

	CQue	m_cDataQue;		// Dead Data Msgs go here
	CQue	m_cCtrlQue;		// Dead Request Msgs go here
	CQue	m_cReplyQue;	// Dead Reply Msgs go here
	CQue	m_cErrorQue;	// Dead Report Msgs go here

	CBuffer	m_cOutputBuffer;

	long	m_lNumDataMsgs;
	long	m_lNumCtrlMsgs;
	long	m_lNumReplyMsgs;
	long	m_lNumErrorMsgs;

	CString	m_sDestQueueName;		// Dest Queue Name in Dead Ltr Hdr
	CString	m_sDestQueueMgrName;	// Dest Q Mgr Name in Dead Ltr Hdr
	CString	m_sFormat;				// Original Msg Format in DLH

	long	m_lDeadReason;			// Dead Reason in Dead Letter Hdr

	char	m_chrGetMode;			// Get Mode to be used by
	                                // ReceiveData (Get vs BrowseNext)

	BOOL	m_bSyncpointEnable;		// Used to set Get Options



	virtual long CheckParms();				// now more specific;
	virtual long OpenInput(BOOL& val);		// Opens Input Queue

	virtual long OpenOutput(BOOL& val);		// Stub to Override Anc.
	virtual long OpenOutput();				// Opens Output Queues;

	virtual long ReceiveData(); 
											// Gets Message off Queue

	virtual long SendData();				// Opens Queue if necessary

	virtual long CloseInput(BOOL& val);		// Closes Input Queue;
	virtual long CloseOutput(BOOL& val);	// Closes Output Queues;
	virtual long initExecute();
	virtual long termExecute();
	virtual BOOL CanDoFinalCommit();
	
	virtual long RemoveDeadLetterHeader(CBuffer& cOutput, 
		                                CBuffer& cInput);


public:

	CDeadLetter();
	virtual ~CDeadLetter();
	virtual long initSelf(CStartParamFile* v_pcParms, CLog* v_pcLog);
	virtual long termSelf();



} ;

#endif
