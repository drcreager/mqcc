/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) CQueue.h 1.2@(#)
 */
//#define ATHOME
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif


#ifndef __CQUEUE_H__
#define __CQUEUE_H__
//---------------------------------------------------------------------+
//  File:     CQueue.h
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     Declaration file for the CQueue Class. 
//
//  History: 
//  11/07/98 DMT Created original version.
//  12/09/98 DMT Added report options, message type, and feedback code
//               to PUT call and added GET call.
//  09/21/99 DRC Added NLS Buffer and support for NLS format in the 
//               MessageId and the CorrelationId.
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
#include "CError.h"
#include "cmqc.h"

//#ifndef _CKeyField
//#define _CKeyField
#include "CKeyField.h"
//#endif

//#ifndef _CCorrelationField
//#define _CCorrelationField
#include "CCorrelationField.h"
//#endif

//#ifndef _CAPIParmSet
//#define _CAPIParmSet
#include "CAPIParmSet.h"
//#endif


#ifdef _WIN32
	#ifndef _windows
	#define _windows
	#include <windows.h>
	#endif
#endif

//---------------------------------------------------------------------+
//  Class:    CQueue
//---------------------------------------------------------------------+
class CQueue {
public:
CError              iu_error;

protected:
CKeyField*          iu_MsgId;                                       //DRC092199
CCorrelationField*  iu_CorrId;                                      //DRC092199
MQHOBJ              imqho_queue_handle;


public:
	virtual long Init();

	virtual long Open(MQHCONN vmqhc_connection, char* vs_q_name, 
					BOOL vb_input_shared, 
					BOOL vb_input_excl, BOOL vb_browse, 
					BOOL vb_output);


	virtual long Put(MQHCONN vmqhc_connection, char* vs_msg_buffer, 
					long vl_data_length,        long vl_expiration, 
					BOOL vb_batch_ind,          long vl_persistence,
					char* vs_msg_id,           char* vs_correl_id, 
					char* vs_reply_to_q,        long vl_report_options,
					long  vl_msg_type,          long vl_feedback_code,
					char* vs_msg_format,		long vl_priority,
					BOOL  vb_fail_if_quiescing,
					BOOL  vb_xlate_key_values,	CAPIParmSet* au_APIParms);


	virtual long PutA(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQPMO  rmqpmo_put_options, 
				  MQLONG  vl_data_length, 
				  char*   vs_msg_buffer,
				  PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms);


	virtual long Get(MQHCONN vmqhc_connection,       
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


	virtual long GetA(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  MQLONG  vl_buffer_length, 
				  char*   rs_msg_buffer,
				  PMQLONG rl_data_length,  
			      PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms);
private: 
	virtual long GetAExt(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  MQLONG  vl_buffer_length, 
				  char*   rs_msg_buffer,
				  PMQLONG rl_data_length,  
			      PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code);
public:


	virtual long Close(MQHCONN vmqhc_connection);

	virtual long TermSelf();

	CQueue( void );
	~CQueue( void );

};
#endif // __CQUEUE_H__
