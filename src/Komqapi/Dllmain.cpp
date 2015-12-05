/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/11	@(#) DllMain.C 1.3@(#)
 */
//---------------------------------------------------------------------+
//  File:     DllMain.cpp
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     This Dynamic Link Library contains a KO "Wrapper" to the
//            MQ Series API.
//
//  History: 
//  11/07/98  DMT Created original code.
//  12/04/98  DMT Changed getlasterror comp code and reason code
//                parameters from long& to long* to try to get 
//                VB calls to work.
//  12/09/98  DMT Changed PUT call to add Report Option, Message Type,
//                and Feedback Code.  Added GET Call too.
//  12/21/98  CAH Ported to UNIX, added local code to support Thread
//                Local Storage (Tls)
//  01/07/99  DMT Added "get2" call to support our DeadLetterHeader
//                program.
//  01/12/99  DMT Renamed "close" function to "komq_close" to avoid 
//                collision with "close" function on file streams.
//  09/24/99  DRC Added CAPIParmSet to the Thread Object (lu_tObj).
//  10/18/99  DRC Added get|set AttrXXX methods.
//---------------------------------------------------------------------+
#include "dllmain.h"

#ifndef _WIN32
#include <stdlib.h>

//---------------------------------------------------------------------+
//
// The TlsSlot and TlsStore classes are used as a replacement for the
// MS Windows (T)hread (L)ocal (S)torage available via the Win32 API.
// The TLS API provides thread safe collection of pointers for use in
// creating storage that is used in the context of a single thread when
// running in a multithreaded environment (that is one or more threads).
//---------------------------------------------------------------------+
//
// The TlsSlot is the basic data structure that contains the information
// necessary to associate a unique thread with it's TLS pointer.
//
//---------------------------------------------------------------------+
class TlsSlot
{
private:
	pthread_t	m_tid;
	void *		m_lpPtr;
	TlsSlot(TlsSlot&);
public:
	TlsSlot();
	void	  SetTID(pthread_t);	// Set the thread ID for a TlsSlot
	pthread_t	  GetTID();			// Get the thread ID for this TlsSlot
	void	  SetPtr(void*);		// Set the pointer to the TLS storage defined by the app.
	void*	  GetPtr();				// Get the pointer
};
//---------------------------------------------------------------------+
//
// The TlsStore aggregates TlsSlots in an pointer array. There is a TlsSlot 
// allocated for each thread that uses TLS storage and a pointer to that 
// TlsSlot is stored in the TlsStore. The TlsStore is constructed with an 
// empty array. When the application wants to use TLS, the Alloc method gets
// an available TlsSlot from the array, using m_iHighestIndex member variable 
// as the upper bound of the array. If the number in m_iHighestIndex is 
// attained, the TlsStore allocates a new array that is 
// m_iHighexIndex + m_iResizeChunck in size. It then copies the existing 
// TlsSlot object pointers to the new array.
//
// The TlsStore uses a POSIX pthread_mutex_t to insure thread saftey.
// Only one thread is allowed to access the TlsStore at a time. 
// Using standard mutex logic, when the first thread enters the TlsStore,
// it requests and is granted ownership of the mutex and continues to execute
// the TlsStore access methods. When another concurrent thread tries to gain
// ownership of the mutex, it is blocked and must wait for the thread executing
// code in the TlsStore to release the mutex. Once the running thread has 
// completed its task and releases the mutex, the blocked thread wakes,
// given ownership of the mutex and allowed to execute a TlsStore 
// method.
//
// Note: Based on the #ifndef _WIN32 conditional, the TlsStore and TlsSlot
// classes are only built on Non MS Windows environment.
//
// Finally, the TlsSlot and TlsStore classes are used to implement a
// standard C interface that emulates the Win32 TLS API.
//
// The TlsStore is based on a "Singleton" pattern. This means that there is
// single TlsStore per process.
//
//---------------------------------------------------------------------+

class TlsStore
{
private:
	static TlsStore* m_this;
	static const int m_iResizeChunck;
	static pthread_mutex_t m_mLock;
	static int m_iIndex;
	static int m_iHighestIndex;
    static TlsSlot* m_lpTlsPtrArry;
	TlsStore(TlsStore&) {};
	TlsSlot* FindTls();
public:
	TlsStore();
	~TlsStore();
	void Lock();
	void Unlock();
	DWORD Alloc();
	BOOL  Free(DWORD index);
	void* GetValue(DWORD);
	BOOL  SetValue(DWORD, void*);
	static TlsStore& Instance();
	
};
//---------------------------------------------------------------------+
// Get the thread ID for this TlsSlot
//---------------------------------------------------------------------+
pthread_t TlsSlot::GetTID()
{
	return m_tid;
}
//---------------------------------------------------------------------+
// Set the thread ID for this TlsSlot
//---------------------------------------------------------------------+
void TlsSlot::SetTID(pthread_t tid)
{
	m_tid = tid;
}
//---------------------------------------------------------------------+
// Get the pointer TLS pointer stored by the application for this thread.
//---------------------------------------------------------------------+
void* TlsSlot::GetPtr()
{
	return m_lpPtr;
}
//---------------------------------------------------------------------+
// Set the TLS pointer to the value specified by the application.
//---------------------------------------------------------------------+
void TlsSlot::SetPtr(void* ptr)
{
	m_lpPtr = ptr;
}
//---------------------------------------------------------------------+
// TlsSlot copy constructor
//---------------------------------------------------------------------+
TlsSlot::TlsSlot(TlsSlot& that)
{
	m_tid = that.m_tid;
	m_lpPtr = that.m_lpPtr;
}
//---------------------------------------------------------------------+
TlsSlot::TlsSlot()
{
	m_lpPtr = NULL;
	m_tid = (pthread_t)-1;
}
//---------------------------------------------------------------------+
// Static member variables used in the TlsStore class.
// These are static so that the same varibles are visable from all
// instance object of the class.
// Of particular importance here are the m_mLock mutex and the m_this
// pointer to the TlsStore. The m_this ptr is used in the creation of
// The TlsStore singleton object.
//---------------------------------------------------------------------+

pthread_mutex_t TlsStore::m_mLock;
const int		TlsStore::m_iResizeChunck = 16;
int             TlsStore::m_iIndex;
int             TlsStore::m_iHighestIndex;
TlsSlot*		TlsStore::m_lpTlsPtrArry;
TlsStore*		TlsStore::m_this;

//---------------------------------------------------------------------+
// TlsStore constructor. Important make sure to initialize the mutex.
//---------------------------------------------------------------------+
TlsStore::TlsStore()
{
	pthread_mutex_init(&m_mLock, NULL);

	m_iIndex = 0;
	m_iHighestIndex = 0;
}
//---------------------------------------------------------------------+
// TlsStore distructor. Delete the array of TlsSlot and destroy the mutex
//---------------------------------------------------------------------+
TlsStore::~TlsStore()
{
	delete[] m_lpTlsPtrArry;
	pthread_mutex_destroy(&m_mLock);
}
//---------------------------------------------------------------------+
// Simplified method to use the mutex. Users only need to call Lock
// to acquire or wait for the mutex.
//---------------------------------------------------------------------+
void TlsStore::Lock()
{
	pthread_mutex_lock(&m_mLock);
}
//---------------------------------------------------------------------+
// Release ownership of the mutex
//---------------------------------------------------------------------+
void TlsStore::Unlock()
{
	pthread_mutex_unlock(&m_mLock);
}
//---------------------------------------------------------------------+
// Alloc and Free are for the C interface passthru. They are really just 
// place keepers.
//---------------------------------------------------------------------+
DWORD TlsStore::Alloc()
{
	return 0;
}
//---------------------------------------------------------------------+
BOOL TlsStore::Free(DWORD tlsIndex)
{
	return TRUE;
}
//---------------------------------------------------------------------+
// TlsStore::FindTls finds the TlsSlot for the currently executing 
// thread. The routine gets the current thread ID and uses it as a look
// up key in the TlsStore. The TlsStore is searched sequentially for
// the TlsSlot that has a thread ID that matches the current thread ID.
//---------------------------------------------------------------------+
TlsSlot* TlsStore::FindTls()
{
	//
	// Get local thread ID
	//
	pthread_t	tid = pthread_self();
	//
	// look for thread ID in array
	// if tls slot found for this tid,
	// return the pointer,
	//
	for (int i = 0; i < m_iHighestIndex; i++)
		if (pthread_equal(m_lpTlsPtrArry[i].GetTID(), tid))
			return &m_lpTlsPtrArry[i];
	//
	// else
	// return NULL!
	//
	return (TlsSlot*)NULL;
}
//---------------------------------------------------------------------+
// Returns:		void*	pointer to users TLS memory.
// Input:		DWORD 	Ignored
//
// TlsStore::GetValue returns a void pointer that has been already been
// stored by the application's thread. If there is no TlsSlot found for
// the current thread, NULL is returned. The input parameter is ignored.
//---------------------------------------------------------------------+
void* TlsStore::GetValue(DWORD)
{
	void*	rptr;		// the return ptr.

	Lock();				// 	Only one thread at a time.
	//
	// Get local thread ID
	//
	TlsSlot* tls = FindTls();

	if (NULL == tls)
		rptr = (void*) NULL;
	else
		rptr = tls->GetPtr();

	Unlock();			// OK another can run now.

	return(rptr);

}
//---------------------------------------------------------------------+
// TlsStore::SetValue
// Returns:		BOOL	TRUE for success, 
//						FALSE critical failure (possibly out of memory)
// Arguments	DWORD	Not currently used.
//				void*	The pointer to thread storage allocated by the
//						application.
//
// This routine sets the TLS pointer in the TlsStore to the value 
// specified in arg2. The TlsStore is searched for a TlsSlot associated 
// with the current thread. If one is found, the pointer value is stored.
// If a TlsSlot is not found, one is allocated for the current thread and
// the value is stored.
//---------------------------------------------------------------------+
BOOL TlsStore::SetValue( DWORD, void* lpValue )
{
	// Prevent any other thread from getting in here
	//
	Lock();
	//
	// Find the current slot for the ptr passed in.
	// If one found, set it's ptr value.
	// 
	TlsSlot*	tls = FindTls();
	//
	// If no existing TLS slot found, allocate one.
	//
	if (NULL == tls)
	{
		try
		{
			//
			// Get the next one.
			//
			++m_iIndex;
			//
			// Check to see if all avail TLS slots used.
			//
			if (m_iHighestIndex < m_iIndex)
			{
				//
				// All existing TLS slots used,
				// need to grow array
				//
				const int newSize = m_iHighestIndex + m_iResizeChunck;
				TlsSlot*	newStore = new TlsSlot[newSize];
				for (int i = 0 ; i < m_iHighestIndex; i++)
				{
					newStore[i] = m_lpTlsPtrArry[i];
				}
				//
				// Update the count for the new TLS store.
				//
				m_iHighestIndex += m_iResizeChunck;
				//
				// Delete old array and store ptr to new one.
				//
				delete[] m_lpTlsPtrArry;
				m_lpTlsPtrArry = newStore;
			}
		}
		catch (...)
		{
			//
			// Insure the TLS Store is not locked.
			//
			Unlock();
			// perror("Exception caught in TlsStore::SetValue...aborting\n");
			return FALSE;
		}
		tls = &m_lpTlsPtrArry[m_iIndex];
	}
	//
	// Now have a valid TLS slot, store the values.
	//
	tls->SetPtr(lpValue);
	tls->SetTID(pthread_self());

	Unlock();
	return TRUE;
}
//---------------------------------------------------------------------+
// TlsStore::Instance
// Returns:		TlsStore&	A reference to the TlsStore singleton object.
// Args:		None
//
// Returns a reference to the single TlsStore object for the process.
// 
//---------------------------------------------------------------------+
TlsStore& TlsStore::Instance()
{
	if (NULL == m_this)
		m_this = new TlsStore();
	return *m_this;
}
//---------------------------------------------------------------------+
// The following are the C interface routines used to bridge from a
// C style API into C++ objects. Each of these routines get the 
// reference to the TlsStore and then calls the member method of TlsStore
// that matches the simple C routine's name.
//---------------------------------------------------------------------+
DWORD TlsAlloc()
{
	TlsStore& tls = TlsStore::Instance();
	return tls.Alloc();
}
//---------------------------------------------------------------------+
BOOL TlsFree(DWORD dIndex)
{
	TlsStore& tls = TlsStore::Instance();
	return tls.Free(dIndex);
}
//---------------------------------------------------------------------+
void* TlsGetValue(DWORD dIndex)
{
	void*	rptr;
	TlsStore& tls = TlsStore::Instance();
	rptr = tls.GetValue(dIndex);
	if (NULL == rptr)
	{
		Allocate();
		rptr = tls.GetValue(dIndex);
	}
	return (rptr);
}
//---------------------------------------------------------------------+
BOOL TlsSetValue(DWORD dIndex, void* lpValue)
{
	TlsStore& tls = TlsStore::Instance();
	return tls.SetValue(dIndex, lpValue);
}
//---------------------------------------------------------------------+
void * GlobalAlloc(DWORD type, DWORD size)
{
	return (void*) calloc(1, size);
}
//---------------------------------------------------------------------+
void GlobalFree(void *lpPtr)
{
	if (NULL != lpPtr)
		free(lpPtr);
}
//---------------------------------------------------------------------+

#endif	//ifndef _WIN32


//---------------------------------------------------------------------+
//  Function: DllMain
//  Desc:     This function contains the constructor and destructor 
//            logic for this Dynamic Link Library.
//  History: 
//  11/07/98  DMT Created original code.
//---------------------------------------------------------------------+
BOOL	APIENTRY	DllMain( 
		HINSTANCE	hinstDLL,		// handle to DLL module 
		DWORD		fdwReason,		// reason for calling function 
		LPVOID		lpvReserved) {	// reserved 

//
//  Prepare or cleanup based upon OS Reason for invocation
//
switch (fdwReason) {
	case DLL_PROCESS_ATTACH: {
		//
		//  Allocate a thread local storage index
		//
		gp_tlsIndex = TlsAlloc();

		//
		//  Allocate the class instances for the primary thread
		//
		Allocate();
		break;
		}

	case DLL_THREAD_ATTACH: {
		//
		//  Allocate the class instances for this thread
		//
		Allocate();
		break;
		}

	case DLL_PROCESS_DETACH: {
		//
		//  Release all instances of classes for the primary thread
		//
		Release();

		//
		//  Release the thread local storage index
		//
		TlsFree(gp_tlsIndex);
		break;
		}

	case DLL_THREAD_DETACH: {
		//
		//  Release all instances of classes for this thread
		//
		Release();
		break;
		}
	}
return TRUE;
}


//---------------------------------------------------------------------+
//  Function: Allocate
//  Desc:     This function contains the constructor and destructor 
//            logic for this Dynamic Link Library.
//  History: 
//  11/07/98  DMT Created original code.
//  09/24/99  DRC Added instantiating the CAPIParmSet Object.
//---------------------------------------------------------------------+
void	Allocate(void) {
//
//  declare local variables
//
threadObjects* lu_tObj;

//
//  Dynamically construct and init a Thread Object structure
//  with instances of the various classes for this thread.
//
lu_tObj = (threadObjects*) GlobalAlloc(GPTR, sizeof(threadObjects));
if (lu_tObj != (threadObjects*) NULL) {

	lu_tObj->lu_queue_mgr	= new CQueueMgr;
	lu_tObj->lu_API_ParmSet	= new CAPIParmSet;

	//
	//  Store this Thread's Object structure pointer
	//
	TlsSetValue(gp_tlsIndex, lu_tObj);
}
return;
}

//---------------------------------------------------------------------+
//  Function: Release
//  Author:   David Tamkun
//  Written:  November 1998
//  Desc:     This function contains the constructor and destructor 
//            logic for this Dynamic Link Library.
//  History: 
//  11/07/98  DMT Created original code.
//  09/24/99  DRC Added releasing the CAPIParmSet Object.
//---------------------------------------------------------------------+
void	Release(void) {
//
//  declare local variables
//
threadObjects* lu_tObj;
		
//
//  Retrieve this Thread's Object structure pointer
//
lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
if (lu_tObj == (threadObjects*) NULL) goto ExitPt;

// 
//  Release the instances of the various classes for this thread.
//

delete lu_tObj->lu_queue_mgr;
delete lu_tObj->lu_API_ParmSet;

// 
//  Release this Thread's Object structure.
//
GlobalFree(lu_tObj);
lu_tObj = (threadObjects*) NULL;

ExitPt:
	return;
}
//---------------------------------------------------------------------+
//  Component: Public External Interface
//  Author:    David Tamkun
//  Written:   November 1998
//  Desc:      The following functions have been exported using C 
//             calling conventions and comprise the PUBLIC EXTERNAL 
//             interface to Objects contained within this Dynamic Link 
//             Library (DLL).
//  History: 
//  11/07/98  DMT Created original code.
//  12/04/98  DMT Changed getlasterror comp code and reason code
//                parameters from long& to long* to try to get 
//                VB calls to work.
//  12/09/98  DMT Added Report Options, Message Type, and Feedback Code
//                to PUT call.  Added GET call too.
//  12/18/98  DMT Changing GET call to not expose "required data length"
//                and "reason code".  Will need separate function for
//                proper dead letter queue handling and this will
//                be addressed later.
//  01/07/98  DMT Adding GET2 call for DeadLetter
//  09/24/99  DRC Added initializing a CAPIParmSet.
//---------------------------------------------------------------------+
#ifdef __cplusplus
extern "C" {
#endif
//
//  Exported Interface Functions
//

DllExport long  __stdcall init(char* vs_q_mgr_name){
	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->Init(vs_q_mgr_name); 
}

DllExport long  __stdcall initq( int& ri_qid, char* vs_q_name, 
								BOOL vb_input_shared, 
								BOOL vb_input_excl, 
								BOOL vb_browse, 
								BOOL vb_output ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->InitQ(ri_qid, vs_q_name, 
								vb_input_shared,	vb_input_excl,
								vb_browse,			vb_output); 
}





DllExport long  __stdcall put( int vi_queue_id,	char* vs_msg_buffer, 
						long vl_data_length,	long  vl_expiration,
						BOOL vb_batch_ind,		long  vl_persistence,
						char* vs_msg_id,        char* vs_correl_id,
						char* vs_reply_to_q,    long  vl_report_options,
						long  vl_msg_type,      long  vl_feedback_code,
						char* vs_msg_format,	long  vl_priority,
						BOOL  vb_fail_if_quiescing,
#ifdef _ver2_
						BOOL  vb_xlate_key_values,
						BOOL  vb_log_receipt,   BOOL  vb_data_receipt){
#else
						BOOL  vb_xlate_key_values){
#endif

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);

	lu_tObj->lu_API_ParmSet->setPut(
								vi_queue_id,	vs_msg_buffer,
								vl_data_length,	vl_expiration,
								vb_batch_ind,	vl_persistence,
								vs_msg_id,		vs_correl_id,
								vs_reply_to_q,	vl_report_options,
								vl_msg_type,	vl_feedback_code,
								vs_msg_format,	vl_priority,
								vb_fail_if_quiescing,
								vb_xlate_key_values,
#ifdef _ver2_
					            vb_log_receipt,   vb_data_receipt);
#else
	                            1,              1);
#endif

	return lu_tObj->lu_queue_mgr->Put(
								vi_queue_id,		vs_msg_buffer, 
								vl_data_length,		vl_expiration, 
								vb_batch_ind,		vl_persistence,
								vs_msg_id,			vs_correl_id,
								vs_reply_to_q,		vl_report_options,
								vl_msg_type,		vl_feedback_code,
					            vs_msg_format,		vl_priority,
					            vb_fail_if_quiescing,
				                vb_xlate_key_values,
								lu_tObj->lu_API_ParmSet); 
}


