/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) QueMgr.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     QueMgr.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CQueMgr
//            Class.
//  
//            This class represents an MQ Series Queue Manager and
//            acts as client program "front-end" for the KO MQ Series
//            API.
//
//  Dev Log: 
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#include "QueMgr.h"






//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CQueMgr::CQueMgr() 
{
	m_lLastCompCode		= 0;
	m_lLastReasonCode	= 0;
	m_pcLog				= NULL;
	is_InstanceName		= "Queue Manager";

	ib_connected		= FALSE;
}






//---------------------------------------------------------------------+
//  Method:   CheckRC
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     private member method to check the return code of the
//            last KO MQ API call and store related error info
//
//  Args:     long	Return Code from last operation (This is the
//                  MQ Series Completion Code).
//
//  Returns:  nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
//
// private member method to check the return code of the
// last KOMQ API call and store pertenant error info.
//
inline void CQueMgr::CheckRC(long rc)
{
	if (0 != rc)
	{
		static const int CtxBuffLen = 1028;

		auto_ptr<char>	szErrCtx(new char[CtxBuffLen]);

	      //-----------------------------------------------------------+
	      //  Call Get Last Error in the API
	      //-----------------------------------------------------------+

		getlasterror(&m_lLastCompCode, &m_lLastReasonCode, 
			szErrCtx, CtxBuffLen-1);

		m_szLastErrContext = szErrCtx;
		
		//
		// Reuse the szErrCtx to get the reason code string.
		// Clean it out before using it to unsure a valid string
		// is returned with no chance for clutter.
		//
		memset(szErrCtx,'\0', CtxBuffLen);
	      //-----------------------------------------------------------+
	      //    Call Get Reason Code in the API
	      //-----------------------------------------------------------+
		getreasoncode(m_lLastReasonCode, szErrCtx, CtxBuffLen-1);

		m_szLastReasonStr = szErrCtx;
	}
}






//---------------------------------------------------------------------+
//  Method:   Open
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Connects to the Queue Manager
//
//  Args:     char*	Queue Manager Name
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/21/99 DMT Added call to CheckRC.  If Queue Manager failed to
//               open, no error information was reported.
//---------------------------------------------------------------------+
long CQueMgr::Open(const char* Name)
{
	long 	rc;
	CString	ls_msg;

	m_szName = (char*)Name;

	//-----------------------------------------------------------------+
	//  Call Init in the API
	//-----------------------------------------------------------------+
	rc = init(m_szName.c_str());

	CheckRC(rc);					// DMT 2/21/99
	
	if (0 != rc) {

		ls_msg << "Error Connecting to Queue Manager.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());

		//throw CExcBadQueMgr(*this, rc);
	}
	else {
		ib_connected = TRUE;
	}
	

	return rc;
}






//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Does Nothing
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  Should we have the Destructor Disconnect from the
//            Queue Manager??  For now, assuming NO because the API
//            will issue an error if we disconnected, and the 
//            destructor tries to disconnect an unconnected Queue
//            Manager.
//            
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CQueMgr::~CQueMgr()
{
}






