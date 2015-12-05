/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) CQueue.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     CQueue.cpp
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     This file contains the implementation of the CQueue
//            Class.
//  
//  Dev Log: 
//  01)  Need to add logic to sub-divide the CorrelationId into 
//       two discrete fields (Indicators(char[6]) + MessageNum(num[18])
//
//  History: 
//  11/07/98 DMT Created original version.
//  04/01/99 DMT Restructured, added Admin Get and Put
//  09/21/99 DRC Added NLS Buffer and support for NLS format in the 
//               MessageId and the CorrelationId.
//  09/24/99 DRC Added support for CAPIParmSet
//---------------------------------------------------------------------+
#include "CQueue.h"




//---------------------------------------------------------------------+
//  Method:   Init
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Does Nothing
//  Args:     Nothing
//  Returns:  0
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::Init (){

return 0;

}






//---------------------------------------------------------------------+
//  Method:   Open
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Opens a Queue - The Fail If Quiescing Option is always
//            turned ON.
//
//  Args:     I  vmqhc_connection - Queue Manager Connection Handle
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
//  Returns:  0 if successful, 1 if there's a Warning, 2 if an Error.
//  
//  Dev Log:  Consider adding an Admin Level for this function.
//            Right now there is not much benefit in doing so, since
//            we allow almost everything one can do in an open with
//            this method.
//  History: 
//  11/11/98  DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::Open( MQHCONN vmqhc_connection, char* vs_q_name, 
					BOOL vb_input_shared, 
					BOOL vb_input_excl, BOOL vb_browse, 
					BOOL vb_output ) {


	MQLONG		lmql_comp_code = 0L;
	MQLONG		lmql_reason_code = 0L;
	MQOD		lmqod_object_desc = { MQOD_DEFAULT };	// sets default 
														// values in the 
														// structure
	MQLONG      lmql_open_options = 0L;



	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();



	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+

	// Queue Manager Not Connected.
	iu_error.CheckError((vmqhc_connection	  != MQHC_UNUSABLE_HCONN), 
	 2, KORC_QMGR_NOT_CONNECTED, "CQueue::PutA.Parameter Validation");


	// Both input options cannot be true
	iu_error.CheckError(!(vb_input_shared && vb_input_excl), 
		2, KORC_INVALID_OPEN_OPT, "CQueue::Open.Parameter Validation");

	// At least 1 of the input options must be true
	iu_error.CheckError((vb_input_shared || vb_input_excl || 
						 vb_browse       || vb_output), 
		2, KORC_INVALID_OPEN_OPT, "CQueue::Open.Parameter Validation");

	// Queue Name cannot be a NULL pointer
	iu_error.CheckError((vs_q_name != NULL), 
		2, KORC_INVALID_Q_NAME, "CQueue::Open.Parameter Validation");


	if(vs_q_name != NULL){

		// We cannot call strlen if the pointer is NULL
		//		We'll crash.

		// Queue Name cannot be an empty string
		iu_error.CheckError((strlen(vs_q_name) > 0), 
			2, KORC_INVALID_Q_NAME, "CQueue::Open.Parameter Validation");

		// Queue Name cannot be too big.
		iu_error.CheckError((strlen(vs_q_name) <= MQ_Q_NAME_LENGTH), 
			2, KORC_INVALID_Q_NAME, "CQueue::Open.Parameter Validation");
	}


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}
	

	//-----------------------------------------------------------------+
	//  Populate Object Descriptor Structure
	//-----------------------------------------------------------------+

	//strcpy queue name into ObjectName element of structure for mqi call.
	strncpy(lmqod_object_desc.ObjectName, vs_q_name, 
			MQ_Q_NAME_LENGTH);


	//-----------------------------------------------------------------+
	//  Establish open options based on booleans passed in.
	//-----------------------------------------------------------------+
	if (vb_input_shared) {
		lmql_open_options = lmql_open_options | MQOO_INPUT_SHARED;
	}

	if (vb_input_excl) {
		lmql_open_options = lmql_open_options | MQOO_INPUT_EXCLUSIVE;
	}

	if (vb_browse) {
		lmql_open_options = lmql_open_options | MQOO_BROWSE;
	}

	if (vb_output) {
		lmql_open_options = lmql_open_options | MQOO_OUTPUT;
	}

	// force the Fail If Quiescing option
	lmql_open_options = lmql_open_options | MQOO_FAIL_IF_QUIESCING;




	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQOPEN
	//-----------------------------------------------------------------+
#ifdef ATHOME
	imqho_queue_handle = 1;
#else
	MQOPEN(vmqhc_connection, &lmqod_object_desc, lmql_open_options, 
			&imqho_queue_handle, &lmql_comp_code, &lmql_reason_code);
