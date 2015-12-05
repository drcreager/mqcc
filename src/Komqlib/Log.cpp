//---------------------------------------------------------------------+
//  File:     CLog.cpp
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     This file contains the implementation of the CLog Class.
//
//            The class logs information to the console and or log
//            file.  At on time, it also supported "logging" to 
//            an error queue.
//  
//  Dev Log:  Note that the "Error Queue" stuff is commented out and
//            not supported currently. (DMT 3/99)
//
//            Consider figuring out how to make this a specialization 
//            of CFile, instead of ofstream.  Will have to deal with
//            "chicken and egg" issues.
//
//            See the Dev Log note for the Translate Errno Method for
//            another change idea.
//
//
//  TechNote: Specialized setup requirements exist for this class due 
//            to the use of cross-aggregation.  Refer to the documentation
//            in RespBatch.h for more details.
//
//  History: 
//  12/31/98 DMT Created original version.
//  02/12/00 DRC Sorted the methods in the source code and inserted the
//               close block/bracket comments.
//           DRC Added the RespBatch pointer and a setter method
//           DRC Added logic to load the RespBatch from Log input.
//           DRC Moved the Banner method into this class.
//  11/10/00 DRC Added validation of m_pcRespBatch prior to use to avoid
//               errors when it is not populated as in Extended Services.
//---------------------------------------------------------------------+
#include "RespBatch.h"  /* DO NOT INCLUDE Log.h!  See TechNote above. */



//---------------------------------------------------------------------+
//  Method:   Banner
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     Displays program information.
//
//  Args:     ostream to write info to.  (Either cout or cLog).
//
//  Returns:  Nothing
//  
//  History: 
//  03/??/99  DMT Created original version.
//  02/12/00  DRC Relocated method from CCmdInterface to CLog
//  11/10/00  DRC Corrected code to display a logical date.
//  01/10/01  DRC Insert a blank line before the banner.
//---------------------------------------------------------------------+
long CLog::Banner(CString* a_psAppName, 
				  CString* a_psAppVersion, 
				  CString* a_psAppDesc,
				  int      a_iEntryType) {

	long       ll_result;
	CString	   ls_msg;
	time_t     l_time;
	struct tm* l_Today;
	char       l_szToday[25];

	//
	//  Set the time String 
	//  Format: Ddd Mmm dd yyyy hh:mm:ss
	//  Sample: Fri Apr 29 1994 12:25:12
	//
	time(&l_time);
	l_Today = localtime(&l_time);
	strftime(l_szToday, 25,"%a %b %d %Y %H:%M:%S", l_Today);

	ls_msg << "\n" << *a_psAppName	<< " Version " << *a_psAppVersion;
	ll_result = WriteLog(ls_msg, a_iEntryType);

	if (ll_result == 0) ll_result = WriteLog(*a_psAppDesc, a_iEntryType);
	if (ll_result == 0) ll_result = WriteLog(l_szToday, a_iEntryType);
	return ll_result;
}  /* End Method */









