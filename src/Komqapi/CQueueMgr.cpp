/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) CQueueMgr.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     CQueueMgr.cpp
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     This file contains the implementation of the CQueueMgr
//            Class.
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+

#include "CQueueMgr.h"







//---------------------------------------------------------------------+
//  Method:   Init
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Calls the MQ Series API function MQCONN to connect to a 
//            queue manager.
//
//  Args:     Name of the Queue Manager to Connect To
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::Init ( char* vs_q_mgr_name ){


	MQCHAR48	lmqc48_queue_mgr_name;
	MQLONG		lmql_comp_code = 0;
	MQLONG		lmql_reason_code = 0;



	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+

	// Make sure we aren't already connected to a Queue Manager
	iu_error.CheckError((imqhc_q_mgr_conn == MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_ALREADY_CONNECTED, "CQueueMgr::Init.Parameter Validation");

	// Queue Manager Name cannot be a NULL pointer
	iu_error.CheckError((vs_q_mgr_name != NULL), 
		2, KORC_INVALID_QMGR_NAME, "CQueueMgr::Init.Parameter Validation");

	if(vs_q_mgr_name != NULL) {
		// if it is and we call stlen, we'll crash
		// Queue Manager Name cannot be an empty string
		iu_error.CheckError((strlen(vs_q_mgr_name) > 0), 
			2, KORC_INVALID_QMGR_NAME, "CQueueMgr::Init.Parameter Validation");

		// Queue Manager Name cannot be too big.
		iu_error.CheckError((strlen(vs_q_mgr_name) <= MQ_Q_MGR_NAME_LENGTH), 
			2, KORC_INVALID_QMGR_NAME, "CQueueMgr::Init.Parameter Validation");
	}

	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}




	//strcpy queue manager name into variable for mqi call.
	strncpy(lmqc48_queue_mgr_name, vs_q_mgr_name, MQ_Q_MGR_NAME_LENGTH);






	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQCONN
	//-----------------------------------------------------------------+
#ifdef ATHOME
	imqhc_q_mgr_conn = 1;
#else
	MQCONN(lmqc48_queue_mgr_name, &imqhc_q_mgr_conn, &lmql_comp_code,
			&lmql_reason_code);
#endif


	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue Manager::Init";
	}

	return 	iu_error.il_comp_code;

}






//---------------------------------------------------------------------+
//  Method:   InitQ
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Adds a Queue to the Queue Collection and Opens the Queue.
//
//  Args:     O  ri_qid           - Queue Handle - Pass this to
//                                  Get, Put, and Close Methods to
//                                  specify the queue.  Not modified
//                                  unless queue opens successfully.
//
//            I  vs_q_name        - Queue Name
//
//            I  vb_input_shared  - TRUE if you want to open this
//                                  Queue for Input and allow others
//                                  to do the same.
//
//               ** Shared And Exclusive cannot both be TRUE **
//
//            I  vb_input_excl    - TRUE if you want to open this
//                                  Queue for Input and NOT allow 
//                                  others to do the same.
//
//            I  vb_browse        - TRUE if you want to be able to
//                                  Browse Messages on the Queue.
//                                  (View but not remove them.)
//
//            I  vb_output        - TRUE if you want to Put Msgs on
//                                  the queue.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log:  If for some reason, Opening the Queue fails and Removing
//            the unopened Queue from the Queues Collection fails,
//            the "Remove from Collection" error is not reported
//            because error handling only supports 1 error.
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::InitQ( int& ri_qid, char* vs_q_name, BOOL vb_input_shared, 
				BOOL vb_input_excl, BOOL vb_browse, BOOL vb_output ) {

	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;
	int		li_qid		= -1;	


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();

	
	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, 
		"CQueueMgr::InitQ.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}



	//-----------------------------------------------------------------+
	//  Add a Queue to the Queue Collection
	//-----------------------------------------------------------------+
	ll_return = iu_queues->AddQueue(lu_queue, li_qid);

	if (ll_return == 0) {

		//-------------------------------------------------------------+
		//  Open the Queue
		//-------------------------------------------------------------+
		ll_return = lu_queue->Open(imqhc_q_mgr_conn, vs_q_name, 
								vb_input_shared, vb_input_excl, 
								vb_browse, vb_output);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Open Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;


			//---------------------------------------------------------+
			// Need to Tell Queue Mgr to Remove Q.
			//---------------------------------------------------------+
			ll_return = iu_queues->RemoveQueue(li_qid);

			//---------------------------------------------------------+
			// TODO 
			// If RemoveQueue Fails, No Error is reported, although
			// we already are in an Error Situation.
			//---------------------------------------------------------+

		}
		else {
			// open was good, return queue id to caller
			ri_qid = li_qid;
		}

	}
	else {
		//-------------------------------------------------------------+
		//	AddQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;

}