#endif


	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue::Open";
	}

	return 	iu_error.il_comp_code;

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
//  Args:     I  vmqhc_connection     - Queue Manager Connection Handle 
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
//  12/09/98 DMT Added Report Options, Message Type, and Feedback Code.
//---------------------------------------------------------------------+
long CQueue::Put(MQHCONN vmqhc_connection, char* vs_msg_buffer, 
					long vl_data_length,    long vl_expiration, 
					BOOL vb_batch_ind,      long vl_persistence,
					char* vs_msg_id,       char* vs_correl_id,
					char* vs_reply_to_q,    long vl_report_options,
					long  vl_msg_type,      long vl_feedback_code,
					char* vs_msg_format,    long vl_priority,
					BOOL  vb_fail_if_quiescing,
					BOOL  vb_xlate_key_values,
					CAPIParmSet* au_APIParms) {


	MQMD	lmqmd_msg_desc		= {MQMD_DEFAULT}; // message_descriptor
	MQPMO	lmqpmo_put_options  = {MQPMO_DEFAULT};// put message options
	MQLONG	lmql_comp_code		= 0L;
	MQLONG	lmql_reason_code	= 0L;




	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//
	//  NOTE:  Assuming MQ Interface validates MessageType, 
	//         Report Options, and Feedback Code.
	//-----------------------------------------------------------------+


	//-----------------------------------------------------------------+
	// Check Buffer Pointer & Size
	//
	// *** KO OVERRIDE *** 
	//
	// Limit Msg Size to Maximum allowed by ISO.
	//-----------------------------------------------------------------+
	iu_error.CheckError((vs_msg_buffer != NULL), 
		2, KORC_INVALID_BUFFER, "CQueue::Put.Parameter Validation");

	if (vs_msg_buffer != NULL) {
	iu_error.CheckError((strlen(vs_msg_buffer) <= KO_MAX_BUFFER_SIZE), 
		2, KORC_BUFFER_TOO_BIG, "CQueue::Put.Parameter Validation");
	}


	//-----------------------------------------------------------------+
	// Check Message ID
	//
	// *** KO OVERRIDE *** 
	//
	// Require Message ID
	//-----------------------------------------------------------------+
	iu_error.CheckError((vs_msg_id     != NULL), 
		2, KORC_INVALID_MSG_ID, "CQueue::Put.Parameter Validation");

	if (vs_msg_id     != NULL){
		iu_error.CheckError(((strlen(vs_msg_id) > 0) && 
						 	 (strlen(vs_msg_id) <= MQ_MSG_ID_LENGTH)), 
			2, KORC_INVALID_MSG_ID, "CQueue::Put.Parameter Validation");
	}


	//-----------------------------------------------------------------+
	// Check Correllation ID
	//
	// *** KO OVERRIDE *** 
	//
	// Require Correl ID
	//-----------------------------------------------------------------+
	iu_error.CheckError((vs_correl_id     != NULL), 
		2, KORC_INVALID_CORREL_ID, "CQueue::Put.Parameter Validation");

	if (vs_correl_id     != NULL){

		iu_error.CheckError(((strlen(vs_correl_id) > 0) && 
							(strlen(vs_correl_id) <= MQ_CORREL_ID_LENGTH)), 
			2, KORC_INVALID_CORREL_ID, "CQueue::Put.Parameter Validation");
	}


	//-----------------------------------------------------------------+
	// Check Reply To Queue
	//
	// *** KO OVERRIDE *** 
	//
	// Require ReplyToQueue Name
	//-----------------------------------------------------------------+
	iu_error.CheckError((vs_reply_to_q     != NULL), 
		2, KORC_INVALID_REPLY_Q_NAME, "CQueue::Put.Parameter Validation");


	if(vs_reply_to_q     != NULL){

		iu_error.CheckError(((strlen(vs_reply_to_q) > 0) && 
							(strlen(vs_reply_to_q) <= MQ_Q_NAME_LENGTH)), 
			2, KORC_INVALID_REPLY_Q_NAME, "CQueue::Put.Parameter Validation");
	}


	//-----------------------------------------------------------------+
	// Check Data Length
	//
	// *** KO OVERRIDE *** 
	//
	// Limit Msg Size to Maximum allowed by ISO.
	//-----------------------------------------------------------------+
	iu_error.CheckError((vl_data_length >= 0 && 
		                 vl_data_length <= KO_MAX_MSG_SIZE), 
		2, KORC_INVALID_MSG_SIZE, "CQueue::Put.Parameter Validation");


	//-----------------------------------------------------------------+
	// Check Expiration
	//
	// *** KO OVERRIDE ***
	//
	// Override Expiration Interval and set to 30 days if it's set
	// to "Unlimited" (-1) or is invalid (<= 0)
	//-----------------------------------------------------------------+
	if (vl_expiration <= 0) {
		vl_expiration = KO_MAX_EXPIRY;
	}
	
	//-- Above override makes this always valid
	//
	//iu_error.CheckError(((vl_expiration == MQEI_UNLIMITED) || 
	//					 (vl_expiration >  0)), 
	//	2, KORC_INVALID_EXPIRE, "CQueue::Put.Parameter Validation");
	

	



	//-----------------------------------------------------------------+
	// Check Msg Type
	//
	// *** KO OVERRIDE ***  
	//
	// If <= 0, reset to DATAGRAM (8)
	//-----------------------------------------------------------------+
	if (vl_msg_type <= 0) {
		vl_msg_type = MQMT_DATAGRAM;
	}
  
	iu_error.CheckError((vl_msg_type == MQMT_REQUEST  ||
	                     vl_msg_type == MQMT_REPLY    ||
						 vl_msg_type == MQMT_DATAGRAM ||
						 vl_msg_type == MQMT_REPORT),
						 2, KORC_INVALID_MSG_TYPE,
                         "CQueue::Put.Parameter Validation");



	//-----------------------------------------------------------------+
	// Check Msg Format
	//
	// *** KO OVERRIDE ***  
	//
	// Allow only MQSTR or MQHSAP, if 8 spaces, default to string.
	//-----------------------------------------------------------------+
	// Make sure it's not NULL
	iu_error.CheckError((vs_msg_format     != NULL), 
		2, KORC_INVALID_MSG_FORMAT, "CQueue::Put.Parameter Validation");


	// Make sure it's not too big or zero length
	if(vs_msg_format     != NULL){

		iu_error.CheckError(((strlen(vs_msg_format) > 0) && 
			(strlen(vs_msg_format) <= sizeof(lmqmd_msg_desc.Format))), 
			2, KORC_INVALID_MSG_FORMAT, 
			"CQueue::Put.Parameter Validation");


		// if 8 spaces, default to STRING
		if(strlen(vs_msg_format) >= 8) {
			if(strncmp(vs_msg_format, KO_USE_DEFAULT_FORMAT, 8) == 0) {
				strncpy(vs_msg_format, MQFMT_STRING, 8);
			}
		}


		// Make sure it's a valid value.
		iu_error.CheckError(((strcmp(vs_msg_format, KOFMT_STRING) == 0) ||
			                 (strcmp(vs_msg_format, MQFMT_STRING) == 0) ||
							 (strcmp(vs_msg_format, KOFMT_SAP)    == 0) ||
							 (strcmp(vs_msg_format, KOFMT8_SAP)   == 0) ),
							2, KORC_INVALID_MSG_FORMAT,
							"CQueue::Put.Parameter Validation");

	}


	
	//-----------------------------------------------------------------+
	// Check Priority
	//
 	// *** KO OVERRIDE ***  
	//
	// If < MQPRI_PRIORITY_AS_Q_DEF (-1), set to KO Default of 5
	//-----------------------------------------------------------------+
	if (vl_priority < MQPRI_PRIORITY_AS_Q_DEF) {
		vl_priority = KO_DEFAULT_PRIORITY;
	}



	//-----------------------------------------------------------------+
	// Check Persistence
	//
 	// *** KO OVERRIDE ***  If <  MQPER_NOT_PERSISTENT       0L, 
	//                      set to KO Default of MQPER_PERSISTENT
	//-----------------------------------------------------------------+
	if (vl_persistence < MQPER_NOT_PERSISTENT) {
		vl_persistence = MQPER_PERSISTENT;
	}

	iu_error.CheckError((vl_persistence == MQPER_PERSISTENT     ||	// 1
		          vl_persistence == MQPER_NOT_PERSISTENT        ||	// 0
			      vl_persistence == MQPER_PERSISTENCE_AS_Q_DEF),	// 2
			2, KORC_INVALID_PERSIST,
			"CQueue::Put.Parameter Validation");



	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}





	//-----------------------------------------------------------------+
	//  Populate Message Descriptor
	//-----------------------------------------------------------------+
	lmqmd_msg_desc.Report         = vl_report_options;		//DMT120998
	lmqmd_msg_desc.MsgType        = vl_msg_type;			//DMT120998
	lmqmd_msg_desc.Expiry         = vl_expiration;
	lmqmd_msg_desc.Encoding       = MQENC_NATIVE;
	lmqmd_msg_desc.CodedCharSetId = MQCCSI_Q_MGR;
	lmqmd_msg_desc.Feedback       = vl_feedback_code;		//DMT120998

	memcpy(lmqmd_msg_desc.Format, vs_msg_format,
		strlen(vs_msg_format));

	lmqmd_msg_desc.Priority       = vl_priority;

	
	lmqmd_msg_desc.Persistence	  = vl_persistence;

	memcpy(lmqmd_msg_desc.MsgId, vs_msg_id, strlen(vs_msg_id));

	memcpy(lmqmd_msg_desc.CorrelId, vs_correl_id, strlen(vs_correl_id));

	memcpy(lmqmd_msg_desc.ReplyToQ, vs_reply_to_q, 
			strlen(vs_reply_to_q));



	//-----------------------------------------------------------------+
	//  Establish message options
	//-----------------------------------------------------------------+
	lmqpmo_put_options.Options =  MQPMO_DEFAULT_CONTEXT;

	if (vb_fail_if_quiescing) {
		lmqpmo_put_options.Options = lmqpmo_put_options.Options |
									MQPMO_FAIL_IF_QUIESCING;
	}


	if (vb_batch_ind) {
		lmqpmo_put_options.Options = lmqpmo_put_options.Options |
									MQPMO_SYNCPOINT;
	}
	else {
		lmqpmo_put_options.Options = lmqpmo_put_options.Options |
									MQPMO_NO_SYNCPOINT;
	}




	//-----------------------------------------------------------------+
	//  Call Put Administrative Layer
	//-----------------------------------------------------------------+
	PutA (vmqhc_connection, &lmqmd_msg_desc, 
			&lmqpmo_put_options, vl_data_length, vs_msg_buffer, 
			&lmql_comp_code, &lmql_reason_code,
			vb_xlate_key_values,	au_APIParms);


	//-----------------------------------------------------------------+
	//  Error State and Context Set in PutA
	//-----------------------------------------------------------------+

	return 	iu_error.il_comp_code;

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
//  Args:     I  vmqhc_connection    - Queue Mgr Connection Handle
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
//  Dev Log:  TODO - Implement ASCII/EBCDIC Stuff
//
//  History: 
//  04/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::PutA(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQPMO  rmqpmo_put_options, 
				  MQLONG  vl_data_length, 
				  char*   vs_msg_buffer,
				  PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code,
				  BOOL    vb_xlate_key_values,	
				  CAPIParmSet* au_APIParms) {



	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	// Queue Manager Not Connected.
	iu_error.CheckError((vmqhc_connection	  != MQHC_UNUSABLE_HCONN), 
	 2, KORC_QMGR_NOT_CONNECTED, "CQueue::PutA.Parameter Validation");

	// Queue Not Open
	iu_error.CheckError((imqho_queue_handle != MQHO_UNUSABLE_HOBJ), 
	 2, KORC_INVALID_QID_NOT_OPEN, "CQueue::PutA.Parameter Validation");
    


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {

		*rl_comp_code   = iu_error.il_comp_code;
	    *rl_reason_code = iu_error.il_reason_code;
		return iu_error.il_comp_code;
	}


	//-----------------------------------------------------------------+
	//  Set the Data Receipt Requested Indicator in the Msg //DRC092199
	//-----------------------------------------------------------------+
	if (au_APIParms->vb_data_receipt) {
		iu_MsgId->setDataRcpt();
		iu_CorrId->setDataRcpt();
	}

	//-----------------------------------------------------------------+
	//  Set the Log Receipt Requested Indicator in the Msg  //DRC092199
	//-----------------------------------------------------------------+
	if (au_APIParms->vb_log_receipt) {
		iu_MsgId->setLogRcpt();
		iu_CorrId->setLogRcpt();
	}

	//-----------------------------------------------------------------+
	//  Translate Msg ID and/or Correl ID (if present) to NLSCharSet if
	//  requested.
	//  
	//  Implemented support for NLS Characterset             //DRC092199
	//-----------------------------------------------------------------+
	if (vb_xlate_key_values) {
		iu_MsgId->encode(&rmqmd_msg_desc->MsgId);
		iu_CorrId->encode(&rmqmd_msg_desc->CorrelId);
	}

#ifndef ATHOME
	MQPUT (vmqhc_connection, imqho_queue_handle,
			rmqmd_msg_desc, rmqpmo_put_options, 
			vl_data_length, vs_msg_buffer,
			rl_comp_code, rl_reason_code);
#endif



	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (*rl_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) *rl_reason_code;
		iu_error.il_comp_code   = (long) *rl_comp_code;
		*iu_error.is_error_context = "Queue::PutA";
	}

	//-----------------------------------------------------------------+
	//  Translate Msg ID and/or Correl ID (if present) back to ASCII if
	//  requested.
	//  
	//  Implemented support for NLS Characterset             //DRC092199
	//-----------------------------------------------------------------+
	if (vb_xlate_key_values) {
		iu_MsgId->decode(&rmqmd_msg_desc->MsgId);
		iu_CorrId->decode(&rmqmd_msg_desc->CorrelId);
	}

	return iu_error.il_comp_code;

}






