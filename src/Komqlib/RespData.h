/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21     @(#) CRespData.h 1.0@(#)
 */
//-----------------------------------------------------------------------+
//  File:     CRespData.h
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Declaration file for the CRespData class. 
//
//
//  History: 
//  09/24/99  Created original code.
//-----------------------------------------------------------------------+
#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CAny
//#define _CAny
#include "CAny.h"
//#endif

//#ifndef _cmqc
//#define _cmqc
#include "cmqc.h"
//#endif

//#ifndef _memory
//#define _memory
#include <memory.h>
//#endif

//#ifndef _CString
//#define _CString
#include <CString.h>
//#endif

//
//  Defininitions for BitToChr and ChrToBit methods. 
//
#define LftSide  '\xF0'
#define RhtSide  '\x0F'
#define Bit_1    '\x40'
#define Bits_2_3 '\x30'

//-----------------------------------------------------------------------+
//  Class:    CRespData
//-----------------------------------------------------------------------+
class CRespData : public CAny {

public:
//
//  API internal parms
//
BOOL		mb_BatchContainsNLSKeys;
BOOL		mb_BatchFirstMsg;
CString*	ms_XlateRef;
CString*	ms_RestartKey;
CString*	ms_BitFields;

//
//  common parms
//
int			vi_queue_id;
char*		vs_msg_buffer;
long*		rl_data_length;    //(Put was byVal)
BOOL		vb_xlate_key_values;
BOOL		vb_data_receipt_requested;
BOOL		vb_log_receipt_requested;


//
//  standard interface parms
//
long*	rl_expiration;     //(Put was byVal)
BOOL	vb_batch_ind;
long*	rl_persistence;    //(Put was byVal)
long*	rl_priority;       //(Put was byVal)
BOOL	vb_fail_if_quiescing;
char*	vs_msg_id;
char*	vs_correl_id;
char*	vs_reply_to_q;
long*	rl_report_options; //(Put was byVal)
long*	rl_msg_type;       //(Put was byVal)
long*	rl_feedback_code;  //(Put was byVal)
char*	vs_format;         //(a.k.a vs_msg_format in Put)

//
//  get parms
//
long	vl_buffer_length;
BOOL	vb_accept_trunc;
char	vc_get_mode;
long	vl_wait_interval;
BOOL	vb_match_msg_id;
BOOL	vb_match_correl_id;
long	vl_CCSID;
long	vl_Encoding;

//
//  advanced interface parms
//
PMQMD   rmqmd_msg_desc;
long*   rl_comp_code;
long*   rl_reason_code;

//
//  advanced interface - PUTA parms
//
PMQPMO  rmqpmo_put_options;

//
//  advanced interface - GETA parms
//
PMQGMO  rmqgmo_get_options;
//long    vl_buffer_length;
		
public:
    virtual void setGet(
		            int		vi_queue_id,       
					char*	vs_msg_buffer,
					long	vl_buffer_length,
					long&	rl_data_length,
					long&	rl_expiration, 
					BOOL	vb_batch_ind,
					BOOL	vb_accept_trunc,
					long&	rl_persistence,
					char*	vs_msg_id,
					char*	vs_correl_id,
					char*	vs_reply_to_q,
					long&	rl_msg_type,
					long&	rl_feedback_code,
					long&	rl_report_options,
					char*	vs_format,
					char	vc_get_mode,
					long	vl_wait_interval,
					BOOL	vb_match_msg_id,
					BOOL	vb_match_correl_id,
					long&	rl_priority,
					BOOL	vb_fail_if_quiescing,
					BOOL	vb_xlate_key_values);

    virtual void setGetA(
					int		vi_queue_id,
					PMQMD	rmqmd_msg_desc,
					PMQGMO	rmqgmo_get_options,
					long	vl_buffer_length,
					char*	rs_msg_buffer,
					long&	rl_data_length,
					long&	rl_comp_code,
					long&	rl_reason_code,
					BOOL	vb_xlate_key_values);

    virtual void setPut(
					int		vi_queue_id,
					char*	vs_msg_buffer,
					long	vl_data_length,
					long	vl_expiration,
					BOOL	vb_batch_ind,
					long	vl_persistence,
					char*	vs_msg_id,
					char*	vs_correl_id,
					char*	vs_reply_to_q,
					long	vl_report_options,
					long	vl_msg_type,
					long	vl_feedback_code,
					char*	vs_msg_format,
					long	vl_priority,
					BOOL	vb_fail_if_quiescing,
					BOOL	vb_xlate_key_values);

    virtual void setPutA(
					int		vi_queue_id,
					PMQMD	rmqmd_msg_desc,
					PMQPMO	rmqpmo_put_options,
					long	vl_data_length,
					char*	vs_msg_buffer,
					long&	rl_comp_code,
					long&	rl_reason_code,
					BOOL	vb_xlate_key_values);

    virtual CString*	getRestartKey(void);
    virtual void		setRestartKey(char* as_Arg1);
    virtual CString*    getBitFlds   (void);
    virtual void        setBitFlds   (CString*	as_Arg1);
    virtual void		ChrToBit     (char* as_To, char* as_From);
    virtual void		BitToChr     (char* as_To, char* as_From);

protected:
    virtual void setCommonParmSet(
					int		vi_queue_id,
					char*	vs_msg_buffer,
					long*	rl_data_length,
					BOOL    vb_xlate_key_values,
					BOOL    vb_data_receipt_requested,
					BOOL    vb_log_receipt_requested);

    virtual void setStndParmSet(
					long&	rl_expiration,
					BOOL	vb_batch_ind,
					long&	rl_persistence,
					long&	rl_priority,
					BOOL	vb_fail_if_quiescing,
					char*	vs_msg_id,
					char*	vs_correl_id,
					char*	vs_reply_to_q,
					long&	rl_report_options,
					long&	rl_msg_type,
					long&	rl_feedback_code,
					char*	vs_format);

    virtual void setAdvParmSet(
					PMQMD   rmqmd_msg_desc,
					long&   rl_comp_code,
					long&   rl_reason_code);


public:
	CRespData();
    ~CRespData(void);
};