//---------------------------------------------------------------------+
//  Method:   Put
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Puts a Message on a Queue.  Edits for KO Standards 
//            beforehand.  The MQPMO_DEFAULT_CONTEXT option is always
//            on.
//
//  Args:     I  vi_queue_id          - Queue "Handle" returned from
//                                      InitQ 
//
//            I  vs_msg_buffer        - Pointer to Data Buffer
//
//            I  vl_data_length       - Length of Data in Message
//
//            I  vl_expiration        - Expiration Interval - if 
//                                      unlimited (-1), reset to 30 days
//
//            I  vb_batch_ind         - TRUE if want to use 
//                                      COMMIT/ROLLBACK
//
//            I  vl_persistence       - 0 if msgs should be logged
//                                        while on Queue Managers.
//
//                                      1 if msgs should not be logged.
//
//                                      2 means MQ Series uses Queue 
//                                        Default
//
//                                      < 0 Func applies KO default (0)
//
//            I  vs_msg_id            - Message ID (required)
//
//            I  vs_correl_id         - Correlation ID (required)
//
//            I  vs_reply_to_q        - ReplyTo Queue Name (required)
//
//            I  vl_report_options    - Report Options (as defined my 
//                                      MQ Series)
//
//            I  vl_msg_type          - Message Type 
//
//                                      If <= 0, Reset to 8
//
//                                      1 = Request  (Control Msg)
//                                      2 = Reply    (Reply Msg)
//                                      4 = Report   (Error Msg)
//                                      8 = Datagram (Data Msg)
//                            
//
//            I  vl_feedback_code     - Feedback Code
//
//            I  vs_msg_format        - Message Format.  Must be 
//                                      "MQSTR",     "MQSTR   ",
//                                      "MQHSAP", or "MQHSAP  "  If
//                                      "      ", changed to "MQSTR"
//
//            I  vl_priority          - Message Priority.  Can be 
//                                      between 0 and the platform Max 
//                                      (ideally, want 5)
//
//                                      If less then 
//                                      MQPRI_PRIORITY_AS_Q_DEF (-1),
//                                      changed to 5.
//
//            I  vb_fail_if_quiescing - if TRUE Put will Fail if Queue
//                                      Manager is Shutting Down
//
//            I  vb_xlate_key_values  - TRUE if Msg ID and Correl ID
//                                      should be translated to EBCDIC 
//                                      before putting on Queue.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//  12/09/98 DMT Added Report Options, MessageType, and Feedback Code
//               to parameter list.
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
long CQueueMgr::Put(int		vi_queue_id,			char*	vs_msg_buffer, 
					long	vl_data_length,			long	vl_expiration, 
					BOOL	vb_batch_ind,			long	vl_persistence,
					char*	vs_msg_id,				char*	vs_correl_id,
					char*	vs_reply_to_q,			long	vl_report_options,
					long	vl_msg_type,			long	vl_feedback_code,
					char*	vs_msg_format,			long	vl_priority,
					BOOL	vb_fail_if_quiescing,
					BOOL	vb_xlate_key_values,	CAPIParmSet* au_APIParms){

	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;




	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();




	//-----------------------------------------------------------------+
	// GetQueue - Have the Queues Collection resolve the "Queue ID"
	//            passed from the caller into a pointer to the 
	//            corresponding Queue Class.
	//
	//            If Queue was never opened or if it didn't open
	//            successfully, the queue ID would not resolve to
	//            a valid queue.
	//-----------------------------------------------------------------+
	ll_return = iu_queues->GetQueue(lu_queue, vi_queue_id);

	if (ll_return == 0) {

		// Put
		ll_return = lu_queue->Put(imqhc_q_mgr_conn, vs_msg_buffer, 
								vl_data_length,     vl_expiration, 
								vb_batch_ind,       vl_persistence,
								vs_msg_id,			vs_correl_id,
								vs_reply_to_q,      vl_report_options,
								vl_msg_type,        vl_feedback_code,
					            vs_msg_format,	    vl_priority,
					            vb_fail_if_quiescing,
					            vb_xlate_key_values,	au_APIParms);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Put Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;
			
		}

	}
	else {
		//-------------------------------------------------------------+
		//	GetQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;
}