DllExport long  __stdcall puta(	int vi_queue_id,	
								PMQMD   rmqmd_msg_desc,
								PMQPMO  rmqpmo_put_options, 
								long    vl_data_length, 
								char*   vs_msg_buffer,
								long&   rl_comp_code,
								long&   rl_reason_code,
#ifdef _ver2_
					         	BOOL  vb_xlate_key_values,
					        	BOOL  vb_log_receipt,   
								BOOL  vb_data_receipt){
#else
						        BOOL  vb_xlate_key_values){
#endif
	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);

	lu_tObj->lu_API_ParmSet->setPutA(
								vi_queue_id,		rmqmd_msg_desc,
								rmqpmo_put_options,	vl_data_length,
								vs_msg_buffer,		rl_comp_code,
								rl_reason_code,		vb_xlate_key_values,
#ifdef _ver2_
					            vb_log_receipt,     vb_data_receipt);
#else
	                            1,                  1);
#endif
	return lu_tObj->lu_queue_mgr->PutA(
								vi_queue_id,		rmqmd_msg_desc,
								rmqpmo_put_options,	vl_data_length,
								vs_msg_buffer,		rl_comp_code,
								rl_reason_code,		vb_xlate_key_values,
								lu_tObj->lu_API_ParmSet);
}


DllExport long  __stdcall get(
					int		vi_queue_id,		char*	vs_msg_buffer,
					long	vl_buffer_length,	long&	rl_data_length,
					long&	rl_expiration,		BOOL	vb_batch_ind,
					BOOL	vb_accept_trunc,	long&	rl_persistence,
					char*	vs_msg_id,			char*	vs_correl_id,
					char*	vs_reply_to_q,		long&	rl_msg_type,
					long&	rl_feedback_code,	long&	rl_report_options,
					char*	vs_format,			char	vc_get_mode,
					long	vl_wait_interval,	BOOL	vb_match_msg_id,
					BOOL	vb_match_correl_id,	long&	rl_priority,
					BOOL	vb_fail_if_quiescing,
					BOOL	vb_xlate_key_values){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);

	lu_tObj->lu_API_ParmSet->setGet(
					vi_queue_id,			vs_msg_buffer,
					vl_buffer_length,		rl_data_length,
					rl_expiration,			vb_batch_ind,
					vb_accept_trunc,		rl_persistence,
					vs_msg_id,				vs_correl_id,
					vs_reply_to_q,			rl_msg_type,
					rl_feedback_code,		rl_report_options,
					vs_format,				vc_get_mode,
					vl_wait_interval,		vb_match_msg_id,
					vb_match_correl_id,		rl_priority,
					vb_fail_if_quiescing,	vb_xlate_key_values);

	return lu_tObj->lu_queue_mgr->Get(		vi_queue_id,       
		              vs_msg_buffer,		vl_buffer_length,
					  rl_data_length,		rl_expiration, 
					  vb_batch_ind,			vb_accept_trunc,
					  rl_persistence,		vs_msg_id,
					  vs_correl_id,			vs_reply_to_q,
					  rl_msg_type,			rl_feedback_code,
					  rl_report_options,	vs_format,
					  vc_get_mode,			vl_wait_interval,  
					  vb_match_msg_id,		vb_match_correl_id,
					  rl_priority,			vb_fail_if_quiescing,
					  vb_xlate_key_values,	lu_tObj->lu_API_ParmSet);

}