//---------------------------------------------------------------------+
//  Method:   Get
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Browses or Gets message off the queue based on the options
//            passed. MQGMO_CONVERT option is always turned on.
//
//            MQGMO_WAIT is always effectively turned on as well, since
//            we replace a wait interval of 0 with 5 seconds. 
//
//            Null terminates received data.
//
//            Issues a warning if the Msg Format or the received 
//            Message is not MQSTR or MQHSAP.
//
//  Args:     I vmqhc_connection	Queue Manager Connection Handle
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
//  Dev Log:  TODO Need to determine where we should put a Warning
//                 if a retrieved Msg appears to contain EBCDIC 
//                 Msg and Correl IDs.
//
//  History: 
//  12/10/98 DMT Created original version.
//  01/07/99 DMT Modified GET to support Match Options and the Get
//               Message Under Cursor Option.
//---------------------------------------------------------------------+
 
long   CQueue::Get(MQHCONN vmqhc_connection,       
		             char* vs_msg_buffer, 	 
					 long  vl_buffer_length,
					 long& rl_data_length,   
					 long& rl_expiration, 
					  BOOL vb_batch_ind,     
					  BOOL vb_accept_trunc,
					 long& rl_persistence,
					 char* vs_msg_id,
					 char* vs_correl_id,
					 char* vs_reply_to_q,
					 long& rl_msg_type,      
					 long& rl_feedback_code,
					 long& rl_report_options,
					 char* vs_format,
					  char vc_get_mode,
					  long vl_wait_interval, 
					  BOOL vb_match_msg_id,   
					  BOOL vb_match_correl_id,
					 long& rl_priority,
					  BOOL vb_fail_if_quiescing,
					  BOOL vb_xlate_key_values,	
			  CAPIParmSet* au_APIParms) {


	MQMD	lmqmd_msg_desc      = {MQMD_DEFAULT};	// message_descriptor
	MQGMO	lmqgmo_get_options  = {MQGMO_DEFAULT};	// get message options
	MQLONG	lmql_comp_code		= 0L;
	MQLONG	lmql_reason_code	= 0L;


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();



	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+

	//-----------------------------------------------------------------+
	// Check Buffer Pointer & Size
	//
	// *** KO OVERRIDE*** Restrict Buffer Size to size of ISO allowed
	//                    Max Msg Size.
	//-----------------------------------------------------------------+
	iu_error.CheckError((vs_msg_buffer != NULL), 
		2, KORC_INVALID_BUFFER, 
		"CQueue::Get.Buffer is NULL");

	iu_error.CheckError(vl_buffer_length > 1, 
		2, KORC_BUFFER_TOO_SMALL, 
		"CQueue::Get.Buffer Length Too Small");

	iu_error.CheckError(vl_buffer_length <= KO_MAX_BUFFER_SIZE, 
		2, KORC_BUFFER_TOO_BIG, 
		"CQueue::Get.Buffer Length Too Large");

	//-----------------------------------------------------------------+
	// Check GetMode
	//-----------------------------------------------------------------+
	iu_error.CheckError((vc_get_mode == 'G' ||
		                 vc_get_mode == 'F' ||
						 vc_get_mode == 'N' ||
						 vc_get_mode == 'U'),
		2, KORC_INVALID_GET_MODE, 
		"CQueue::Get.Get Mode Not G, F, N, or U");


	//-----------------------------------------------------------------+
	// Check Message ID Buffer
	//-----------------------------------------------------------------+
	iu_error.CheckError(vs_msg_id != NULL, 
		2, KORC_INVALID_BUFFER, 
		"CQueue::Get.NULL Message Buffer");

	
	//-----------------------------------------------------------------+
	// Check Correlation ID Buffer
	//-----------------------------------------------------------------+
	iu_error.CheckError(vs_correl_id != NULL, 
		2, KORC_INVALID_BUFFER, 
		"CQueue::Get.NULL Correlation Id Buffer");

	
	//-----------------------------------------------------------------+
	// Check Reply To Queue Buffer
	//-----------------------------------------------------------------+
	iu_error.CheckError(vs_reply_to_q != NULL, 
		2, KORC_INVALID_BUFFER, 
		"CQueue::Get.NULL ReplyTo Queue Buffer");


	//-----------------------------------------------------------------+
	// Check Message Format Buffer
	//-----------------------------------------------------------------+
	iu_error.CheckError(vs_format != NULL, 
		2, KORC_INVALID_BUFFER, 
		"CQueue::Get.NULL Message Format Buffer");

	
	//-----------------------------------------------------------------+
	// Check Wait Interval
	//
	// *** KO OVERRIDE*** 
	//
	// if <=0 (0 = No Wait) or > KO_MAX (15 seconds), 
	// set to KO Default of 5 seconds.
	//-----------------------------------------------------------------+
	if (vl_wait_interval <= 0 ||
		vl_wait_interval >  KO_MAX_WAIT_INTERVAL) {

		vl_wait_interval = KO_DEFAULT_WAIT_INTERVAL;
	}

	//-- Override above means this will always be valid.
	//--
	//iu_error.CheckError((vl_wait_interval >= 0 &&
	//		             vl_wait_interval <= KO_MAX_WAIT_INTERVAL),
	//	2, KORC_INVALID_WAIT_INTERVAL, 
	//	"CQueue::Get.Invalid Wait Interval");


	

	
	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}


	
	//-----------------------------------------------------------------+
	//  Populate Message Descriptor - enabled 1/7/99
	//
	//  If these fields are populated, Get will try to match on these.
	//  This isn't what the manual says, so only copy ids over
	//  if user said they wanted to match via the match options.
	//-----------------------------------------------------------------+
	if((strlen(vs_msg_id) > 0) && vb_match_msg_id) {
		memcpy(lmqmd_msg_desc.MsgId, vs_msg_id, MQ_MSG_ID_LENGTH);
	}
    
	if((strlen(vs_correl_id) > 0) && vb_match_correl_id) {
		memcpy(lmqmd_msg_desc.CorrelId, vs_correl_id, 
			MQ_CORREL_ID_LENGTH);
	}



	//-----------------------------------------------------------------+
	//  Establish message options
	//-----------------------------------------------------------------+
	//
	// Set Match Options - Enabled 1/7/99
	lmqgmo_get_options.MatchOptions = 0;

	if(vb_match_msg_id) {
		lmqgmo_get_options.MatchOptions += MQMO_MATCH_MSG_ID;
	}
	
	if(vb_match_correl_id) {
		lmqgmo_get_options.MatchOptions += MQMO_MATCH_CORREL_ID;
	}

	// Set Other Get Options
	lmqgmo_get_options.Options = MQGMO_CONVERT;

	if(vb_batch_ind) {
		lmqgmo_get_options.Options += MQGMO_SYNCPOINT;
	}
	else {
		lmqgmo_get_options.Options += MQGMO_NO_SYNCPOINT;
	}


	if(vb_accept_trunc) {
		lmqgmo_get_options.Options += MQGMO_ACCEPT_TRUNCATED_MSG;
		// There is no converse option for this, so there's no
		// else needed.
	}


	if(vb_fail_if_quiescing) {
		lmqgmo_get_options.Options += MQGMO_FAIL_IF_QUIESCING;
		// There is no converse option for this, so there's no
		// else needed.
	}


	if(vl_wait_interval > 0) {
		lmqgmo_get_options.Options += MQGMO_WAIT;
		lmqgmo_get_options.WaitInterval = vl_wait_interval;
	}
	else {
		lmqgmo_get_options.Options += MQGMO_NO_WAIT;
	}


	switch (vc_get_mode) {
	// if 'G', we don't specify one of the 
	// browse options.
	case 'F':
		lmqgmo_get_options.Options += MQGMO_BROWSE_FIRST;
		break;

	case 'N':
		lmqgmo_get_options.Options += MQGMO_BROWSE_NEXT;
		break;

	case 'U':	// Added 1/7/99
		lmqgmo_get_options.Options += MQGMO_MSG_UNDER_CURSOR;
		break;
	}


	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQGET
	//
	//  If No Messages are available, 
	//    lmql_reason_code = MQRC_NO_MSG_AVAILABLE
	//
	//  NOTE that we pass MQGET buffer_length - 1, so we have room
	//  to add a NULL at the end.  If we pass MQGET 40 and there are 40
	//  or more bytes of data, MQGET fills all 40 bytes with data.
	//-----------------------------------------------------------------+
	 GetA (vmqhc_connection, 
			&lmqmd_msg_desc, &lmqgmo_get_options, 
			vl_buffer_length - 1, vs_msg_buffer,
			&rl_data_length,  
			&lmql_comp_code, &lmql_reason_code,
			vb_xlate_key_values,	au_APIParms);


	//-----------------------------------------------------------------+
	//  Error State and Context Set by GetA
	//-----------------------------------------------------------------+





	//-----------------------------------------------------------------+
	//  TODO Set Warning here if Msg ID/Correl ID appear to be EBCDIC?
	//       or do we do this in GetA?
	//-----------------------------------------------------------------+





	//-----------------------------------------------------------------+
	//  Return Data to Caller if Get Was Good or if Caller will 
	//  Accept a Truncated Message.
	//-----------------------------------------------------------------+
	if ((lmql_comp_code   == MQCC_OK) ||
	   ((lmql_comp_code   == MQCC_WARNING) &&
		(lmql_reason_code == MQRC_TRUNCATED_MSG_ACCEPTED))) {


		if((lmql_comp_code   == MQCC_WARNING) &&
           (lmql_reason_code == MQRC_TRUNCATED_MSG_ACCEPTED)) {

			//-----------------------------------------------------+
			// Put NULL in last byte of buffer to NULL terminate
			// the data.
			//
			// Since MQGET was passed (vl_buffer_length - 1), we 
			// can put the terminating NULL in the last position 
			// and not overwrite any data.  
			//
			// Example:  A 100 byte buffer filled is filled with at 
			//           most 99 characters because MQGET was told 
			//           the buffer was only 99 bytes.
			//
			//           Data is in [0]...[98], NULL goes in [99].
			//-----------------------------------------------------+
			vs_msg_buffer[vl_buffer_length - 1] = NULL;

		}
		else {
			//-----------------------------------------------------+
			// Put NULL in byte after last byte of data to NULL 
			// terminate the data.
			//
			// Example:  A 100 byte buffer received a 75 byte msg.
			//
			//           Data is in [0]...[74], NULL goes in [75].
			//-----------------------------------------------------+
			vs_msg_buffer[rl_data_length] = NULL;
		}


		//---------------------------------------------------------+
		// Get Format from Message Descriptor
		// and put it caller's buffer
		//---------------------------------------------------------+
		memcpy(vs_format, lmqmd_msg_desc.Format, 
		       sizeof(lmqmd_msg_desc.Format));

		// caller should've allocated a 9 byte field
		// put terminating NULL in 9th position (#8 cause 0-8)
		vs_format[sizeof(lmqmd_msg_desc.Format)] = NULL;


		//-------------------------------------------------------------+
		// Return Remaining Values
		//-------------------------------------------------------------+
		rl_msg_type       = lmqmd_msg_desc.MsgType;

		rl_feedback_code  = lmqmd_msg_desc.Feedback;

		rl_report_options = lmqmd_msg_desc.Report;

		rl_expiration     = lmqmd_msg_desc.Expiry;

		rl_persistence    = lmqmd_msg_desc.Persistence;

		rl_priority       = lmqmd_msg_desc.Priority;

		memcpy(vs_msg_id,     lmqmd_msg_desc.MsgId, 
			                  MQ_MSG_ID_LENGTH);

		// caller should've allocated a 25 byte field
		// put terminating NULL in 25th position (#24 cause 0-24)
		vs_msg_id[MQ_MSG_ID_LENGTH] = NULL;


		memcpy(vs_correl_id,  lmqmd_msg_desc.CorrelId, 
			                  MQ_CORREL_ID_LENGTH);

		// caller should've allocated a 25 byte field
		// put terminating NULL in 25th position (#24 cause 0-24)
		vs_correl_id[MQ_CORREL_ID_LENGTH] = NULL;	// Add Term. NULL;


		memcpy(vs_reply_to_q, lmqmd_msg_desc.ReplyToQ, 
			                  MQ_Q_NAME_LENGTH);

		// caller should've allocated a 49 byte field
		// put terminating NULL in 49th position (#48 cause 0-48)
		vs_reply_to_q[MQ_Q_NAME_LENGTH] = NULL;

	}


	//-----------------------------------------------------------------+
	//  *** KO Override *** 
	//
	//  If Msg Format is not "MQSTR   " or "MQHSAP  ",
	//  set a warning if we don't have any other error.
	//
	//  Admin API is used for Dead Letter Processing.
	//-----------------------------------------------------------------+
	if ((lmql_comp_code   == MQCC_OK) ||
	   ((lmql_comp_code   == MQCC_WARNING) &&
		(lmql_reason_code == MQRC_TRUNCATED_MSG_ACCEPTED))) {

		if (strncmp(MQFMT_STRING, lmqmd_msg_desc.Format, 
		          sizeof(lmqmd_msg_desc.Format)) != 0 &&
			strncmp(KOFMT8_SAP, lmqmd_msg_desc.Format, 
		          sizeof(lmqmd_msg_desc.Format)) != 0 ) {
		
			lmql_comp_code   = MQCC_WARNING;
			lmql_reason_code = KORC_INVALID_MSG_FORMAT;

			//---------------------------------------------------------+
			//  Set Error State and Context
			//---------------------------------------------------------+
			iu_error.il_reason_code = lmql_reason_code;
			iu_error.il_comp_code   = lmql_comp_code;
			*iu_error.is_error_context = "Queue::Get";
		}
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
//  Args:     I  vmqhc_connection    - Queue Mgr Connection Handle
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
//  Dev Log:  TODO - Implement ASCII/EBCDIC Stuff
//
//  History: 
//  04/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::GetA(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  MQLONG  vl_buffer_length, 
				  char*   rs_msg_buffer,
				  PMQLONG rl_data_length,  
			      PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code,
				  BOOL    vb_xlate_key_values,
				  CAPIParmSet* au_APIParms) {


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	// Queue Manager Not Connected.
	iu_error.CheckError((vmqhc_connection	  != MQHC_UNUSABLE_HCONN), 
	 2, KORC_QMGR_NOT_CONNECTED, "CQueue::GetA.Parameter Validation");

	// Queue Not Open
	iu_error.CheckError((imqho_queue_handle != MQHO_UNUSABLE_HOBJ), 
	 2, KORC_INVALID_QID_NOT_OPEN, "CQueue::GetA.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		*rl_comp_code   = iu_error.il_comp_code;
	    *rl_reason_code = iu_error.il_reason_code;
		return iu_error.il_comp_code;
	}


	//-----------------------------------------------------------------+
	//  (Re)Set the First Msg indicator whenever NULL is provided for 
	//  the MsgId and the CorrlId.                          //DRC092199
	//-----------------------------------------------------------------+
	if ((&rmqmd_msg_desc->MsgId			==	NULL)
	&&  (&rmqmd_msg_desc->CorrelId		==	NULL)){
		au_APIParms->mb_BatchFirstMsg	=	True;
	}

#ifdef _prev_code_
	//-----------------------------------------------------------------+
	//  Set the Bit fields from caller provided value
	//-----------------------------------------------------------------+
	if (au_APIParms->mb_BatchFirstMsg) {
	   iu_CorrId->setBitFlds(au_APIParms->getBitFlds());
	}


	//-----------------------------------------------------------------+
	//  Translate Msg ID and/or Correl ID (if present) to NLSCharSet
	//  
	//  Implemented support for NLS Characterset             //DRC092199
	//-----------------------------------------------------------------+
	if (au_APIParms->mb_BatchContainsNLSKeys){
		iu_MsgId->encode(&rmqmd_msg_desc->MsgId);
		iu_CorrId->encode(&rmqmd_msg_desc->CorrelId);
	}


#else
	//-----------------------------------------------------------------+
	//  Translate Msg ID and/or Correl ID (if present) to NLSCharSet
	//  
	//  Implemented support for NLS Characterset             //DRC092199
	//-----------------------------------------------------------------+
	if (au_APIParms->mb_BatchContainsNLSKeys){
		iu_MsgId->encode(&rmqmd_msg_desc->MsgId);
		iu_CorrId->setBitFlds(au_APIParms->getBitFlds());
		iu_CorrId->encode(&rmqmd_msg_desc->CorrelId);
	}


#endif
	GetAExt(vmqhc_connection,	rmqmd_msg_desc, 
			rmqgmo_get_options,	vl_buffer_length, 
			rs_msg_buffer,		rl_data_length,  
			rl_comp_code,		rl_reason_code); 

	//-----------------------------------------------------------------+
	//  On_First_Record, If Msg_Not_Found, then cancel the encoding 
	//  and try the GET again.
	//-----------------------------------------------------------------+
	if ((au_APIParms->mb_BatchContainsNLSKeys)
	&&  (au_APIParms->mb_BatchFirstMsg)
	&&	(*rl_comp_code		!= MQCC_OK) 
	&&  (*rl_reason_code	== MQRC_NO_MSG_AVAILABLE)) {

		iu_MsgId->cancel(&rmqmd_msg_desc->MsgId);
		iu_CorrId->cancel(&rmqmd_msg_desc->CorrelId);

		GetAExt(vmqhc_connection,	rmqmd_msg_desc, 
			rmqgmo_get_options,	vl_buffer_length, 
			rs_msg_buffer,		rl_data_length,  
			rl_comp_code,		rl_reason_code); 
	}

	//-----------------------------------------------------------------+
	//  Translate Msg ID and/or Correl ID (if present) back to 
	//  the Host Character set.
	//  
	//  Implemented support for NLS Characterset             //DRC092199
	//-----------------------------------------------------------------+
	if (*rl_comp_code		== MQCC_OK){

		au_APIParms->mb_BatchContainsNLSKeys = False;
		au_APIParms->mb_BatchFirstMsg        = False;

		//-----------------------------------------------------------------+
		//  Determine the message format and decode as necessary. //DRC092199
		//-----------------------------------------------------------------+
		if (iu_CorrId->isNLSKey(&rmqmd_msg_desc->CorrelId)){
			au_APIParms->mb_BatchContainsNLSKeys = True;
			iu_MsgId->decode(&rmqmd_msg_desc->MsgId);
			iu_CorrId->decode(&rmqmd_msg_desc->CorrelId);

			//-------------------------------------------------------------+
			//  Provide the caller with:                        //DRC092199
			//  1)  Data Receipt Requested Indicator
			//  2)  Log  Receipt Requested Indicator
			//  3)  RestartKey field
			//-------------------------------------------------------------+
			au_APIParms->vb_data_receipt = iu_CorrId->isDataRcpt(); 
			au_APIParms->vb_log_receipt  = iu_CorrId->isLogRcpt();
			au_APIParms->setBitFlds(iu_CorrId->getBitFlds());
		}

		iu_MsgId->setOldKey();
		iu_CorrId->setOldKey();
	}

	return iu_error.il_comp_code;

}