//---------------------------------------------------------------------+
//  Method:   PutA
//  Author:   David M Tamkun
//  Written:  April 1999
//
//  Desc:     MQ Series Put Function (Administrative Level).  No
//            KO Edits.
//
//
//  Args:     I  vi_queue_id         - Queue "Handle" of Queue to Put
//                                     Msg On.
//
//            IO rmqmd_msg_desc      - Ptr to Message Descriptor Struc.
//
//            IO rmqpmo_put_options  - Ptr to Put Options Struc.
//
//            I  vl_data_length      - Data Size
//
//            I  vs_msg_buffer       - Pointer to Buffer Containing Msg.
//
//            O  rl_comp_code        - MQ Series Completion Code
//
//            O  rl_reason_code      - MQ Series Reason Code
//
//            I  vb_xlate_key_values - TRUE if Msg ID and Correl ID 
//                                     should be translated to EBCDIC
//                                     before Get and again After Get.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//            
//  
//  Dev Log:  TODO - Implement ASCII/EBCDIC Stuff at Queue Level
//
//  History: 
//  04/01/99  DMT Created original version.
//  09/24/99  DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
long CQueueMgr::PutA(int vi_queue_id,
				  PMQMD   rmqmd_msg_desc, 
				  PMQPMO  rmqpmo_put_options, 
				  long    vl_data_length, 
				  char*   vs_msg_buffer,
				  long&   rl_comp_code,
				  long&   rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms) {
				  
	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;



	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();




	//-----------------------------------------------------------------+
	// GetQueue - Have the Queues Collection resolve the "Queue ID"
	//            passed from the caller into a pointer to the 
	//            corresponding Queue Class.
	//
	//            If Queue was never opened or if it didn't open
	//            successfully, the queue ID would not resolve to
	//            a valid queue.
	//-----------------------------------------------------------------+
	ll_return = iu_queues->GetQueue(lu_queue, vi_queue_id);

	if (ll_return == 0) {

		// Put
		ll_return = lu_queue->PutA(imqhc_q_mgr_conn, 
			              rmqmd_msg_desc, 
				          rmqpmo_put_options, 
				          vl_data_length, 
				          vs_msg_buffer,
				          &rl_comp_code,
				          &rl_reason_code,
				          vb_xlate_key_values,	au_APIParms);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Put Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;
			
		}

	}
	else {
		//-------------------------------------------------------------+
		//	GetQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;
}