DllExport long  __stdcall geta(int vi_queue_id,
		          PMQMD   rmqmd_msg_desc, 
				  PMQGMO  rmqgmo_get_options, 
				  long    vl_buffer_length, 
				  char*   rs_msg_buffer,
				  long&   rl_data_length,  
			      long&   rl_comp_code,
				  long&   rl_reason_code,
				  BOOL    vb_xlate_key_values) {


	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);

	lu_tObj->lu_API_ParmSet->setGetA(
								vi_queue_id,		rmqmd_msg_desc,
								rmqgmo_get_options,	vl_buffer_length,
								rs_msg_buffer,		rl_data_length,
								rl_comp_code,		rl_reason_code,
								vb_xlate_key_values);

	return lu_tObj->lu_queue_mgr->GetA(
								vi_queue_id,			rmqmd_msg_desc,
								rmqgmo_get_options,		vl_buffer_length,
								rs_msg_buffer,			rl_data_length,
								rl_comp_code,			rl_reason_code,
								vb_xlate_key_values,	
                                                lu_tObj->lu_API_ParmSet);

}





DllExport long  __stdcall commit( void ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->Commit(); 
}




DllExport long  __stdcall rollback( void ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->Rollback(); 
}



DllExport long  __stdcall komq_close( int vi_queue_id ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->Close( vi_queue_id ); 
}




