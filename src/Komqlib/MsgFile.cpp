//---------------------------------------------------------------------+
//  File:     CMsgFile.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CMsgFile
//            Class.
//
//            This class represents a file that can be used as an
//            input for Put programs and an Output for Get Programs.
//  
//  Dev Log:  This file uses the Log Class, but the Log Class is not
//            a specialization of this class.  I tried this 
//            briefly, but got into a "chicken and egg" situation
//            because neither class was complete without the other. I 
//            thought I knew how to get around this with a generic 
//            forward "class" declaration, but it didn't work.  
//
//            Consider doing this in the future (making CLog a 
//            specialization of CMsgFile).  Also, the Parameter File
//            classes do not use CMsgFile because they were developed
//            by someone else at a different time.  This logic 
//            could be integrated in the future.
//
//  History: 
//  03/12/99 DMT Created original version.
//  09/18/00 DRC Renamed CFile to CMsgFile.
//           DRC Isolated the reusable logic from this class into a 
//               new class called CFile and adjusted this class to 
//               inherit from CFile.
//---------------------------------------------------------------------+
#include "MsgFile.h"



//---------------------------------------------------------------------+
//  Method:   Advance
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Reads through file by the specified # of lines for 
//            restart purposes.  Future reads or writes begin after
//            the number or lines specified.
//
//            Logs "Advanced" info to the Log Class.
//
//  Args:     ref to CBuffer class - provides buffer for each line read.
// 
//            long vl_lines - Number of lines to skip.
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CMsgFile::Advance(CBuffer& cBuffer, long vl_lines) {

	long	ll_return = KOCC_GOOD;
	long	ll_counter = 0;
	CString	ls_msg;


	ll_return = CFile::Advance(cBuffer, vl_lines);

    if ((ll_return == KOCC_FAIL) 
	&&  (m_iLastCC == CFILE_ERR_REPOSITION_FAILED)){
		ls_msg << "Could not position data file '"
			   << m_szFileName
			   << "' to the restart point.";

		m_pcLog->LogError(KORC_RESTART_REPOSITION_FAILED, ll_return,
						ls_msg,
						"CMsgFile::Advance:1",
						is_InstanceName.getPointer());
	}
	else {
		ls_msg << "-- Restarting data file "
			   << m_szFileName 
			   << " at record/message " 
			   << vl_lines
			   << ".";

		m_pcLog->WriteLog(ls_msg, LOGCONSOLE + LOGFILE);

	}
	return ll_return;
}




//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/18/00 DRC Modified to inherit from CFile.
//---------------------------------------------------------------------+
CMsgFile::CMsgFile() : CFile() {
	m_pcLog			= NULL;
	is_InstanceName	= "MsgFile";
}







//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
CMsgFile::~CMsgFile() {
//	termSelf();
}

	



//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/18/00 DRC Modified to inherit from CFile.
//---------------------------------------------------------------------+
long CMsgFile::initSelf(CLog* v_pcLog, char* szFileName) {

	long ll_return = KOCC_GOOD;
	
	m_pcLog	  = v_pcLog;
	ll_return = CAny2::initSelf();

	if (ll_return ==  KOCC_GOOD) {
		ll_return = CFile::initSelf(szFileName);
	} /* End If */

	return ll_return;
}







//---------------------------------------------------------------------+
//  Method:   Open
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Opens the file
//
//  Args:     int	Open Mode
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CMsgFile::Open(int vi_mode) {

	long    ll_return = KOCC_GOOD;
	CString	ls_msg;
	CString ls_errno_desc;

    ll_return = CFile::Open(vi_mode);

    if (ll_return == KOCC_FAIL){
		m_pcLog->TranslateErrno(ls_errno_desc, errno);
		
		ls_msg << "File '" << m_szFileName << "' did not open!  "
			   << ls_errno_desc;

		m_pcLog->LogError(KORC_FILE_OPEN_ERROR, ll_return,
							ls_msg, 
							"CMsgFile::Open:1",
							is_InstanceName.getPointer());
	} /* End If */

	return ll_return;
} /* End Open */













