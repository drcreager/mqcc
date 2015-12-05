/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/19	@(#) CmdLine.C 1.1@(#)
 */
//---------------------------------------------------------------------+
//  File:     CmdLine.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CCmdArgs
//            Class.
// 
//            This class gets Command Line Parameters.
// 
//  Dev Log: 
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//  04/27/99 DMT Constructor Change.
//               Removing first parm begining with "/" check.  
//               prevents AIX users from fully qualifying parameter 
//               file.
//---------------------------------------------------------------------+
#include "CmdLine.h"







//---------------------------------------------------------------------+
//  Method:   GetParamFileName
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  char* Pointer to Parameter File Name String
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const char* CCmdArgs::GetParamFileName()
{
	return (const char *)m_szParameterFile;
}





//---------------------------------------------------------------------+
//  Method:   GetBatchInterval
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  ulong Batch Interval
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const ulong CCmdArgs::GetBatchInterval() const
{
	return (const ulong) m_ulBatchInterval;
}





//---------------------------------------------------------------------+
//  Method:   IsRestart
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  BOOL	True if Users specified Restart, FALSE otherwise
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const BOOL CCmdArgs::IsRestart() const
{
	return (const BOOL) m_bRestart;
}






//---------------------------------------------------------------------+
//  Method:   BatchIntervalOverride
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     None
//
//  Returns:  BOOL	returns TRUE if user specified a Batch Interval 
//                  on the command line.  FALSE otherwise
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
const BOOL CCmdArgs::BatchIntervalOverride() const
{
	BOOL	rc = FALSE;
	if (-1 != m_ulBatchInterval) 
		rc = TRUE;

	return rc;
}





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     int		argument count
//            char*[]	array of pointers to command line args.
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/21/99 DMT Added Comments.  Corrected Validation of Batch 
//               Interval to accept /b0 as a valid value.
//  04/27/99 DMT Removing first parm begining with "/" check.  
//               prevents AIX users from fully qualifying parameter 
//               file.
//---------------------------------------------------------------------+
CCmdArgs::CCmdArgs(int argc, const char *argv[]) {

	//-----------------------------------------------------------------+
	// Set Default Values
	//-----------------------------------------------------------------+
	is_InstanceName		= "Command Line";
	m_bRestart			= FALSE;
	m_ulBatchInterval	= -1L;
	memset(m_szParameterFile, 0, sizeof(m_szParameterFile));

	CString ls_msg(132);
	int li_error = 1;

	try {

	//-----------------------------------------------------------------+
	// Define Possible "labeled" Command Line Parameters.
	//-----------------------------------------------------------------+
	static const CString szRestart("/r");
	static const CString szBatchInterval("/b");

	CString szArg;


	if (argc <= 1) {	// only program name specified.

		iu_errors.AddError(KORC_MISSING_COMMAND_PARMS, KOCC_FAIL, 
			"Required Parameter Filename not specified.",
			"CCmdArgs::Constructor:1", 
			is_InstanceName.getPointer());


		li_error = 1;
		throw li_error;
	}

	// DMT 4/27/99 - removing this check.  Prevents AIX users
	//               from providing fully qualified path and file
	//               name for parameter file.
	//if (*argv[1] == '/')
	//{
	//
	//	iu_errors.AddError(KORC_MISSING_COMMAND_PARMS, KOCC_FAIL, 
	//		"Required Parameter Filename not specified.",
	//		"CCmdArgs::Constructor:2", 
	//		is_InstanceName.getPointer());
	//
	//	li_error = 2;
	//	throw li_error;
	//
	//}
	

	strncpy(m_szParameterFile, argv[1], PATH_MAX);

		
	for ( int i = 2; i < argc; i++ )
	{
		const char*  parg = argv[i];
		if (strlen(parg) >= 2 && parg[0] == '/')
		{
			switch(parg[1])
			{
				//
				// Batch interval, must be numeric and >= 0 -DMT 2/21/99
				//
				case ('b'):
				{
					const char* p;
					for (p = &parg[2]; isdigit(*p); p++);
					if (*p == '\0' && p != &parg[2])
						m_ulBatchInterval = atol(&parg[2]);
					else
					{
						ls_msg = "";
						ls_msg << "Invalid Argument '" << (char*) argv[i] <<  "'.";

						iu_errors.AddError(KORC_NONNUMERIC_CMD_PARM, KOCC_FAIL, 
							ls_msg,
							"CCmdArgs::Constructor:3", 
							is_InstanceName.getPointer());
						
				 	    li_error = 3;
					    throw li_error;

						
					}
						
					if (m_ulBatchInterval < 0)				// DMT 2/21/99
					{

						ls_msg << "Invalid Batch Interval '" << (char*) argv[i] << "'.";

						iu_errors.AddError(KORC_INVALID_BATCH_INTERVAL, KOCC_FAIL, 
							ls_msg,
							"CCmdArgs::Constructor:4", 
							is_InstanceName.getPointer());

				 	    li_error = 4;
					    throw li_error;
						
					}
		
					break;
				}
				//
				// Must be just a switch.
				//
				case ('r'):
					if (strlen(argv[i]) == 2)
						m_bRestart = TRUE;
					else
					{

						ls_msg << "Invalid Argument '" << (char*) argv[i] << "'";

						iu_errors.AddError(KORC_INVALID_ARGUMENT, KOCC_FAIL, 
							ls_msg,
							"CCmdArgs::Constructor:5", 
							is_InstanceName.getPointer());

				 	    li_error = 5;
					    throw li_error;
						
					}
					break;
				//
				// We got a switch argument that was invalid.
				//
				default:
				{

					ls_msg << "Invalid Argument '" << (char*) argv[i] << "'";

					iu_errors.AddError(KORC_PARM_SYNTAX_ERROR, KOCC_FAIL, 
						ls_msg,
						"CCmdArgs::Constructor:6", 
						is_InstanceName.getPointer());

				 	    li_error = 6;
					    throw li_error;
					
				}
			} // end Switch
		}
		else
		{

			ls_msg << "Invalid Argument '" << (char*) argv[i] << "'";

			iu_errors.AddError(KORC_PARM_SYNTAX_ERROR, KOCC_FAIL, 
				ls_msg,
				"CCmdArgs::Constructor:7", 
				is_InstanceName.getPointer());

				li_error = 7;
			    throw li_error;
			
		} // end if
	} // end for

	} // end try

	catch (int ai_error) {
		// Error Already Added to collection
		ai_error = ai_error; // psuedo-ref to make the compiler happy

	}


	catch (...) {

		ls_msg << "Unexpected Error!!";

		iu_errors.AddError(KORC_UNSPECIFIED, KOCC_FAIL, 
				ls_msg,
				"CCmdArgs::Constructor:8", 
				is_InstanceName.getPointer());

	}

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
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CCmdArgs::~CCmdArgs()
{
}