DllExport long  __stdcall disconnect( void ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	return lu_tObj->lu_queue_mgr->Disconnect(); 
}


DllExport long __stdcall getlasterror( long* rl_comp_code, long* rl_reason_code,
									   char* is_context, long vl_buffer_size) {

	long			ll_comp_code	= 0;
	long			ll_reason_code	= 0;
	long			ll_return		= 0;
	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);


	ll_return = lu_tObj->lu_queue_mgr->GetLastError( ll_comp_code, 
							   ll_reason_code, is_context, vl_buffer_size);

	*rl_comp_code   = ll_comp_code;
	*rl_reason_code = ll_reason_code;

	return ll_return;
}



DllExport long __stdcall getreasoncode( long vl_value, char* vs_reason, 
							    long vl_buffer_size ){

	threadObjects* lu_tObj;

	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);

	return lu_tObj->lu_queue_mgr->GetReasonCode(vl_value, vs_reason, 
							    vl_buffer_size );

}



DllExport long __stdcall getAttrLng(int ai_Type){
	threadObjects* lu_tObj;
	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	switch (ai_Type){
	case CCSID:                           // Coded Character Set Id
		 return lu_tObj->lu_API_ParmSet->vl_CCSID;
	case Encoding:                        // Numeric Encoding Scheme
		 return lu_tObj->lu_API_ParmSet->vl_Encoding;
	default:
		 return KORC_INVALID_ARGUMENT;
	}
}



