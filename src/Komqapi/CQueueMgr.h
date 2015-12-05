/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: %E%	%A%
 */
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#ifndef __CQUEUEMGR_H__
#define __CQUEUEMGR_H__
//---------------------------------------------------------------------+
//  File:     CQueueMgr.h
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     Declaration file for the CQueueMgr Class. 
//
//  History: 
//  11/07/98 DMT Created original version.
//  12/09/98 DMT Changed Put Call to add Report Options, Msg Type, and
//               Feedback Code.  Added Get Call.
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
#include "CError.h"
#include "CQueues.h"
#include "CQueue.h"

//#ifndef _CAPIParmSet
//#define _CAPIParmSet
#include "CAPIParmSet.h"
//#endif

//#ifndef _cmqc
//#define _cmqc
#include "cmqc.h"
//#endif

#ifdef _WIN32
//#ifndef _windows
//#define _windows
#include <windows.h>
//#endif
#endif

//---------------------------------------------------------------------+
//  Class:    CQueueMgr
//---------------------------------------------------------------------+
class CQueueMgr {
public:
CError		iu_error;
MQHCONN		imqhc_q_mgr_conn;
CQueues*	iu_queues;


public:
	virtual long Init( char* vs_q_mgr_name );

	virtual long InitQ( int& ri_qid, char* vs_q_name, BOOL vb_input_shared, 
				BOOL vb_input_excl, BOOL vb_browse, BOOL vb_output );

	virtual long Put(int vi_queue_id,       char* vs_msg_buffer, 
					long vl_data_length,    long vl_expiration, 
					BOOL vb_batch_ind,      long vl_persistence,
					char* vs_msg_id,       char* vs_correl_id,
					char* vs_reply_to_q,    long vl_report_options,
					long  vl_msg_type,      long vl_feedback_code,
					char* vs_msg_format,	long vl_priority,
					BOOL  vb_fail_if_quiescing,
					BOOL  vb_xlate_key_values,	
					CAPIParmSet* au_APIParms);




	virtual long PutA(int vi_queue_id,
		          PMQMD   rmqmd_msg_desc, 
				  PMQPMO  rmqpmo_put_options, 
				  long    vl_data_length, 
				  char*   vs_msg_buffer,
				  long&   rl_comp_code,
				  long&   rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms);





	virtual long Get(  int vi_queue_id,       
		             char* vs_msg_buffer, 	 long  vl_buffer_length,
					 long& rl_data_length,   long& rl_expiration, 
					  BOOL vb_batch_ind,     BOOL  vb_accept_trunc,
					 long& rl_persistence,
					 char* vs_msg_id,
					 char* vs_correl_id,
					 char* vs_reply_to_q,
					 long& rl_msg_type,      long& rl_feedback_code,
					 long& rl_report_options,
					 char* vs_format,
					  char vc_get_mode,
					  long vl_wait_interval,
					  BOOL vb_match_msg_id,   BOOL vb_match_correl_id,
					 long& rl_priority,
					  BOOL vb_fail_if_quiescing,
					  BOOL vb_xlate_key_values,
					  CAPIParmSet* au_APIParms);


	virtual long GetA(int vi_queue_id,
		          PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  long    vl_buffer_length, 
				  char*   rs_msg_buffer,
				  long&   rl_data_length,  
			      long&   rl_comp_code,
				  long&   rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms);


	virtual long Commit( void );
	virtual long Rollback( void );
	virtual long Close( int vi_queue_id );
	virtual long Disconnect( void );
	virtual long TermSelf( void );
	virtual long GetLastError( long& rl_comp_code, long& rl_reason_code,
							   char* is_context,   long vl_buffer_size);

	virtual long GetReasonCode( long vl_value, char* vs_reason, 
							    long vl_buffer_size );



	CQueueMgr( void );
	~CQueueMgr( void );
};

#endif // __CQUEUEMGR_H__
