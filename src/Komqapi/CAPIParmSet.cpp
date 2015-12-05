/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21	@(#) CAPIParmSet.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CAPIParmSet.cpp
//  Author:   Daniel R Creager
//  Written:  September 1999
//  Desc:     Implementation of the CAPIParmSet class. 
//
//
//  Dev Log: 
//
//  History: 
//  09/24/99  Created original code.
//  10/08/99  Incorporated the BitToChr and ChrToBit methods.
//  10/18/99  Added RestartKey attribute and the setRestartKey method.
//---------------------------------------------------------------------+
#ifndef _CAPIParmSet
#define _CAPIParmSet
#include "CAPIParmSet.h"
#endif

//---------------------------------------------------------------------+
//  Method:   setGet
//  Desc:     Store the API parameters for the GET API call.
//---------------------------------------------------------------------+
void CAPIParmSet::setGet(
					int		ai_queue_id,       
					char*	as_msg_buffer,
					long	al_buffer_length,
					long&	al_data_length,
					long&	al_expiration, 
					BOOL	ab_batch_ind,
					BOOL	ab_accept_trunc,
					long&	al_persistence,
					char*	as_msg_id,
					char*	as_correl_id,
					char*	as_reply_to_q,
					long&	al_msg_type,
					long&	al_feedback_code,
					long&	al_report_options,
					char*	as_format,
					char	ac_get_mode,
					long	al_wait_interval,
					BOOL	ab_match_msg_id,
					BOOL	ab_match_correl_id,
					long&	al_priority,
					BOOL	ab_fail_if_quiescing,
					BOOL	ab_xlate_key_values){

setCommonParmSet(ai_queue_id,		as_msg_buffer,
				 &al_data_length,	ab_xlate_key_values,
				 0,				0);

setStndParmSet(al_expiration,
			   ab_batch_ind,		al_persistence,
			   al_priority,			ab_fail_if_quiescing,
			   as_msg_id,			as_correl_id,
			   as_reply_to_q,		al_report_options,
			   al_msg_type,			al_feedback_code,
			   as_format);

vl_buffer_length	         	= al_buffer_length;
vb_accept_trunc			= ab_accept_trunc;
vc_get_mode				= ac_get_mode;
vl_wait_interval	      	= al_wait_interval;
vb_match_msg_id			= ab_match_msg_id;
vb_match_correl_id		= ab_match_correl_id;
vl_CCSID				= MQCCSI_Q_MGR;
vl_Encoding				= MQENC_NATIVE;
}

//---------------------------------------------------------------------+
//  Method:   setGetA
//  Desc:     Store the API parameters for the GETA API call.
//---------------------------------------------------------------------+
void CAPIParmSet::setGetA(
					int		ai_queue_id,
					PMQMD	amqmd_msg_desc,
					PMQGMO	amqgmo_get_options,
					long	al_buffer_length,
					char*	as_msg_buffer,
					long&	al_data_length,
					long&	al_comp_code,
					long&	al_reason_code,
					BOOL	ab_xlate_key_values){

setCommonParmSet(ai_queue_id,		as_msg_buffer,
				 &al_data_length,	ab_xlate_key_values,
				 0,				0);

setAdvParmSet(amqmd_msg_desc,	al_comp_code,	al_reason_code);

rmqgmo_get_options	= amqgmo_get_options;
vl_buffer_length	= al_buffer_length;
}


//---------------------------------------------------------------------+
//  Method:   setPut
//  Desc:     Store the API parameters for the PUT API call.
//---------------------------------------------------------------------+
void CAPIParmSet::setPut(
					int		ai_queue_id,
					char*	as_msg_buffer,
					long	al_data_length,
					long	al_expiration,
					BOOL	ab_batch_ind,
					long	al_persistence,
					char*	as_msg_id,
					char*	as_correl_id,
					char*	as_reply_to_q,
					long	al_report_options,
					long	al_msg_type,
					long	al_feedback_code,
					char*	as_msg_format,
					long	al_priority,
					BOOL	ab_fail_if_quiescing,
					BOOL	ab_xlate_key_values,
					BOOL	ab_log_receipt,
					BOOL	ab_data_receipt){

setCommonParmSet(ai_queue_id,		as_msg_buffer,
				 &al_data_length,	ab_xlate_key_values,
				 ab_log_receipt,	ab_data_receipt);

setStndParmSet(al_expiration,
			   ab_batch_ind,		al_persistence,
			   al_priority,			ab_fail_if_quiescing,
			   as_msg_id,			as_correl_id,
			   as_reply_to_q,		al_report_options,
			   al_msg_type,			al_feedback_code,
			   as_msg_format);
}


//---------------------------------------------------------------------+
//  Method:   setPutA
//  Desc:     Store the API parameters for the PUTA API call.
//---------------------------------------------------------------------+
void CAPIParmSet::setPutA(
					int		ai_queue_id,
					PMQMD	amqmd_msg_desc,
					PMQPMO	amqpmo_put_options,
					long	al_data_length,
					char*	as_msg_buffer,
					long&	al_comp_code,
					long&	al_reason_code,
					BOOL	ab_xlate_key_values,
					BOOL	ab_log_receipt,
					BOOL	ab_data_receipt){

setCommonParmSet(ai_queue_id,		as_msg_buffer,
				 &al_data_length,	ab_xlate_key_values,
				 ab_log_receipt,	ab_data_receipt);

setAdvParmSet(amqmd_msg_desc,	al_comp_code,	al_reason_code);

rmqpmo_put_options	= amqpmo_put_options;
}


//---------------------------------------------------------------------+
//  Method:   setCommonParmSet
//  Desc:     Store the API parameters common to all other calls.
//---------------------------------------------------------------------+
void CAPIParmSet::setCommonParmSet(
					int		ai_queue_id,
					char*	as_msg_buffer,
					long*	al_data_length,
					BOOL    ab_xlate_key_values,
					BOOL    ab_log_receipt,
					BOOL    ab_data_receipt){


vi_queue_id					= ai_queue_id;
vs_msg_buffer				= as_msg_buffer;
rl_data_length				= al_data_length;
vb_xlate_key_values			= ab_xlate_key_values;
vb_data_receipt               	= ab_data_receipt;
vb_log_receipt                	= ab_log_receipt;
}


//---------------------------------------------------------------------+
//  Method:   setStndParmSet
//  Desc:     Store the API parameters common to all standard calls.
//---------------------------------------------------------------------+
void CAPIParmSet::setStndParmSet(
					long&	al_expiration,
					BOOL	ab_batch_ind,
					long&	al_persistence,
					long&	al_priority,
					BOOL	ab_fail_if_quiescing,
					char*	as_msg_id,
					char*	as_correl_id,
					char*	as_reply_to_q,
					long&	al_report_options,
					long&	al_msg_type,
					long&	al_feedback_code,
					char*	as_format){
	
rl_expiration			= &al_expiration;
vb_batch_ind			= ab_batch_ind;
rl_persistence			= &al_persistence;
rl_priority				= &al_priority;
vb_fail_if_quiescing	= ab_fail_if_quiescing;
vs_msg_id				= as_msg_id;
vs_correl_id			= as_correl_id;
vs_reply_to_q			= as_reply_to_q;
rl_report_options		= &al_report_options;
rl_msg_type				= &al_msg_type;
rl_feedback_code		= &al_feedback_code;
vs_format				= as_format;
}


//---------------------------------------------------------------------+
//  Method:   setAdvParmSet
//  Desc:     Store the API parameters common to all advanced calls.
//---------------------------------------------------------------------+
void CAPIParmSet::setAdvParmSet(
					PMQMD   amqmd_msg_desc,
					long&   al_comp_code,
					long&   al_reason_code){
rmqmd_msg_desc		= amqmd_msg_desc;
rl_comp_code		= &al_comp_code;
rl_reason_code		= &al_reason_code;
}


//---------------------------------------------------------------------+
//  Method:   getRestartKey
//  Desc:     Translate the BitFields into an external RestartKey
//            and return it to the caller.
//  History: 
//  10/18/99  DRC Created original code. 
//---------------------------------------------------------------------+
CString* CAPIParmSet::getRestartKey(){
BitToChr(ms_RestartKey->getPointer(), ms_BitFields->getPointer());
return ms_RestartKey;
}


//---------------------------------------------------------------------+
//  Method:   setRestartKey
//  Desc:     Save the current BitField settings. 
//  History: 
//  10/18/99  DRC Created original code. 
//---------------------------------------------------------------------+
void CAPIParmSet::setRestartKey(char* as_Arg1){
*ms_RestartKey = as_Arg1;
ChrToBit(ms_BitFields->getPointer(), ms_RestartKey->getPointer());
}


//---------------------------------------------------------------------+
//  Method:   getBitFlds
//  Desc:     Translate the RestartKey into an internal BitField
//            and return it to the caller.
//  History: 
//  10/18/99  DRC Created original code. 
//---------------------------------------------------------------------+
CString* CAPIParmSet::getBitFlds(void){
return ms_BitFields;
}

	
//---------------------------------------------------------------------+
//  Method:   setBitFlds
//  Desc:     Save the current BitField settings. 
//  History: 
//  10/18/99  DRC Created original code. 
//---------------------------------------------------------------------+
void CAPIParmSet::setBitFlds(CString*	as_Arg1){
*ms_BitFields = as_Arg1;
}


//---------------------------------------------------------------------+
//  Method:   BitToChr
//  Desc:     Convert a BitString into a character encoded hexadecimal
//            string.  The input is assumed to be a six byte bitString
//            and the output a 12 byte string buffer. 
//  Dev Log: 
//  01)  It may make more sense to incorporate these methods into 
//       either CString or a specialization of CString, perhaps 
//       a CBitString class. 
//
//  History: 
//  10/11/99  DRC Created original code. 
//---------------------------------------------------------------------+
void CAPIParmSet::BitToChr(char* as_To, char* as_From){
//
//  declare local variables
//
int i, j;

//
//  For each byte in the string buffer.
//
for (i = 0; i < 12; i += 2){
	i > 1 ? j = i / 2 : j = i;
	//
	//  Isolate and translate the left-side nibble 
	//
	as_To[i+0] = as_From[j] >> 4 & RhtSide;
	as_To[i+0] = ms_XlateRef->getPointer()[as_To[i+0]];

	//
	//  Isolate and translate the right-side nibble 
	//
	as_To[i+1] = as_From[j] & RhtSide;
	as_To[i+1] = ms_XlateRef->getPointer()[as_To[i+1]];
} /* endFor */

} /* End of BitToChr Method */


//---------------------------------------------------------------------+
//  Method:   ChrToBit
//  Desc:     Convert character encoded hexadecimal string into an 
//            actual BitString.  The input is assumed to be a 12 byte 
//            string buffer and the output a six byte bitString. 
//  Dev Log: 
//  01)  It may make more sense to incorporate these methods into 
//       either CString or a specialization of CString, perhaps 
//       a CBitString class. 
//
//  History: 
//  10/11/99  DRC Created original code. 
//---------------------------------------------------------------------+
void CAPIParmSet::ChrToBit(char* as_To, char* as_From){
//
//  declare local variables
//
int	i, j;
char	ls_To;

//
//  For each byte in the string buffer.
//
for (i = 0; i < 6; i++){
	i > 0 ? j = i * 2 : j = i;
	//
      //  Convert the first byte
	//  1)  translate the first byte from chr to binary 
      //  2)  adjust for non-numeric digits
	//  3)  switch base of the first digit by multiplying by 16
	//
          as_To[i] = as_From[j+0] & ~Bits_2_3;
	    if (as_To[i] & Bit_1)
		 as_To[i] = (as_To[i] & ~Bit_1) + 9;
          as_To[i] <<= 4;

	//
      //  Convert the second byte
	//  1)  translate the first byte from chr to binary 
      //  2)  adjust for non-numeric digits
	//
	    ls_To = as_From[j+1] & ~Bits_2_3;
	    if (ls_To & Bit_1) 
             ls_To = (ls_To & ~Bit_1) + 9;

	//
	//  combine the first and second byte values
	//
	as_To[i] |= ls_To;
} /* endFor */

} /* End of BitToChr Method */


//---------------------------------------------------------------------+
//  Method:   Constuctor
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CAPIParmSet::CAPIParmSet(){
mb_BatchContainsNLSKeys = True;
mb_BatchFirstMsg = True;
ms_XlateRef = new CString("0123456789ABCDEF");
ms_RestartKey = new CString("000000000000");
ms_BitFields  = new CString("\0\0\0\0\0\0");
}

//---------------------------------------------------------------------+
//  Method:   Destructor
//  Desc:     TBD. 
//---------------------------------------------------------------------+
CAPIParmSet::~CAPIParmSet(void){
delete ms_XlateRef;
delete ms_RestartKey;
delete ms_BitFields;
}