//---------------------------------------------------------------------+
//  Method:   GetAExt
//  Author:   Daniel R Creager
//  Written:  September 1999
//
//  Desc:     MQ Series Get Function (Administrative Level) Extension

//  History: 
//  04/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::GetAExt(MQHCONN vmqhc_connection,
				  PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options,
				  MQLONG  vl_buffer_length, 
				  char*   rs_msg_buffer,
				  PMQLONG rl_data_length,  
			      PMQLONG rl_comp_code,
				  PMQLONG rl_reason_code) {

//-----------------------------------------------------------------+
//  Issue the MQI MQGET call 
//-----------------------------------------------------------------+
#ifndef ATHOME
MQGET (vmqhc_connection, imqho_queue_handle,
		rmqmd_msg_desc, rmqgmo_get_options, 
		vl_buffer_length, rs_msg_buffer,
		rl_data_length,  
		rl_comp_code, rl_reason_code);
#endif



//-----------------------------------------------------------------+
//  Set Error State and Context
//-----------------------------------------------------------------+
if (*rl_comp_code != MQCC_OK) {
	iu_error.il_reason_code = (long) *rl_reason_code;
	iu_error.il_comp_code   = (long) *rl_comp_code;
	*iu_error.is_error_context = "Queue::GetA";
}

return iu_error.il_comp_code;
}