//---------------------------------------------------------------------+
//  Method:   Get
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Gets Message from Queue.
//
//  Args:     --- See API Doc for details ---
//
//                int vi_queue_id,
//                char* vs_msg_buffer,
//                long  vl_buffer_length,
//                long& rl_data_length,   
//                long& rl_expiration,
//                BOOL  vb_batch_ind,     
//                BOOL  vb_accept_trunc,
//                long& rl_persistence,
//                char* vs_msg_id,
//                char* vs_correl_id,
//                char* vs_reply_to_q,
//                long& rl_msg_type,      
//                long& rl_feedback_code,
//                long& rl_report_options,
//                char* vs_format,
//                char  vc_get_mode,
//                long  vl_wait_interval, 
//                BOOL  vb_match_msg_id,   
//                BOOL  vb_match_correl_id,
//                long& rl_priority,
//                BOOL  vb_fail_if_quiescing,
//                BOOL  vb_xlate_key_values
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::Get( int vi_queue_id,
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
				BOOL  vb_xlate_key_values)
{

	CString	ls_msg;

	//-----------------------------------------------------------------+
	//  Call Get Standard Level in the API
	//-----------------------------------------------------------------+
	long rc =   get(  vi_queue_id,
                 vs_msg_buffer,    
				 vl_buffer_length,
                 rl_data_length,   
				 rl_expiration,
                 vb_batch_ind,     
				 vb_accept_trunc,
                 rl_persistence,
                 vs_msg_id,
                 vs_correl_id,
                 vs_reply_to_q,
                 rl_msg_type,      
				 rl_feedback_code,
                 rl_report_options,
                 vs_format,
                 vc_get_mode,
                 vl_wait_interval, 
                 vb_match_msg_id,   
				 vb_match_correl_id,
				 rl_priority,
				 vb_fail_if_quiescing,
				 vb_xlate_key_values);

	CheckRC(rc);

	switch(rc) {
	case 0:
		// return 0, not an error
		break;

	case 1:
		if (m_lLastReasonCode == MQRC_TRUNCATED_MSG_ACCEPTED) {
			// okay
		}
		else {
			rc = 2;
		}
		break;

	default:
		if (m_lLastReasonCode == MQRC_NO_MSG_AVAILABLE) {
			rc = 1;
		}
		break;
	} // end switch


	if (rc > 1) {

		ls_msg << "Error Getting Message from Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}

	return rc;
}









//---------------------------------------------------------------------+
//  Method:   GetA
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Maps Get Parameters to Administrative API, bypassing
//            std KO defaults and edits.
//
//  Args:     --- See API Doc for details ---
//
//            Parms into this method are identical to the Get Method.
//
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/04/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueMgr::GetA( int vi_queue_id,
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
				BOOL  vb_xlate_key_values)
{

	CString	ls_msg;
	BOOL    lb_map_back = FALSE;
	long    ll_return	= KOCC_GOOD;

	MQMD	lmqmd_msg_desc      = {MQMD_DEFAULT};	// message_descriptor
	MQGMO	lmqgmo_get_options  = {MQGMO_DEFAULT};	// get message options
	MQLONG	lmql_comp_code		= 0L;
	MQLONG	lmql_reason_code	= 0L;




	
	//-----------------------------------------------------------------+
	//  Populate Message Descriptor 
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
	ll_return = geta(vi_queue_id, 
			&lmqmd_msg_desc, &lmqgmo_get_options, 
			vl_buffer_length - 1, vs_msg_buffer,
			rl_data_length,  
			lmql_comp_code, lmql_reason_code,
			vb_xlate_key_values);



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


	CheckRC(ll_return);

	switch(ll_return) {
	case 0:
		// return 0, not an error
		break;

	case 1:
		if (m_lLastReasonCode == MQRC_TRUNCATED_MSG_ACCEPTED) {
			// okay
		}
		else {
			ll_return = 2;
		}
		break;

	default:
		if (m_lLastReasonCode == MQRC_NO_MSG_AVAILABLE) {
			ll_return = 1;
		}
		break;
	} // end switch


	if (ll_return > 1) {

		ls_msg << "Error Getting Message from Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}

	return ll_return;
}




//---------------------------------------------------------------------+
//  Method:   GetAttrLng
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Retrieves selected attributes from the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  Long attribute value.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long CQueMgr::GetAttrLng( int ai_Type){
return getAttrLng(ai_Type);
}






//---------------------------------------------------------------------+
//  Method:   GetAttrStr
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Retrieves selected attributes from the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  Pointer to the string attribute value.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
char* CQueMgr::GetAttrStr( int ai_Type){
return getAttrStr(ai_Type);
}






//---------------------------------------------------------------------+
//  Method:   setAttrLng
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Sets the value for a selected attributes in the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long  CQueMgr::SetAttrLng( int ai_Type, long al_Arg){
return setAttrLng(ai_Type, al_Arg);
}




//---------------------------------------------------------------------+
//  Method:   SetAttrStr
//  Author:   Daniel R Creager
//  Written:  January 2000
//
//  Desc:     Sets the value for a selected attributes in the API Parmset.
//
//  Args:     Type = The specific attribute to be retrieved
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_WARNING if there's no data,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/10/00  DRC Created original version.
//---------------------------------------------------------------------+
long  CQueMgr::SetAttrStr( int ai_Type, char* as_Arg){
return setAttrStr(ai_Type, as_Arg);
}






//---------------------------------------------------------------------+
//  Method:   Put
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Puts Message on Queue
//
//  Args:     --- See API Doc for details ---
//
//                    int  vi_queue_id,       char* vs_msg_buffer,
//                    long  vl_data_length,    long vl_expiration,
//                    BOOL  vb_batch_ind,      long vl_persistence,
//                    char* vs_msg_id,         char* vs_correl_id,
//                    char* vs_reply_to_q,     long vl_report_options,
//                    long  vl_msg_type,      long vl_feedback_code,
//                    char* vs_msg_format,    long vl_priority,
//                    BOOL  vb_fail_if_quiescing,
//                    BOOL  vb_xlate_key_values
//                    BOOL  vb_log_receipt    BOOL vb_data_receipt )
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  01/08/00 DRC Added Return Receipt parameters
//---------------------------------------------------------------------+
long CQueMgr::Put  (int vi_queue_id,       char* vs_msg_buffer,
                    long vl_data_length,    long vl_expiration,
                    BOOL vb_batch_ind,      long vl_persistence,
                    char* vs_msg_id,       char* vs_correl_id,
                    char* vs_reply_to_q,    long vl_report_options,
                    long  vl_msg_type,      long vl_feedback_code,
					char* vs_msg_format,    long vl_priority,
					BOOL  vb_fail_if_quiescing,
#ifdef _ver2_
					BOOL  vb_xlate_key_values,
					BOOL  vb_log_receipt,   BOOL vb_data_receipt)
#else
   					BOOL  vb_xlate_key_values)
#endif
{
	long 	rc;
	CString	ls_msg;

	//-----------------------------------------------------------------+
	//  Call Put Standard Layer in the API
	//-----------------------------------------------------------------+
	rc = put(	vi_queue_id,       vs_msg_buffer,
                vl_data_length,    vl_expiration,
                vb_batch_ind,      vl_persistence,
                vs_msg_id,         vs_correl_id,
                vs_reply_to_q,     vl_report_options,
                vl_msg_type,       vl_feedback_code,
				vs_msg_format,     vl_priority,
				vb_fail_if_quiescing,
#ifdef _ver2_
				vb_xlate_key_values,
				vb_log_receipt,    vb_data_receipt);
#else
				vb_xlate_key_values);
#endif
	CheckRC(rc);

	if (rc != 0) {

		ls_msg << "Error Putting Message Onto Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}
	return rc;
}









//---------------------------------------------------------------------+
//  Method:   PutA
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Maps Parameters onto MQ Series Structures and calls 
//            Administrative PUT functions which bypasses KO Edits and
//            defaults.
//
//  Args:     --- See API Doc for details ---
//
//                 ( int vi_queue_id,       char* vs_msg_buffer,
//                  long vl_data_length,    long vl_expiration,
//                  BOOL vb_batch_ind,      long vl_persistence,
//                  char* vs_msg_id,        char* vs_correl_id,
//                  char* vs_reply_to_q,    long vl_report_options,
//                  long  vl_msg_type,      long vl_feedback_code,
//					char* vs_msg_format,    long vl_priority,
//					BOOL  vb_fail_if_quiescing,
//                  BOOL  vb_xlate_key_values
//                  BOOL  vb_log_receipt    BOOL vb_data_receipt )
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/04/99  DMT Created original version.
//  01/08/00 DRC Added Return Receipt parameters
//---------------------------------------------------------------------+
long CQueMgr::PutA (int vi_queue_id,        char* vs_msg_buffer,
                    long vl_data_length,    long vl_expiration,
                    BOOL vb_batch_ind,      long vl_persistence,
                    char* vs_msg_id,        char* vs_correl_id,
                    char* vs_reply_to_q,    long vl_report_options,
                    long  vl_msg_type,      long vl_feedback_code,
					char* vs_msg_format,    long vl_priority,         
					BOOL  vb_fail_if_quiescing,
#ifdef _ver2_
				    BOOL vb_xlate_key_values,
				    BOOL vb_log_receipt,    
			        BOOL vb_data_receipt)
#else
				    BOOL vb_xlate_key_values)