//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     None
//
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//  11/10/00 DRC Added m_sFileName and m_pcRespBatch to the list of 
//               attributes to be initialized.
//---------------------------------------------------------------------+
CLog::CLog() : CAny2() {

	m_bOpen			= FALSE;		// ancestor member.
	m_bQueueOpen	= FALSE;
	is_InstanceName	= "Log File";
	m_sFileName     = "";
    m_pcRespBatch   = NULL;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   Copy Constructor
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     TBD
//  Returns:  Nothing
//  
//  Dev Log:  NOT IMPLEMENTED!!!!!!!
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CLog::CLog(const CLog &right) {

//	assert(0);

}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   Destructor 
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
CLog::~CLog() {

	termSelf();
 
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   Commit
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Flushes Output File Buffer to Disk to Ensure it's 
//            written.
//
//  Args:     None
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::Commit() {

	CString	ls_msg(255);
	long	ll_return = KOCC_GOOD;
	CString ls_errno_desc;

	flush();

	if (bad() || fail()) {

		ll_return = KOCC_FAIL;

		TranslateErrno(ls_errno_desc, errno);

		ls_msg << "Error Flushing Output Buffer for Output File '"
			   << m_sFileName << "'.  "
			   << ls_errno_desc;


		LogError(KORC_WRITE_TO_OUTPUT_FAIL, ll_return,
				ls_msg,
	  			"CLog::Commit:1",
  				is_InstanceName.getPointer(),
				LOGCONSOLE);
	}  /* End If */

	return ll_return;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   initQueue
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Registers KO MQ Series API Class (CKOMQAPI) with the
//            Queue Class and then Opens specified log Queue
//
//  Args:     I Pointer to CKOMQAPI Class
//
//            I CString	Queue Name
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log:  Currently, this is all commented out.  CKOMQAPI is an 
//            old class from the original Dead Letter Queue Program.
//            Now, you'd want to use the CQue class.
//
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
//long CLog::initQueue(KOMQAPI* vu_mqapi, CString& vs_queuename) {
//
//	m_cQueue.RegisterAPI(vu_mqapi);
//
//	// Open Queue for Output 
//	if(m_cQueue.InitQ(vs_queuename, false, false, false, true) != 0) {
//
//		CopyErrors(&m_cQueue);
//
//		return 2;
//	}
//	else {
//		m_bQueueOpen = true;
//		return 0;
//	}  /* End If */
//
//}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Initializes (Opens) specified log file
//
//  Args:     I CString	Log Filename
//
//  Returns:  int	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::initSelf(char* vs_filename) {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg(132);
	CString ls_errno_desc;


	// Call Method on Ancestor
	ll_return = CAny2::initSelf();

	if (vs_filename == NULL) {

		ll_return = KOCC_FAIL;

		ls_msg = "Log Filename cannot be NULL.";

		LogError(KORC_INVALID_FILE_NAME, ll_return,
			ls_msg,
	  		"CLog::initSelf:1", 
  			is_InstanceName.getPointer(),
			LOGCONSOLE);
	}  /* End If */


	if (ll_return == 0) {
		// init stuff here

		open(vs_filename, ios::out);

#ifndef _WIN32
		if (!rdbuf()->is_open()) {
#else
		if (!is_open()) {
#endif
			ll_return = KOCC_FAIL;

			TranslateErrno(ls_errno_desc, errno);

			ls_msg << "Error opening log file '"
				   << vs_filename << "'.  "
				   << ls_errno_desc;

			
			LogError(KORC_OPEN_OUTPUT_FAIL, ll_return,
				ls_msg,
	  			"CLog::initSelf:2", 
  				is_InstanceName.getPointer(),
				LOGCONSOLE);
		}
		else {
			m_bOpen = TRUE;
		}  /* End If */

	}  /* End If */
	return ll_return;
}  /* End Method */






long CLog::initSelf(CString& vs_filename) {
	return initSelf(vs_filename.getPointer());
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   LogError
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     Logs an error to the specified location.  Displays
//            a flower box around the error and precedes each line
//            with "*** ".  Also calls the WrapLine method to ensure
//            each line does not exceed 72 characters.
//
//  Args:     I long	Error Number
//            I long	Error Severity, ie KOCC_WARNING or KOCC_FAIL
//            I CString	Error Message
//            I char*	Location Code, ie "ClassName::Method:Seq#"
//            I int     Entry Type, ie LOGCONSOLE to go to Screen
//                                     LOGFILE to go to Log File
//                                     LOGDEFAULT (for both)
//
//                                     Console AND File is the default.
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::LogError (long    vl_error_number, long vl_severity,
		                     CString& vs_msg, 
							 char*    vs_location,
							 char*    vs_context,
							 int      vi_entry_type) {

	return LogError(vl_error_number, vl_severity,
		            vs_msg.getPointer(), 
					vs_location,
					vs_context,
					vi_entry_type);
}  /* End Method */






long CLog::LogError (long    vl_error_number, long vl_severity,
		                     char*    vs_msg, 
							 char*    vs_location,
							 char*    vs_context,
							 int      vi_entry_type) {


	CString	ls_msg;
	CString ls_wrapped;


	WrapLine(ls_wrapped, vs_msg, 72, "*** ");

//             12345678901234567890123456789012345678901234567890
	ls_msg << 
"************************************************************************\n"
//		   << "*** " << vs_msg << "\n"
           << ls_wrapped
		   << "*** "           << "\n"
		   << "*** Error Number: " << vl_error_number << "\n"
		   << "***     Severity: " << vl_severity     << "\n"
		   << "***     Location: " << vs_location     << "\n"
		   << "*** Program Area: " << vs_context      << "\n"
		   << 
"************************************************************************\n";

	return WriteLog(ls_msg, vi_entry_type);
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   LogErrors
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     Logs all errors in the specified object's Errors 
//            Collection.
//
//  Args:     Reference to descendent of CAny2
//            int	Entry Type
//                  LOGCONSOLE | LOGFILE | LOGDEFAULT
//
//                  Console AND File is the default.
//
//  Returns:  long	always returns 0
//  
//  Dev Log: 
//  History: 
//  03/19/99  DMT Created original version.
//---------------------------------------------------------------------+
long CLog::LogErrors(CAny2&	  cAny2,
					 int      vi_entry_type) {

	long	ll_return		= KOCC_GOOD;
	CString ls_msg(132);
	int		li_handle		= 0;
	long	ll_error_number	= 0;
	long	ll_severity		= 0;
	CString	ls_error_msg(132);
	CString ls_location(132);
	CString ls_context(132);

	//-----------------------------------------------------------------+
	// If there are errors to output.
	//-----------------------------------------------------------------+
	if(cAny2.GetErrorCount() >= 1) {

		//-------------------------------------------------------------+
		// Get the First Error
		//-------------------------------------------------------------+
		cAny2.GetFirstError(li_handle,   ll_error_number,
			                ll_severity, ls_error_msg,
							ls_location, ls_context);

		//-------------------------------------------------------------+
		// Write First Error
		//-------------------------------------------------------------+
		ll_return = LogError(ll_error_number, ll_severity, 
							 ls_error_msg,
							 ls_location.getPointer(), 
							 ls_context.getPointer(),
							 vi_entry_type);


		//-------------------------------------------------------------+
		// While we get more errors
		//-------------------------------------------------------------+
		while(cAny2.GetNextError(li_handle,   ll_error_number,
			                     ll_severity, ls_error_msg,
								 ls_location, ls_context)) {

			//---------------------------------------------------------+
			// Write more errors to the screen (aka console or cout)
			//     and the log file.
			//---------------------------------------------------------+
			ll_return = LogError(ll_error_number, ll_severity, 
								 ls_error_msg,
								 ls_location.getPointer(), 
								 ls_context.getPointer(),
								 vi_entry_type);
		}  /* End While */
	}  /* End If */

	return ll_return;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   Assignment Operator
//  Author:   David M Tamkun
//  Written:  December 1998
//  Desc:     TBD
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:  NOT IMPLEMENTED !!!!
//
//            Implementation of this should include a  check to make
//            sure caller isn't trying to assign an object to itself.
//  History: 
//  12/28/98 DMT Created original version.
//---------------------------------------------------------------------+
CLog & CLog::operator=(const CLog &right) {

//	assert(0);
	return *this;

}  /* End Method */







//---------------------------------------------------------------------+
//  Method:   setRespBatch
//  Author:   Daniel R Creager
//  Written:  February 2000
//  Desc:     set the member attribute for the Response Batch object
//  Args:     CRespBatch* the instantiated object.
//  Returns:  Nothing
//  
//  History: 
//  02/12/00 DRC Created original version.
//---------------------------------------------------------------------+
void CLog::setRespBatch(CRespBatch* a_pcRespBatch){
m_pcRespBatch = a_pcRespBatch;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   termQueue
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Closes the Queue
//
//  Args:     None
//
//  Returns:  long	always returns 0
//  
//  Dev Log:  All commented out.
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
//long CLog::termQueue() {
//
//	m_cQueue.Close();
//	m_bQueueOpen = false;
//
//	return 0;
//
//}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Closes log file if it is open.
//
//  Args:     None
//
//  Returns:  int	always returns 0
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::termSelf(void) {

	long ll_return = KOCC_GOOD;

	if (m_bOpen) {
		close();
		m_bOpen = FALSE;
	}  /* End If */

	return CAny2::termSelf();
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   TranslateErrno
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     This function takes an error number as defined in ERRNO.H
//            and returns a messag describing the error.
//
//  Args:     Ref to CString - msg returned
//            int            - errno as defined in errno.h
//
//  Returns:  Nothing
//  
//  Dev Log:  This function could probably placed elsewhere, but is 
//            related to logging errors, so it goes here for now.
//
//            Note that is function is duplicated on CParamFile and on
//            CLog.  ParamFile descendents don't have a log file 
//            pointer since most of their processing happens before
//            a Log File is opened.  This duplication should be 
//            eliminated when time allows.
//
//  History: 
//  03/28/99  DMT Created original version.
//---------------------------------------------------------------------+
void CLog::TranslateErrno (CString& rs_msg, int vi_errno) {

	rs_msg = "";

	switch (vi_errno) {


	case EACCES:

		rs_msg << "Permission Denied ("
			   << vi_errno
			   << ").";
		break;


	case EEXIST:

		rs_msg << "The file already exists ("
			   << vi_errno
			   << ").";
		break;


	case EINVAL:
		rs_msg << "The specified filename or part of its path "
		       << "does not exist or is invalid ("
			   << vi_errno
			   << ").";

		break;


	case ENOENT:
		rs_msg << "The specified filename or part of its path "
		       << "does not exist or is invalid ("
			   << vi_errno
			   << ").";

		break;


	case ENOSPC:
		rs_msg << "No disk space remaining ("
			   << vi_errno
			   << ").";
		break;


	case ENOMEM:
		rs_msg << "No disk space remaining ("
			   << vi_errno
			   << ").";
		break;


	default:
		rs_msg << "System Error Number ("
			   << vi_errno
			   << ").";
		break;

	}  /* End Switch */

}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   WrapLine
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     This function wraps a string to the specified line width
//            and begins each line with the specified line prefix.
//
//            Begins line with line prefix, then pops a word off the 
//            input string and checks to see if it will fit.
//            If so, the word and the delimiting space is added to the 
//            line.  If not, we add the current line to the output and
//            start building a new line.
//
//            Note that only a space is recognized as a delimiter.
//
//  Args:     Ref to CString - Wrapped String
//
//            char*          - Input String to wrap
//
//            uint           - Line Width to Wrap to (Width of line
//                             prefix is included in this width).
//
//            char*          - Line Prefix, characters to begin each
//                             line width.  (For Example, to "flowerbox"
//                             errors.
//
//  Returns:  Nothing
//  
//  Dev Log:  Input and Line Prefix must not be NULL.  Line Width must
//            be greater than 0.  Should be a better way to handle 
//            this, rather than just return.
//
//            Multiple spaces are replaced by only 1 space.  Consider
//            fixing this in the future.
//
//  History: 
//  03/28/99  DMT Created original version.
//---------------------------------------------------------------------+
void CLog::WrapLine(CString& s_out, char* sz_in, 
					     uint i_lw, char* sz_lp) {


	CString s_line;
	char*	sz_word	= NULL;


	//-----------------------------------------------------------------+
	//  Check input so we don't blow up.
	//-----------------------------------------------------------------+
	if (sz_in == NULL) {
		return;
	}  /* End If */

	if (sz_lp == NULL) {
		return;
	}  /* End If */

	if (i_lw <= 0) {
		return;
	}  /* End If */


	//-----------------------------------------------------------------+
	//  Begin the first line and get the first word.
	//-----------------------------------------------------------------+
	s_line = sz_lp;
	sz_word = strtok(sz_in, " ");


	//-----------------------------------------------------------------+
	//  While we have words, continue processing.
	//-----------------------------------------------------------------+
	while (sz_word != NULL) {


		//-------------------------------------------------------------+
		//  While the word fits on the line, continue processing.
		//-------------------------------------------------------------+
		while (s_line.vlen() + strlen(sz_word) + 1 <= i_lw) {

			// add word and space to line
			s_line << sz_word << " ";

			// get the next word.
			sz_word = strtok(NULL, " ");

			// if no more words, break out of this loop.
			if (sz_word == NULL) {
				break;
			}  /* End If */

		}  /* End While */

		// Append line to output and add line break.
		s_out << s_line << "\n";

		// Start new line beginning with Line Prefix.
		s_line = sz_lp;


		//-------------------------------------------------------------+
		// What if word we just got is too wide to fit on a line?
		// Force linebreak even though it exceeds line width.
		// Otherwise we'd loop forever.
		//-------------------------------------------------------------+
		if (sz_word != NULL) {

			if (s_line.vlen() + strlen(sz_word) + 1 > i_lw) {

				//  Add Current Line, huge word and Line Break to 
				//  Output
				s_out << s_line << sz_word << "\n";

				// Get Next Word
				sz_word = strtok(NULL, " ");

			}  /* End If */
		}  /* End If */
	}  /* End While */
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   WriteLog
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Writes a Log Entry to the Console, Log File, or Error
//            Queue as Specified.
//
//  Args:     I CString	Entry to Write
//
//            I int		Entry type:
//
//                      LOGCONSOLE	= Write to Console (stdout or cout)
//                      LOGFILE		= Write to Log File
//						LOGQUEUE	= Write to Error Queue
//
//                      Add these values together to write to more than
//                      one location.
//
//                      If 0, no output is written.
//
//                      Console AND File is the default.
//
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::WriteLog (CString& rs_entry, int vi_entry_type) {

	return WriteLog(rs_entry.getPointer(), vi_entry_type);
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   WriteLog
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Writes a Log Entry to the Console, Log File, or Error
//            Queue as Specified.
//
//  Args:     I char*	Entry to Write
//
//            I int		Entry type:
//
//                      LOGCONSOLE	= Write to Console (stdout or cout)
//                      LOGFILE		= Write to Log File
//						LOGQUEUE	= Write to Error Queue
//
//                      Add these values together to write to more than
//                      one location.
//
//                      If 0, no output is written.
//
//                      Console AND File is the default.
//
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log:  If the Error Queue is re-enabled in the future, make 
//            sure you handle ll_return correctly.  Dont' want
//            Error Queue success to mask Log File failure.
//  History: 
//  12/31/98 DMT Created original version.
//  02/12/00 DRC Added mirroring the LOGFILE enteries in CRespBatch.
//---------------------------------------------------------------------+
long CLog::WriteLog (char* rs_entry, int vi_entry_type) {

	long	ll_return = KOCC_GOOD;


	if (vi_entry_type & LOGCONSOLE) {
		cout << rs_entry << endl;
	}  /* End If */


	if ((vi_entry_type & LOGFILE) && m_bOpen) {
		//
		// write to log file
		//
		ll_return = WriteRecord(rs_entry);
		//
		// mirror the log file entry in the response batch
		//
		if (isValidPointer(m_pcRespBatch)) 
			m_pcRespBatch->set(rs_entry);
	}  /* End If */



	if((vi_entry_type & LOGQUEUE) && m_bQueueOpen) {

		// If the Error Queue is Connected, Put Status Msg
		// If an error occurrs, don't bother reporting it.

//		if (m_cQueue.isConnected()) {
//
//			is_msg_id		= "KOMQDLQ Status";
//			is_correl_id	= "000000000000000000000000";
//			is_replyto_q	= "No Reply Queue";
//
//			ll_return = m_cQueue.Put(rs_entry.getPointer(),
//			         rs_entry.vlen(), // Data Length
//					 -1,			// Expiration Interval Unlimited
//					 false,			// Batch Indicator, want it on queue
//					 true,			// Persistence Indicator
//					 is_msg_id,		// Msg ID
//					 is_correl_id,	// Correl ID
//					 is_replyto_q,	// Reply Queue Name
//					 0,				// No Report Options
//					 4,				// Msg Type = MQMT_REPORT = 4
//					 0);			// Feedback Code
//
//			if (ll_return > 0) {
//				CopyErrors(&m_cQueue);
//			}  /* End If */
//		}  /* End If */
	}  /* End If */

	return ll_return;

}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   WriteRecord
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Writes specifed Buffer to Output File
//
//  Args:     I char*	Info to Write
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log:  Should translate errno to descriptive string.
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::WriteRecord (char* vs_record) {

	CString	ls_msg(255);
	long	ll_return = KOCC_GOOD;

	try {
		*this << vs_record << endl;
	}

	catch (...) {
		// hopefully below catches error
		ll_return = KOCC_FAIL;

		ls_msg = "Unexpected Exception.";


		LogError(KORC_WRITE_TO_OUTPUT_FAIL, ll_return,
			ls_msg,
	  		"CLog::WriteRecord:1",
  			is_InstanceName.getPointer(),
			LOGCONSOLE);
	}

	if (bad() || fail()) {

		ll_return = KOCC_FAIL;


		ls_msg << "Error Writing to Output File '"
			   << m_sFileName << "'.  System Error Number " 
			   << errno << ".";
		

		LogError(KORC_WRITE_TO_OUTPUT_FAIL, ll_return,
				ls_msg,
	  			"CLog::WriteRecord:2",
  				is_InstanceName.getPointer(),
				LOGCONSOLE);

	}  /* End If */

	return ll_return;
}  /* End Method */






//---------------------------------------------------------------------+
//  Method:   WriteRecord
//  Author:   David M Tamkun
//  Written:  December 1998
//
//  Desc:     Writes specifed CString to Output File
//
//  Args:     I CString	Info to Write
//
//  Returns:  long	0 if no errors encountered.
//                  2 if an error occurrs.
//  
//  Dev Log: 
//  History: 
//  12/31/98 DMT Created original version.
//---------------------------------------------------------------------+
long CLog::WriteRecord (CString& rs_record) {

	return WriteRecord(rs_record.getPointer());

}  /* End Method */