//---------------------------------------------------------------------+
//  Method:   Get
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Browses or Gets message off the queue based on the options
//            passed. MQGMO_CONVERT option is always turned on.
//
//            MQGMO_WAIT is always effectively turned on as well, since
//            we replace a wait interval of 0 with 5 seconds. 
//
//            Null terminates received data.
//
//  Args:     I  vi_queue_id        Queue "Handle" returned from
//                                  InitQ 
//
//            I vs_msg_buffer		pointer to caller allocated buffer
//                                  to receive message data.
//
//            I vl_buffer_length	length of caller allocated buffer.
//                                  Buffer must be large enough to
//                                  receive data and have 1 byte extra
//                                  for a terminating NULL character.
// 
//                                  The last byte of the buffer is 
//                                  reserved for a terminating NULL.
//
//                                  The Maximum Buffer Size is
//                                  2,093,153 bytes.  The largest Msg
//                                  permitted is 1 byte less.
//
//            O rl_data_length      length of data in buffer, or 
//                                  total length of data if msg
//                                  is truncated.
//
//                                  The largest Msg permitted is 
//                                  2,093,152 bytes.
//                                  
//            O rl_expiration		expiration interval of message
//
//            I vb_batch_ind		TRUE if Syncpoint option should be
//                                  turned on for the GET.
//
//            I vb_accept_trunc		TRUE if GET should accept truncated
//                                  messages if the message buffer 
//                                  isn't big enough.
//
//            O  rl_persistence		0 if Persistent, 1 if NOT persistent,
//                                  2=Use Queue Default for Persistence.
//
//            IO vs_msg_id			Pointer to caller allocated buffer
//                                  to receive Message ID.  Must be
//                                  preallocated to at least 25 bytes.
//                                  24 bytes for data plus NULL char.
//                                  
//            IO vs_correl_id       Pointer to caller allocated buffer
//                                  to receive Correlation ID.  Must be
//                                  preallocated to at least 25 bytes.
//                                  24 bytes for data plus NULL char.
//
//            O vs_reply_to_q       Pointer to caller allocated buffer
//                                  to receive ReplyToQ Name.  Must be
//                                  preallocated to at least 49 bytes.
//                                  48 bytes for data plus NULL char.
//
//            O rl_msg_type         Message Type of Message
//
//            O rl_feedback_code    Feedback Code of Message
//
//            O rl_report_options   Report Options of Message
//
//            O vs_format           Pointer to caller allocated buffer
//                                  to receive Format Indicator. Must be
//                                  preallocated to at least 9 bytes.
//                                  8 bytes for data plus NULL char.
//
//            I vc_get_mode         'G' for Get
//                                  'F' for Browse First Message
//                                  'N' for Browse Next Message
//                                  'U' for Get Msg Under Cursor(1/7/99)
//
//            I vl_wait_interval    # of milliseconds to wait for new
//                                  messages to arrive before returning.
//
//                                  0 = don't wait. (But Overridden)
//
//                                  Must be >= 0 and <= 15000.
//
//                                  If 0, overridden to 5 seconds (5000)
//
//            I vb_match_msg_id     If TRUE, Get or Browse will only
//                                  return msg with matching MsgID.
//
//            I vb_match_correl_id  If TRUE, Get or Browse will only
//                                  return msg with matching CorrelID. 
//
//            O rl_priority         Returns Msg Priority
//
//            I vb_fail_if_quiescing Turns on FAIL If Quiescing option
//                                   if TRUE
//
//            I vb_xlate_key_values Translates MSG ID and Correl ID 
//                                  from EBCDIC to ASCII values.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//
//  Dev Log: 
//  History: 
//  12/10/98 DMT Created Original Version
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
long CQueueMgr::Get(   int vi_queue_id,       
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
					CAPIParmSet* au_APIParms) {



	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;




	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();

	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, "CQueueMgr::Get.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}



	//-----------------------------------------------------------------+
	// GetQueue - Have the Queues Collection resolve the "Queue ID"
	//            passed from the caller into a pointer to the 
	//            corresponding Queue Class.
	//
	//            If Queue was never opened or if it didn't open
	//            successfully, the queue ID would not resolve to
	//            a valid queue.
	//-----------------------------------------------------------------+
	ll_return = iu_queues->GetQueue(lu_queue, vi_queue_id);

	if (ll_return == 0) {

		// Get
		ll_return = lu_queue->Get(imqhc_q_mgr_conn,        
		                          vs_msg_buffer, 	 vl_buffer_length,
								  rl_data_length,    rl_expiration, 
								  vb_batch_ind,      vb_accept_trunc,
								  rl_persistence,
					              vs_msg_id,
					              vs_correl_id,
					              vs_reply_to_q,
					              rl_msg_type,       rl_feedback_code,
					              rl_report_options,
					              vs_format,
					              vc_get_mode,
					              vl_wait_interval,  
								  vb_match_msg_id,   vb_match_correl_id,
					              rl_priority,
					              vb_fail_if_quiescing,
					              vb_xlate_key_values,	au_APIParms);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Get Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;
			
		}

	}
	else {
		//-------------------------------------------------------------+
		//	GetQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;

}