#endif
{

	long 	ll_return			= KOCC_GOOD;
	CString	ls_msg;

	MQMD	lmqmd_msg_desc		= {MQMD_DEFAULT}; // message_descriptor
	MQPMO	lmqpmo_put_options  = {MQPMO_DEFAULT};// put message options
	MQLONG	lmql_comp_code		= 0L;
	MQLONG	lmql_reason_code	= 0L;



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
	//  Call Put Administrative Layer in the API
	//-----------------------------------------------------------------+
	ll_return = puta (vi_queue_id, &lmqmd_msg_desc, 
			&lmqpmo_put_options, vl_data_length, vs_msg_buffer, 
			lmql_comp_code, lmql_reason_code,
       #ifdef _ver2_
			vb_xlate_key_values, vb_log_receipt, vb_data_receipt);       
       #else
			vb_xlate_key_values);       
       #endif


	CheckRC(ll_return);

	if (ll_return != 0) {

		ls_msg << "Error Putting Message Onto Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}
	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   InitQ
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Opens a Queue
//
//  Args:     ref int   Queue ID
//                char* Queue Name
//                BOOL  TRUE to Open for Shared Input
//                BOOL  TRUE to Open for Exclusive Input
//                BOOL  TRUE to Open for Browse
//                BOOL  TRUE to Open for Output
//
//  Returns:  long	MQ Series Completion Code
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::InitQ( int& ri_qid, 
                        char* vs_q_name, 
                        BOOL vb_input_shared,
                        BOOL vb_input_excl, 
                        BOOL vb_browse, 
                        BOOL vb_output )
{
	long rc;
	CString	ls_msg;


	//-----------------------------------------------------------------+
	//  Call Init Queue in the API
	//-----------------------------------------------------------------+
	rc = initq( ri_qid, 
                vs_q_name, 
                vb_input_shared,
                vb_input_excl, 
                vb_browse, 
                vb_output );

	CheckRC(rc);

	if (rc != 0) {

		ls_msg << "Error Opening a Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}

	return rc;
}
	






//---------------------------------------------------------------------+
//  Method:   Commit
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Commits Queue Activity up to the last commit or
//            rollback.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::Commit()
{
	CString	ls_msg;

	long rc = commit();
	CheckRC(rc);

	if (0 != rc) {

		ls_msg << "Error Committing Queue Manager Activity.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			 ls_msg, 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());

		//throw CExcBadCommitOrRollback("Commit failed.", *this);
	}

	return rc;
}






//---------------------------------------------------------------------+
//  Method:   Rollback
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Rolls back Queue Activity up to the last commit or
//            rollback.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::Rollback()
{
	CString	ls_msg;
	long rc = rollback();
	CheckRC(rc);
	//if (0 != rc)
	//	throw CExcBadCommitOrRollback("Commit failed.", *this);

	if (rc != 0) {

		ls_msg << "Error Rolling Back Queue Manager Activity.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			"Error Rolling Back Queue Manager Activity.", 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());

	}

	return rc;
}






//---------------------------------------------------------------------+
//  Method:   Close
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Disconnects from an MQ Series Queue Manager
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::Close()
{
	CString	ls_msg;

	long rc = disconnect();
	CheckRC(rc);		// MUST CALL! this loads last error info.

	if (rc != 0) {

		if (m_pcLog != NULL) {

			ls_msg << "Error Disconnecting from Queue Manager.  "
				   << m_szLastReasonStr;

			m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
				 ls_msg, 
				 m_szLastErrContext.getPointer(),
				 is_InstanceName.getPointer());
		}

	}
	else {
		ib_connected = FALSE;
	}

	return rc;
}






//---------------------------------------------------------------------+
//  Method:   QClose
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Closes a Queue
//
//  Args:     int	Queue Handle
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CQueMgr::QClose(int iQueHandle)
{
	CString	ls_msg;

	long rc = komq_close(iQueHandle);
	CheckRC(rc);		// MUST CALL! this loads last error info.

	if (rc != 0) {

		ls_msg << "Error Closing Queue.  "
			   << m_szLastReasonStr;

		m_pcLog->LogError(m_lLastReasonCode, m_lLastCompCode,
			"Error Closing Queue.", 
			 m_szLastErrContext.getPointer(),
			 is_InstanceName.getPointer());
	}
	return rc;
}





//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Sets Queue Manager Name and interal Log Pointer.
//
//  Args:     ptr to CLog,
//            char* Queue Manager Name
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueMgr::initSelf(CLog* pLog, char* szName) {

	long ll_return	= KOCC_FAIL;


	if (CAny2::initSelf() == KOCC_GOOD) {
		m_pcLog   = pLog;
		ll_return = Open(szName);
	}

	return ll_return;

}





//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Disconnects from Queue Manager if it's still connnected.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/01/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueMgr::termSelf() {


	if (ib_connected) {
		return Close();
	}
	else {
		return KOCC_GOOD;
	}

}


