//---------------------------------------------------------------------+
//  File:     Service.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation file for the CService class.  CService
//            provides a common ancestor for both Transfer and 
//            Non-Transfer services allowing them to share the 
//            fact that they all use a CLog and CParamFile object.
//
//  Dev Log: 
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
#include "Service.h"

//---------------------------------------------------------------------+
//  Method:   CService (Default Constructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Shuts down Queue Manager and Buffer classes.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
CService::CService() : CAny2() {

}  /* End CService */





//---------------------------------------------------------------------+
//  Method:   ~CService (Destructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Shuts down Queue Manager and Buffer classes.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
CService::~CService(){

}  /* End ~CService */





long  CService::CheckParms(void){ return 0;}



//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Performs class initialization.
//
//  Args:     ptr to Log Class
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//  12/05/00 DRC Moved CheckParms invocation from CTransfer to its 
//               ancestor to make this logic available for the other
//               decendant objects as well.
//---------------------------------------------------------------------+
long CService::initSelf(CLog* v_pcLog) {

	long	ll_return = KOCC_GOOD;

	//-----------------------------------------------------------------+
	// Initialize Log Object Pointers
	//-----------------------------------------------------------------+
	m_pcLog		= v_pcLog;

	ll_return = CAny2::initSelf();

	//-----------------------------------------------------------------+
	// Check Parameters - Make sure we have valid parameters for this
	//                    service.
	//-----------------------------------------------------------------+
	if (ll_return == KOCC_GOOD) {
		ll_return = CheckParms();
	}

	return ll_return;
}  /* End initSelf */





//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Shuts down Queue Manager and Buffer classes.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  11/09/00 DRC Created original version.
//---------------------------------------------------------------------+
long CService::termSelf() {

	m_pcLog		= NULL;

	return CAny2::termSelf();

}  /* End termSelf */