//---------------------------------------------------------------------+
//  Method:   GetA
//  Author:   David M Tamkun
//  Written:  April 1999
//
//  Desc:     MQ Series Get Function (Administrative Level)
//
//            Note this function does NOT NULL terminate received 
//            Msg Data.
//
//  Args:     I  vi_queue_id         - Queue "Handle" of Queue to Get
//                                     Msg From.
//
//            IO rmqmd_msg_desc      - Ptr to Message Descriptor Struc.
//
//            IO rmqgmo_get_options  - Ptr to Get Options Struc.
//
//            I  vl_buffer_length    - Buffer Size
//
//            O  rs_msg_buffer       - Pointer to Buffer to Receive Msg.
//
//            O  rl_data_length      - Data Length Returned or Size of
//                                     Full Message if too larg for 
//                                     Buffer
//
//            O  rl_comp_code        - MQ Series Completion Code
//
//            O  rl_reason_code      - MQ Series Reason Code
//
//            I  vb_xlate_key_values - TRUE if Msg ID and Correl ID 
//                                     should be translated to EBCDIC
//                                     before Get and again After Get.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//            
//  
//  Dev Log:  TODO - Implement ASCII/EBCDIC Stuff at CQueue Level
//
//  History: 
//  04/01/99  DMT Created original version.
//  09/24/99  DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
long CQueueMgr::GetA(int vi_queue_id,
				  PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  long    vl_buffer_length, 
				  char*   rs_msg_buffer,
				  long&   rl_data_length,  
			      long&   rl_comp_code,
				  long&   rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms) {


	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;




	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();



	//-----------------------------------------------------------------+
	// GetQueue - Have the Queues Collection resolve the "Queue ID"
	//            passed from the caller into a pointer to the 
	//            corresponding Queue Class.
	//
	//            If Queue was never opened or if it didn't open
	//            successfully, the queue ID would not resolve to
	//            a valid queue.
	//-----------------------------------------------------------------+
	ll_return = iu_queues->GetQueue(lu_queue, vi_queue_id);

	if (ll_return == 0) {

		// Get
		ll_return = lu_queue->GetA(imqhc_q_mgr_conn,
			              rmqmd_msg_desc, 
				          rmqgmo_get_options, 
				          vl_buffer_length, 
				          rs_msg_buffer,
				          &rl_data_length,  
			              &rl_comp_code,
				          &rl_reason_code,
				          vb_xlate_key_values,	au_APIParms);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Get Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;
			
		}

	}
	else {
		//-------------------------------------------------------------+
		//	GetQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;

}











//---------------------------------------------------------------------+
//  Method:   Commit
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Commits All Queue Activity if Gets/Puts were done with
//            the Batch Indicator (aka Synchpoint) Option ON.
//
//            Note that this affects all pending work of all queues 
//            of the connected Queue Manager
//
//  Args:     None
//				 
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log: 
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::Commit( void ) {

	MQLONG		lmql_comp_code   = 0L;
	MQLONG		lmql_reason_code = 0L;

	
	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, 
		"CQueueMgr::Commit.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}

	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQCMIT
	//-----------------------------------------------------------------+
#ifndef ATHOME
	MQCMIT(imqhc_q_mgr_conn, &lmql_comp_code, &lmql_reason_code);
#endif


	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue Manager::Commit";
	}

	return 	iu_error.il_comp_code;

}










