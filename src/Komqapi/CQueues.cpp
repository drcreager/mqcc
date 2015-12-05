/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/07	@(#) CQueues.C 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CQueues.cpp
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     This file contains the implementation of the CQueues
//            Class.
//  
//  Dev Log: 
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT changed code to handle increase of Queue Array from
//               2 to 50 elements.
//---------------------------------------------------------------------+


#include "CQueues.h"







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
long CQueues::Init (){

return 0;

}



//---------------------------------------------------------------------+
//  Method:   AddQueue
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Adds a new Queue to the Collection
//
//  Args:     Output - Pointer to Queue Class
//            Output - Int - "Handle" to Queue class which can be
//                           resolved back into a Queue Class pointer
//                           later.
//
//  Returns:  0   if successful
//            > 0 if an error occurs.
//  
//  Dev Log: 
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT Changed code from nested if to for loop to handle
//               increased number of queues.
//---------------------------------------------------------------------+
long CQueues::AddQueue(CQueue*& ru_queue, int& ri_qid) {

	int	li_counter = 0;


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();


	//-----------------------------------------------------------------+
	//  Loop through Queue Array, looking for first NULL (and therefore
	//  free) class pointer.
	//-----------------------------------------------------------------+
	for (li_counter = 1; li_counter <= KO_MAX_QUEUES; li_counter++) {

		if(iu_queues[li_counter] == NULL) {

			try {
				iu_queues[li_counter] = new CQueue;
			}

			catch (...) {
				iu_queues[li_counter] = NULL;
			}

			if (iu_queues[li_counter] != NULL) {

				ru_queue = iu_queues[li_counter];
				ri_qid = li_counter;

				// Break out of for loop. 'cause we found free slot.
				break;
			}
			else {
				// error allocating memory HANDLE!!
				iu_error.il_comp_code      = 2;
				iu_error.il_reason_code    = KORC_NEW_CQUEUE_FAIL;
				*iu_error.is_error_context = "Queues::AddQueue";
			}
		}

	}  // end for


	if(li_counter > KO_MAX_QUEUES) {
		// All "slots" Full cannot allocate more queues
		iu_error.il_comp_code      = 2;
		iu_error.il_reason_code    = KORC_MAX_QUEUES_EXCEEDED;
		*iu_error.is_error_context = "Queues::AddQueue - NoQueuesFree";

	}
	return iu_error.il_comp_code;
}






//---------------------------------------------------------------------+
//  Method:   GetQueue
//  Author:   David M Tamkun
//  Written:  November 1998
//
//  Desc:     Gets Pointer of Requested Queue from the Collection.
//            uses the passed in Queue Handle as an array index
//            to get the Pointer to a Queue Class out of the 
//            Queue Class Array.
//
//            Note valid Queue IDs are 1 to KO_MAX_QUEUES.
//
//            Array has KO_MAX_QUEUES + 1 elements and we never
//            reference element 0.
//
//  Args:     Output - Pointer to Queue Class
//            Input  - Queue "Handle"
//
//  Returns:  0 if okay
//            1 if queue is closed
//            2 if queue_id is not valid.
//  
//  Dev Log: 
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT Changed code to handle array of up to 50 elements.
//---------------------------------------------------------------------+
long CQueues::GetQueue(CQueue*& ru_queue, int vi_qid) {


	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();



	if(vi_qid < 1 || vi_qid > KO_MAX_QUEUES){
		// invalid queue id

		iu_error.il_comp_code      = 2;
		iu_error.il_reason_code    = KORC_INVALID_QID;
		*iu_error.is_error_context = "Queues::GetQueue - Invalid QueueID";

	}
	else {
		ru_queue = iu_queues[vi_qid];

		if (iu_queues[vi_qid] == NULL) {
			// set warning

			iu_error.il_comp_code      = 1;
			iu_error.il_reason_code    = KORC_INVALID_QID_NOT_OPEN;
			*iu_error.is_error_context = "Queues::GetQueue - Queue Not Open";

		}
	}

	return iu_error.il_comp_code;
}





//---------------------------------------------------------------------+
//  Method:   RemoveQueue
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Removes Specified Queue from the Collection
//             
//            NOTE!!  Assumes CQueueMgr is closing queue before this
//                    request to remove it.
//
//			  It is also possible the Open failed, so we'd need to
//            free up the Queue Class.
//
//  Args:     TBD
//  Returns:  0 if Queue is removed
//            1 if Queue was not there.
//            2 if Queue ID is invalid.
//  
//  Dev Log: 
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT Changed code to handle array of up to 50 elements.
//---------------------------------------------------------------------+
long CQueues::RemoveQueue(int vi_qid) {

	//-----------------------------------------------------------------+
	//  Reset Error Codes
	//-----------------------------------------------------------------+
	iu_error.ClearErrorCodes();

	if(vi_qid < 1 && vi_qid > KO_MAX_QUEUES){
		// invalid queue id

		iu_error.il_comp_code      = 2;
		iu_error.il_reason_code    = KORC_INVALID_QID;
		*iu_error.is_error_context = "Queues::RemoveQueue - Invalid QueueID";

	}
	else {
		if (iu_queues[vi_qid] == NULL) {
			// set warning

			iu_error.il_comp_code      = 1;
			iu_error.il_reason_code    = KORC_INVALID_QID_NOT_OPEN;
			*iu_error.is_error_context = "Queues::RemoveQueue - Queue Not Open";
		}
		else {
			delete iu_queues[vi_qid];
			iu_queues[vi_qid] = NULL;
		}
	}

	return iu_error.il_comp_code;
}




//---------------------------------------------------------------------+
//  Method:   RemoveAll
//  Author:   David M Tamkun
//  Written:  November 1998
//  Desc:     Removes All Queues from the Collection
//             
//            NOTE!!  Assumes caller has already "closed" the queues.
//
//  Args:     TBD
//  Returns:  0
//  
//  Dev Log: 
//  History: 
//  11/11/98 DMT Created original version.
//  12/09/98 DMT Changed code to loop through Queue Array.
//---------------------------------------------------------------------+
long CQueues::RemoveAll( void ) {

	int	li_counter = 0;

	for (li_counter = 1; li_counter <= KO_MAX_QUEUES; li_counter++) {

		RemoveQueue(li_counter);

	} // end for

	return 0;
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
long CQueues::TermSelf( void ) {

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
//  12/09/98 DMT Changed code to loop through Queue Array.
//---------------------------------------------------------------------+
CQueues::CQueues( void ){

	int	li_counter = 0;

	for (li_counter = 0; li_counter <= KO_MAX_QUEUES; li_counter++) {
		iu_queues[li_counter] = NULL;
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
//  Dev Log: 
//  History: 
//  11/07/98 DMT Created original version.
//---------------------------------------------------------------------+
CQueues::~CQueues( void ){

	RemoveAll();

}
