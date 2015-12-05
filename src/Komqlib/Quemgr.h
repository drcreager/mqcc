/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) QueMgr.h 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     QueMgr.h
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     Declaration file for the CQueMgr class.
//
//            This class represents an MQ Series Queue Manager and
//            acts as client program "front-end" for the KO MQ Series
//            API.
//
//  History: 
//  01/15/99 CAH Created original code.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __QUEMGR_H__
#define __QUEMGR_H__
#include "CString.h"
#include "Any2.h"
#include "komqidll.h"
#include <stdlib.h>
#include <string.h>
#include "auto_ptr.h"
#include "Log.h"
#include <cmqc.h>






//---------------------------------------------------------------------+
//  Class:    CQueMgr
//---------------------------------------------------------------------+
class CQueMgr : public CAny2
{
private:
	CString m_szName;
	CString m_szLastErrContext;
	long	m_lLastCompCode;
	long	m_lLastReasonCode;
	CString m_szLastReasonStr;
	CQueMgr(CQueMgr&);

	void CheckRC(long);

	CLog*	m_pcLog;

	BOOL	ib_connected;


public:
	CQueMgr();
	virtual ~CQueMgr();
	const long GetLastReasonCode() const { return m_lLastReasonCode; }
	const long GetLastCompCode() const { return m_lLastCompCode; } 
	const char* GetLastErrContext() { return m_szLastErrContext.c_str(); }
	const char* GetLastReasonStr() { return (const char*)m_szLastReasonStr.c_str(); }

	virtual long XlateReasonCode(long vl_value, CString& rs_Name);

    long Put(	int   vi_queue_id,       
				char* vs_msg_buffer,
                long  vl_data_length,    
				long  vl_expiration,
                BOOL  vb_batch_ind,      
				long  vl_persistence,
                char* vs_msg_id,       
				char* vs_correl_id,
                char* vs_reply_to_q,    
				long  vl_report_options,
                long  vl_msg_type,      
				long  vl_feedback_code,
				char* vs_msg_format,
				long  vl_priority,
				BOOL  vb_fail_if_quiescing,
#ifdef _ver2_
				BOOL  vb_xlate_key_values,
				BOOL  vb_log_receipt,
				BOOL  vb_data_receipt);
#else
				BOOL  vb_xlate_key_values);
#endif


    long PutA(	int   vi_queue_id,       
				char* vs_msg_buffer,
                long  vl_data_length,    
				long  vl_expiration,
                BOOL  vb_batch_ind,      
				long  vl_persistence,
                char* vs_msg_id,       
				char* vs_correl_id,
                char* vs_reply_to_q,    
				long  vl_report_options,
                long  vl_msg_type,      
				long  vl_feedback_code,
				char* vs_msg_format,
				long  vl_priority,
				BOOL  vb_fail_if_quiescing,
#ifdef _ver2_
				BOOL  vb_xlate_key_values,
				BOOL  vb_log_receipt,
				BOOL  vb_data_receipt);
#else
				BOOL  vb_xlate_key_values);
#endif



    long Get(  	int   vi_queue_id,
				char* vs_msg_buffer,    
				long  vl_buffer_length,
				long& rl_data_length,   
				long& rl_expiration,
				BOOL  vb_batch_ind,     
				BOOL  vb_accept_trunc,
				long& rl_persistence,
				char* vs_msg_id,
				char* vs_correl_id,
				char* vs_reply_to_q,
				long& rl_msg_type,      
				long& rl_feedback_code,
				long& rl_report_options,
				char* vs_format,
				char  vc_get_mode,
				long  vl_wait_interval, 
				BOOL  vb_match_msg_id,   
				BOOL  vb_match_correl_id,
				long& rl_priority,
				BOOL  vb_fail_if_quiescing,
				BOOL  vb_xlate_key_values);


    long GetA(  int   vi_queue_id,
				char* vs_msg_buffer,    
				long  vl_buffer_length,
				long& rl_data_length,   
				long& rl_expiration,
				BOOL  vb_batch_ind,     
				BOOL  vb_accept_trunc,
				long& rl_persistence,
				char* vs_msg_id,
				char* vs_correl_id,
				char* vs_reply_to_q,
				long& rl_msg_type,      
				long& rl_feedback_code,
				long& rl_report_options,
				char* vs_format,
				char  vc_get_mode,
				long  vl_wait_interval, 
				BOOL  vb_match_msg_id,   
				BOOL  vb_match_correl_id,
				long& rl_priority,
				BOOL  vb_fail_if_quiescing,
				BOOL  vb_xlate_key_values);

#ifdef _ver2_
      long  GetAttrLng(  int   ai_Type);
      long  SetAttrLng(  int   ai_Type, long  al_Arg);
      char* GetAttrStr(  int   ai_Type);
      long  SetAttrStr(  int   ai_Type, char* as_Arg);

#endif
	long Commit();
	long Rollback();
    virtual long InitQ( int& ri_qid, 
						char* vs_q_name, 
						BOOL vb_input_shared,
                		BOOL vb_input_excl, 
						BOOL vb_browse, 
						BOOL vb_output );

	long Open(const char* Name);
	char* GetName() { return m_szName.c_str(); }
	long QClose(int iQueHandle);
	long Close();

	long initSelf(CLog* pLog, char* szName);
	long termSelf();
};
#endif