//---------------------------------------------------------------------+
//  Method:   Rollback
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Rolls Back All Queue Activity if Gets/Puts were done with
//            the Batch Indicator (aka Synchpoint) Option ON.
//
//            Note that this affects all pending work of all queues 
//            of the connected Queue Manager
//
//  Args:     None
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::Rollback( void ) {


	MQLONG		lmql_comp_code   = 0L;
	MQLONG		lmql_reason_code = 0L;

	
	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();

	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, 
		"CQueueMgr::Rollback.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}

	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQBACK
	//-----------------------------------------------------------------+
#ifndef ATHOME
	MQBACK(imqhc_q_mgr_conn, &lmql_comp_code, &lmql_reason_code);
#endif


	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue Manager::Rollback";
	}

	return iu_error.il_comp_code;
}













//---------------------------------------------------------------------+
//  Method:   Close
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Closes the specified queue.
//
//  Args:     The Queue Handle returned from InitQ.
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log:  If for some reason, removing
//            the Closed Queue from the Queues Collection fails,
//            the "Remove from Collection" error is not reported
//            because error handling only supports 1 error.
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::Close( int vi_queue_id ) {


	long	ll_return	= 0;
	CQueue* lu_queue	= NULL;


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, 
		"CQueueMgr::Close.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}


	//-----------------------------------------------------------------+
	// GetQueue - Have the Queues Collection resolve the "Queue ID"
	//            passed from the caller into a pointer to the 
	//            corresponding Queue Class.
	//
	//            If Queue was never opened or if it didn't open
	//            successfully, the queue ID would not resolve to
	//            a valid queue.
	//-----------------------------------------------------------------+
	ll_return = iu_queues->GetQueue(lu_queue, vi_queue_id);

	if (ll_return == 0) {

		// Close
		ll_return = lu_queue->Close(imqhc_q_mgr_conn);

		if(ll_return > 0) {
			//---------------------------------------------------------+
			//	Close Failed
			//
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lu_queue->iu_error.il_reason_code;
			iu_error.il_comp_code   = lu_queue->iu_error.il_comp_code;
			*iu_error.is_error_context = 
					lu_queue->iu_error.is_error_context;			
		}

		//-------------------------------------------------------------+
		//	Close Successful, ask Queues Collection to Remove the 
		//  Queue Object.
		//-------------------------------------------------------------+
		ll_return = iu_queues->RemoveQueue(vi_queue_id);

		//-------------------------------------------------------------+
		// TODO 
		// If RemoveQueue Fails, No Error is reported, although
		// we already are in an Error Situation.
		//-------------------------------------------------------------+

	}
	else {
		//-------------------------------------------------------------+
		//	GetQueue Failed
		//
		//  Set Error State and Context
		//-------------------------------------------------------------+
		iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
		iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
		*iu_error.is_error_context = 
			iu_queues->iu_error.is_error_context;
	}

	return iu_error.il_comp_code;

}












//---------------------------------------------------------------------+
//  Method:   Disconnect
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Disconnects from the Queue Manager.  
//
//            Any outstanding activity (Gets/Puts) will be committed.
//
//  Args:     None
//
//  Returns:  MQ Series Completion Code 
//            0 if okay, 1 if Warning, 2 if Error
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::Disconnect( void ) {


	MQLONG		lmql_comp_code   = 0;
	MQLONG		lmql_reason_code = 0;

	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();



	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	iu_error.CheckError((imqhc_q_mgr_conn != MQHC_UNUSABLE_HCONN), 
		2, KORC_QMGR_NOT_CONNECTED, 
		"CQueueMgr::Disconnect.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}



	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQDISC
	//-----------------------------------------------------------------+
#ifndef ATHOME
	MQDISC(&imqhc_q_mgr_conn, &lmql_comp_code, &lmql_reason_code);
#endif


	//-----------------------------------------------------------------+
	//  Set Error State and Context if Disconnect Fails
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK){
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue Manager::Disconnect";
	}
	else {

		//-------------------------------------------------------------+
		//  Need to tell Queues Collection to Remove All Its Queues
		//  since we successfully disconnected from the Queue Mgr.
		//-------------------------------------------------------------+
		lmql_comp_code = iu_queues->RemoveAll();

		if (lmql_comp_code > 0) {
			//---------------------------------------------------------+
			//  Set Error State and Context if RemoveAll Fails
			//---------------------------------------------------------+
			iu_error.il_reason_code = iu_queues->iu_error.il_reason_code;
			iu_error.il_comp_code   = iu_queues->iu_error.il_comp_code;
			lmql_comp_code          = iu_error.il_comp_code;
			*iu_error.is_error_context = 
				iu_queues->iu_error.is_error_context;
		}
	}

	return iu_error.il_comp_code;
}