//---------------------------------------------------------------------+
//  Method:   Close
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Closes a Queue.  Note there's no "Admin" version of this.
//            There's really no need for one.
//
//  Args:     I  vmqhc_connection - Queue Manager Connection Handle
//
//  Returns:  0 if okay, 1 if there's a warning, 2 if there's an error.
//  
//  Dev Log:  
//
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
long CQueue::Close( MQHCONN vmqhc_connection ) {


	MQLONG		lmql_comp_code   = 0L;
	MQLONG		lmql_reason_code = 0L;


	
	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Parameter Validation
	//-----------------------------------------------------------------+
	// Queue Manager Not Connected.
	iu_error.CheckError((vmqhc_connection	  != MQHC_UNUSABLE_HCONN), 
	 2, KORC_QMGR_NOT_CONNECTED, "CQueue::Close.Parameter Validation");

	// Queue Not Open
	iu_error.CheckError((imqho_queue_handle != MQHO_UNUSABLE_HOBJ), 
	 2, KORC_INVALID_QID_NOT_OPEN, "CQueue::Close.Parameter Validation");


	//-----------------------------------------------------------------+
	//  Return if we have a bad parameter
	//-----------------------------------------------------------------+
	if (iu_error.il_comp_code) {
		return iu_error.il_comp_code;
	}

	//-----------------------------------------------------------------+
	//  Call MQ Series API - MQCLOSE with the MQCO_NONE option
	//-----------------------------------------------------------------+
#ifndef ATHOME
	MQCLOSE(vmqhc_connection, &imqho_queue_handle, MQCO_NONE, 
			&lmql_comp_code, &lmql_reason_code);
#endif

	//-----------------------------------------------------------------+
	//  Set Error State and Context
	//-----------------------------------------------------------------+
	if (lmql_comp_code != MQCC_OK) {
		iu_error.il_reason_code = (long) lmql_reason_code;
		iu_error.il_comp_code   = (long) lmql_comp_code;
		*iu_error.is_error_context = "Queue::Close";
	}

	return 	iu_error.il_comp_code;

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
long CQueue::TermSelf( void ) {

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
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//  09/21/99 DRC Added instantiation of the MsgId and CorrlId objects
//               for National Language support.
//---------------------------------------------------------------------+
CQueue::CQueue( void ){

	imqho_queue_handle = MQHO_UNUSABLE_HOBJ;
	iu_MsgId  = new CKeyField();                           //DRC092199
    iu_CorrId = new CCorrelationField();                   //DRC092499
}



//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//  09/21/99 DRC Added removal of the MsgId and CorrlId objects.
//---------------------------------------------------------------------+
CQueue::~CQueue( void ){
	delete iu_MsgId;                                       //DRC092199
	delete iu_CorrId;                                      //DRC092499
}
