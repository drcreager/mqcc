/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/09/21	@(#) CRespBatch.C 1.0@(#)
 */
//---------------------------------------------------------------------+
//  File:     CRespBatch.cpp
//  Author:   Daniel R Creager
//  Written:  January 2000
//  Desc:     Response Batch Object of the MQSeries Common Code.
//
//            Encapsulates the knowledge of how we interface with the 
//            Reply-To queue to return status and receipt information 
//            to senders. This object will establish the Correlation Id
//            to synchronize a batch containing data from multiple callers. 
//
//  TechNote: Specialized setup requirements exist for this class due 
//            to the use of cross-aggregation.  Refer to the documentation
//            in RespBatch.h for more details.
//
//  History: 
//  12/06/99  Created original code in Cobol.
//  01/20/00  Ported code to C++
//  09/20/00  Eliminated Log ref on CBuffer.initSelf()
//---------------------------------------------------------------------+
// 
#include "RespBatch.h"


//---------------------------------------------------------------------+
//  Method:   checkParms
//  Desc:     Make sure we have valid parameters.
//---------------------------------------------------------------------+
long CRespBatch::checkParms(CString* as_MsgId, CString* as_BatchId){

long ll_return = KOCC_GOOD;

//
//  Validate that all required data is present
//
if (m_lUseCount           <= 0) ll_return = KOCC_FAIL;
if (m_sQMgr_Name.vlen()   <= 0) ll_return = KOCC_FAIL;
if (m_sQueue_Name.vlen()  <= 0) ll_return = KOCC_FAIL;
if (as_MsgId->vlen()      <= 0) ll_return = KOCC_FAIL;
if (as_BatchId->vlen()    <= 0) ll_return = KOCC_FAIL;

return ll_return;
}	/* End Method */ 







//---------------------------------------------------------------------+
//  Method:   clear
//  Desc:     Clear the Data Cache
//---------------------------------------------------------------------+
long CRespBatch::clear(void) {
	m_pcCache->clear();
	return 0;
} /* End Method */ 







//---------------------------------------------------------------------+
//  Method:   close
//  Desc:     Release the Reply-To Queue
//---------------------------------------------------------------------+
long CRespBatch::close(void){
	m_lUseCount--;
	return 0;
}	/* End Method */ 








//---------------------------------------------------------------------+
//  Method:   CRespBatch (Constructor)
//  Desc:     Initialize this object. 
//---------------------------------------------------------------------+
CRespBatch::CRespBatch(CLog* a_pcLog, CString*	as_QMgr_Name){

is_InstanceName = "Return Receipt Business Logic Layer";	
initSelf(a_pcLog, as_QMgr_Name);
}	/* End Method */ 







//---------------------------------------------------------------------+
//  Method:   ~CRespBatch (Destructor)
//  Desc:     Release this object. 
//---------------------------------------------------------------------+
CRespBatch::~CRespBatch(void){
termSelf();
}	/* End Method */ 






//---------------------------------------------------------------------+
//  Method:   InitSelf
//  Desc:     Initialize the Queue Manager
//---------------------------------------------------------------------+
long CRespBatch::initSelf(CLog* a_pcLog, CString*	as_QMgr_Name){

	long	ll_return = KOCC_GOOD;
	m_lUseCount       = 0L;
	m_pcLog           = a_pcLog;
	m_sQMgr_Name      = *as_QMgr_Name;

	ll_return = CAny2::initSelf();

	//  Initialize the Cache
	m_pcCache = new CBufferList(a_pcLog);

	return ll_return;
}	/* End Method */ 









//---------------------------------------------------------------------+
//  Method:   Open
//  Desc:     Open the Reply-To Queue
//---------------------------------------------------------------------+
long CRespBatch::open(CString* as_Queue_Name){
	m_lUseCount++;
	m_sQueue_Name = as_Queue_Name;
	m_cQue.SetReplyToQueueName(as_Queue_Name->getPointer());
	return 0;
}	/* End Method */ 






//---------------------------------------------------------------------+
//  Method:   put
//  Desc:     Put the cache contents into the Reply-To Queue
//---------------------------------------------------------------------+
long CRespBatch::put(CString* as_MsgId, CString* as_BatchId){

long ll_return = KOCC_GOOD;

//
// On ValidParameters, Write the data cache into the reply-to queue
//
ll_return = checkParms(as_MsgId, as_BatchId);
if (ll_return == KOCC_GOOD) {
    ll_return = writeCache(as_MsgId, as_BatchId);
} /* End If */

return ll_return;

} /* End Method */ 








//---------------------------------------------------------------------+
//  Method:   set
//  Desc:     Store this response data in the data cache
//---------------------------------------------------------------------+
long CRespBatch::set(CBuffer* as_Arg){
//
//  Add an element into the data cache
// 
return m_pcCache->push(as_Arg);
} /* End Method */ 







long CRespBatch::set(char* as_Arg){
CBuffer l_cBuffer;

l_cBuffer.initSelf(strlen(as_Arg));
l_cBuffer = as_Arg; 
return set(&l_cBuffer);
} /* End Method */ 







//---------------------------------------------------------------------+
//  Method:   termSelf
//  Desc:     Disconnect from the Queue Manager
//---------------------------------------------------------------------+
long CRespBatch::termSelf(void){
if (m_lUseCount <= 0){
     delete m_pcCache;
	 m_sQMgr_Name = "";
} /* End If */
return 0;
} /* End Method */ 








//---------------------------------------------------------------------+
//  Method:   writeCache
//  Desc:     Output Queue Data
//---------------------------------------------------------------------+
long CRespBatch::writeCache(CString* as_MsgId, CString* as_BatchId){

//
//  Declare local variables
//
long ll_return = KOCC_GOOD;

//-----------------------------------------------------------------+
// Open Queue Manger
//-----------------------------------------------------------------+
ll_return = m_cQueMgr.initSelf(m_pcLog, m_sQMgr_Name.getPointer());
if (ll_return != KOCC_GOOD) {
	iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, 
	"Unable to connect to the queue manager",
	"CRespBatch::writeCache:1",
	is_InstanceName.getPointer());
}  /* End If */
 

//-----------------------------------------------------------------+
// Initialize the Queue class
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
    ll_return = m_cQue.initSelf(m_pcLog, &m_cQueMgr,  
                m_sQueue_Name.getPointer(),
                True, True, False, False);
	if (ll_return != KOCC_GOOD) {
		iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, 
		"Unable to initialize the queue class",
		"CRespBatch::writeCache:2",
		is_InstanceName.getPointer());
	}   /* End If */
}       /* End If */ 

