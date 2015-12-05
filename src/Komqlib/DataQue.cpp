/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) DataQue.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     DataQue.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CDataQue
//            Class.
//
//            This class represents a Data Queue in the Utilities.
//
//            There really isn't much different between it and CQue.
//  
//  Dev Log: 
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#include "DataQue.h"





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Initializes Members in Ancestor
//
//  Args:     Queue Manager Object Reference
//            long	Buffer Size
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CDataQue::CDataQue(CQueMgr* v_pcQM) : CQue(v_pcQM)
{
	is_InstanceName	= "Data Queue";
}


CDataQue::CDataQue() : CQue()
{
	is_InstanceName	= "Data Queue";
}







//---------------------------------------------------------------------+
//  Method:   Destructor
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
CDataQue::~CDataQue()
{
}





//---------------------------------------------------------------------+
//  Method:   GetNext
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Catches the generic "No More Messages" exception and
//            throws the more specific "No More Data Messages"
//            exception.
//
//  Args:     char*	Message ID
//            ulong	Correlation ID
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CDataQue::GetNext(CBuffer& cBuffer,
					      const char* szMsgID, ulong ulCorlID)
{
	return CQue::GetNext(cBuffer, szMsgID, ulCorlID);
}