//---------------------------------------------------------------------+
//  Method:   GetLastError
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Returns the completion and reason codes and the error
//            context string from the last operation.
//
//  Args:     ref long Completion Code
//
//            ref long Reason Code
//
//            pointer to caller allocated string buffer to receive 
//                 error context
//
//            long size of allocated buffer including terminating
//                 NULL character
//
//  Returns:  -1 if buffer size <= 0 or buffer pointer is NULL
//
//             0 if everything is okay
//
//             >0 buffer size required for Context Contents Plus
//                 terminating NULL character.
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::GetLastError( long& rl_comp_code, long& rl_reason_code,
							  char* is_context, long vl_buffer_size ){


	if(is_context == NULL || vl_buffer_size <= 0) {
		// Buffer Parameters Invalid Return -1
		return -1;
	}

	rl_comp_code   = iu_error.il_comp_code;
	rl_reason_code = iu_error.il_reason_code;

	if (iu_error.is_error_context->vlen() >= (ulong) vl_buffer_size) {

		// Buffer Too Small, return size needed + 1 for NULL Terminator.
		return (long) iu_error.is_error_context->vlen() + 1;
	}
	else {
		// Copy Context to Buffer and Return 0
		strncpy(is_context, iu_error.is_error_context->getPointer(), 
			vl_buffer_size);
		return 0;
	}

}











//---------------------------------------------------------------------+
//  Method:   GetReasonCode
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Returns the string descriptive value of the
//            specified numeric reason code.
//
//  Args:     long    Error Value to Get Reason Name of
//
//			  pointer to caller allocated buffer
//
//            long    length of buffer including terminating NULL.
//
//  Returns:  -1 if there's an buffer parameters are not valid
//             0 if successful
//            >0 the required buffer size to hold the description and
//               the NULL terminator.
//  
//  Dev Log: 
//  History: 
//  11/10/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::GetReasonCode( long vl_value, char* vs_reason, 
							   long vl_buffer_size ){

	CString ls_name(KODEFAULT_STRING_SIZE);

	if(vs_reason == NULL || vl_buffer_size <= 0) {
		// Buffer parameters invalid, return -1
		return -1;
	}

	iu_error.GetReasonCodeName(vl_value, &ls_name);

	if (ls_name.vlen() >= (ulong) vl_buffer_size) {
		// Buffer is too small, return size needed for value
		// and NULL terminator.
		return (long) ls_name.vlen() + 1;
	}
	else {
		strncpy(vs_reason, ls_name.getPointer(), 
			vl_buffer_size);
		return 0;
	}
}







//---------------------------------------------------------------------+
//  Method:   TermSelf
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     TBD
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueueMgr::TermSelf( void ) {

return 0;

}

	


//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:  Consider moving the memory allocation to initSelf so we
//            can return a return code if something in the 
//            constructor fails.
//
//            How can we report this error now?  Don't want to put up
//            a MessageBox because this code is non-visual and have 
//            no guarantee anyone would be around to see it.
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
CQueueMgr::CQueueMgr( void ){

	imqhc_q_mgr_conn = MQHC_UNUSABLE_HCONN;

	try {
		iu_queues = new CQueues;
	}

	catch(...) {
		// How to report Error?
	}

}




//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:  Consider moving this to a termSelf() method and have
//            Destructor always call termSelf()
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
CQueueMgr::~CQueueMgr( void ){

	iu_queues->RemoveAll();
	delete iu_queues;

}