//-----------------------------------------------------------------+
// Open the Reply-To Queue
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
    ll_return = m_cQue.QOpen(m_sQueue_Name.getPointer(),
		                     CQue::Output);
	if (ll_return != KOCC_GOOD) {
		iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, 
		"Unable to open a ReplyTo queue",
		"CRespBatch::writeCache:3",
		is_InstanceName.getPointer());
	}   /* End If */
}       /* End If */ 

//-----------------------------------------------------------------+
// Allocate Message Buffer for Sending/Receiving Messages
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
	ll_return = m_cBuffer.initSelf(KO_RR_BUFFER_SIZE);
	if (ll_return != KOCC_GOOD) {
		iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, 
		"Unable to allocate an I/O buffer",
		"CRespBatch::writeCache:4",
		is_InstanceName.getPointer());
	}   /* End If */
}       /* End If */



//-----------------------------------------------------------------+
// Set the Queue/Msg Attributes and Reposition the Cache Cursor
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
    m_cQue.SetMsgID(as_MsgId->getPointer());
	m_cQue.SetCorlID(0L);
	m_cQue.SetMsgType(KOMT_DATA);
	m_cQue.SetExpiration(KO_SEVEN_DAY_EXPIRATION);
	m_cQue.SetPriority(KO_RR_PRIORITY);
	m_cQue.SetSyncpointEnable(False);
	m_cQue.SetMsgFormat(KOFMT_STRING);
	m_cQue.SetPersistence(KO_DEFAULT_PERSISTENCE);

}   /* End If */ 


//-----------------------------------------------------------------+
// Pull each element from the data cache and write it to the queue
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
    m_pcCache->top(&m_cBuffer);

    while ((!m_pcCache->endOfList())
	   &&  (ll_return == KOCC_GOOD)){
		//
		//  read and delete the next message
		//
		m_pcCache->pop(&m_cBuffer);
		//

		//  Increment the Correlation Id and write the msg 
		//
	    m_cQue.SetCorlID(m_cQue.GetCorlIDBin() + 1);
		ll_return = m_cQue.Put(m_cBuffer);

		//
		//  On error, write out a diagnostic message
		//
		if (ll_return != KOCC_GOOD){
			iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, 
				"Unable to PUT a Return Receipt into a ReplyTo queue",
				"CRespBatch::writeCache:5",
				is_InstanceName.getPointer());
		}   /* End If    */

	    
    }   /* End While */

}   /* End If    */ 

//-----------------------------------------------------------------+
// Release the Message Buffer
//-----------------------------------------------------------------+
ll_return = m_cBuffer.termSelf();


//-----------------------------------------------------------------+
// Close the Reply-To Queue
//-----------------------------------------------------------------+
ll_return = m_cQue.termSelf();
 

//-----------------------------------------------------------------+
// Disconnect from the Queue Manager
//-----------------------------------------------------------------+
ll_return = m_cQueMgr.termSelf(); 
return 0;
} /* End Method */ 