DllExport char* __stdcall getAttrStr(int ai_Type){
	threadObjects* lu_tObj;
	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	switch (ai_Type){
	case RestartKey:                      // Restart Key
		 return lu_tObj->lu_API_ParmSet->getRestartKey()->getPointer();
	default:
		 return NULL;
	}
}


DllExport long __stdcall getAttrStr2(int ai_Type, char* as_Arg){
	threadObjects* lu_tObj;
	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	switch (ai_Type){
	case RestartKey:                      // Restart Key
		 *lu_tObj->lu_API_ParmSet->getRestartKey() >> as_Arg;
		 return KOCC_GOOD;
	default:
		 return KORC_INVALID_ARGUMENT;
	}
}


DllExport long __stdcall setAttrLng(int ai_Type, long al_Arg){
	threadObjects* lu_tObj;
	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	switch (ai_Type){
	case CCSID:                           // Coded Character Set Id
		 lu_tObj->lu_API_ParmSet->vl_CCSID    = al_Arg;
		 return KOCC_GOOD;
	case Encoding:                        // Numeric Encoding Scheme
		 lu_tObj->lu_API_ParmSet->vl_Encoding = al_Arg;
		 return KOCC_GOOD;
	default:
		 return KORC_INVALID_ARGUMENT;
	}
}


DllExport long __stdcall setAttrStr(int ai_Type, char* as_Arg){
	threadObjects* lu_tObj;
	lu_tObj = (threadObjects*) TlsGetValue(gp_tlsIndex);
	switch (ai_Type){
	case RestartKey:                      // Restart Key
		 lu_tObj->lu_API_ParmSet->setRestartKey(as_Arg);
		 return KOCC_GOOD;
	default:
		 return KORC_INVALID_ARGUMENT;

	}
}


#ifdef __cplusplus
}
#endif