//---------------------------------------------------------------------+
//  Method:   XlateReasonCode
//  Author:   David M. Tamkun
//  Written:  April 1999
//
//  Desc:     Translates the provided Reason Code into an 
//            Alphanumeric Reason Name.
//
//  Args:         long    Value to Translate
//            ref CString Receives Name
//
//  Returns:  KOCC_GOOD if successful
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  04/16/99  DMT Created original version.
//---------------------------------------------------------------------+
long CQueMgr::XlateReasonCode(long vl_value, CString& rs_Name) {

	long	ll_return		= KOCC_FAIL;
	long    ll_api_return	= 0;
	char	lpc_string[35];
	CString	ls_msg;

	lpc_string[34] = '\0';

	// getreasoncode returns -1 if an error occurs, 0 if okay,
	// or > 0 to indicate the size needed to hold length of
	// string plus 1 byte for a terminating NULL.
	//
	ll_api_return = getreasoncode(vl_value, lpc_string, 35);

	if (ll_api_return == 0) {

		ll_return = KOCC_GOOD;
		rs_Name = lpc_string;
	}
	else {
		ll_return = KOCC_FAIL;

		ls_msg << "Error Translating Reason Code "
			   << vl_value
			   << " to a Reason Name.  "
			   << "getreasoncode returned "
			   << ll_api_return
			   << ".";

		m_pcLog->LogError(KORC_UNSPECIFIED, ll_return,
						ls_msg,
						"CQueMgr::XlateReasonCode",
						is_InstanceName.getPointer());


	}
		

	return ll_return;

}