//---------------------------------------------------------------------+
//  Method:   Read
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Reads line of data from file.  
//
//  Args:     CBuffer
//
//  Returns:  KOCC_WARNING - if EOF
//
//            KOCC_FAIL    - if error 
//
//                         - if the last line in the file does
//                           not end in CRLF
//
//                         - if width of line exceeds buffer size
//
//            KOCC_GOOD    - if read line of data.
//  
//  Dev Log:  To allow this to accept the last line of a file if it
//            doesn't end in CRLF, see the comments below.
//                 
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CMsgFile::Read(CBuffer& cBuffer) {

	long	ll_return = KOCC_GOOD;
	long	ll_strlen = 0;
	CString	ls_msg;
	CString	ls_errno_desc;


    ll_return = CFile::Read(cBuffer);

	//-----------------------------------------------------------------+
	// Did the read fail for some reason other than eof()?
	//-----------------------------------------------------------------+
    if ((ll_return == KOCC_FAIL) 
	&&  (m_iLastCC == CFILE_ERR_READ_FAILED)){

		m_pcLog->TranslateErrno(ls_errno_desc, errno);

		ls_msg << "Error while Reading from file '"
			   << m_szFileName
			   << "'.  "
			   << ls_errno_desc;

		m_pcLog->LogError(KORC_FILE_READ_ERROR, ll_return,
			ls_msg,
		    "CMsgFile::Read:1",
  			is_InstanceName.getPointer());		
	}




	//-----------------------------------------------------------------+
	// Did we just read a line of data that's wider than our Buffer?
	//-----------------------------------------------------------------+ 
    if ((ll_return == KOCC_FAIL) 
	&&  (m_iLastCC == CFILE_ERR_READ_TRUNCATED)){

		ls_msg << "A line in "
			   << m_szFileName
			   << " is wider than the buffer size of "
			   << cBuffer.getSize() - 1 << " bytes.";

		m_pcLog->LogError(KORC_LINE_WIDTH_EXCEEDS_BUFFER, ll_return,
						ls_msg,
						"CMsgFile::Read:1",
						is_InstanceName.getPointer());
	}




	//-----------------------------------------------------------------+
	// If not one of the errors above, did the last line of data
	// not ended by a CRLF?  
	//-----------------------------------------------------------------+
    if ((ll_return == KOCC_FAIL) 
	&&  (m_iLastCC == CFILE_ERR_LINE_TERMINATOR_MISSING)){
		ls_msg << "The last line in "
			   << m_szFileName
			   << " does not end with a <return> character.";

		m_pcLog->LogError(KORC_LINE_ENDS_AT_END_OF_FILE, ll_return,
						ls_msg,
						"CMsgFile::Read:2",
						is_InstanceName.getPointer());
	}




	//-----------------------------------------------------------------+
	// To accept the the last line of a file if it doesn't end in 
	// CRLF, uncomment out gcount() == 0 condition below and 
	// comment out the error above.
	//-----------------------------------------------------------------+
    if ((ll_return == KOCC_WARNING) 
	&&  (m_iLastCC == CFILE_EOF)){
		//ls_msg << "-- At end of file '" << m_szFileName << "'.";
		//m_pcLog->WriteLog(ls_msg, LOGDEFAULT);
	}
	

	return ll_return;
}









//---------------------------------------------------------------------+
//  Method:   Remove
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Calls the Close method to ensure the files closed and then
//            deletes the file.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CMsgFile::Remove() {
	
	long	ll_return = KOCC_GOOD;
	CString	ls_msg;

	ll_return = CFile::Remove();

	if ((ll_return == KOCC_WARNING) 
	&&  (m_iLastCC == CFILE_ERR_REMOVE_FAILED)){
		ls_msg << "Cannot Remove File because filename was never set.";

		m_pcLog->LogError(KORC_FILE_NAME_NOT_PROVIDED, ll_return,
			              ls_msg,
						  "CMsgFile::Remove:1",
						  is_InstanceName.getPointer());
	}

	return ll_return;	
}









//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     This method is necessary to avoid the ambiguity created 
//            by dual inheritance.  If this method is removed there 
//            is no way to determine the routing of termSelf messages.
//            They could be intended for either CFile or CAny2.
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/18/00 DRC Modified to inherit from CFile.
//---------------------------------------------------------------------+
long CMsgFile::termSelf() {
return CFile::termSelf();
} /* End termSelf */

	
	
	
	
//---------------------------------------------------------------------+
//  Method:   Write
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Writes the contents of CBuffer to the file and appends
//            and end-of-line character.  The buffer contents becomes
//            the next line in the file.
//
//  Args:     ref to CBuffer class
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CMsgFile::Write(CBuffer& cBuffer) {

	long	ll_return = KOCC_GOOD;
	CString	ls_msg;
	CString ls_errno_desc;

	ll_return = CFile::Write(cBuffer);

    if ((ll_return == KOCC_FAIL) 
	&&  (m_iLastCC == CFILE_ERR_WRITE_FAILED)){
		m_pcLog->TranslateErrno(ls_errno_desc, errno);

		ls_msg << "Error Writing Data to file '" 
			   << m_szFileName 
			   << "'.  "
			   << ls_errno_desc;

		m_pcLog->LogError(KORC_FILE_WRITE_ERROR, ll_return,
							ls_msg, 
							"CMsgFile::Write:1",
							is_InstanceName.getPointer());
		
	}

	return ll_return;
}















