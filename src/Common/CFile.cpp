//---------------------------------------------------------------------+
//  File:     CFile.cpp
//  Author:   David M. Tamkun
//  Written:  March 1999
//  Desc:     This file contains the implementation of the CFile
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
//            specialization of CFile).  Also, the Parameter File
//            classes do not use CFile because they were developed
//            by someone else at a different time.  This logic 
//            could be integrated in the future.
//
//  01)  Fixed length blocking is working fine but there is a bug 
//       that is causing bytes to be dropped at the end of blocks 
//       in variable length blocking.          D. Creager 11/2000
//
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Reduced this class to make it more general and 
//               therefore more reusable.  Eliminating any references 
//               the CLog class.
// 
//  09/15/00 DRC Added support for Blocked I/O.
// 
//               Note:  Blocked I/O assumes fixed length records.
// 
//               Blocked input means that multiple records are read 
//               into the buffer until there is no more space for a 
//               whole record.  
//
//               Blocked output means that multiple records are written
//               from a single buffer until it can contain no more whole 
//               records.
//  11/10/00 DRC Added support for Variable Length Blocking.
//
//               The Unix style record/block delimiter (LF) is used
//               within the buffer's internal representation.   
//---------------------------------------------------------------------+
#include "CFile.h"


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
//  09/15/00 DRC Removed all CLog references
//  02/21/01 DRC Added a second constructor to support the direct 
//               construction of fstream in decendants as part of 
//               supporting is_open in the AIX environment.
//               Ref: CParamFile::CParamFile(char*, int);
//---------------------------------------------------------------------+
CFile::CFile() : fstream() {
initSelf();
} /* End CFile */


CFile::CFile(  char* a_szName, int a_iMode) 
: fstream(a_szName, a_iMode){ 
initSelf(a_szName);
} /* end CFile */



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
CFile::~CFile() {} /* End ~CFile */

	



//---------------------------------------------------------------------+
//  Method:   getMaxRcrdLen
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Calculate and set the longest record length
//            currently in this file.
//
//  Args:     None
//
//  Returns:  long(Longest file record length)
//            Negative values indicate error conditions
//            -1     =  Open Failed
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created Original code.
//---------------------------------------------------------------------+
long CFile::getMaxRcrdLen(){

long l_RC = KOCC_GOOD;
CBuffer	l_Buf;

if (m_lMaxRcrdLen > 0) return m_lMaxRcrdLen;

//
//  Init program for a full file scan to set the value.
//
this->Open(ios::in | ios::nocreate);

#ifndef _WIN32
	if (!rdbuf()->is_open()) return -1;
#else
	if (!is_open()) return -1;
#endif


//
// Prep the I/O Loop
//
l_Buf.initSelf(2 mb);
l_RC = this->Read(l_Buf, false);

//
// I/O Loop: Browse the file for largest record
//
while (l_RC == KOCC_GOOD){
	if (m_lMaxRcrdLen < l_Buf.getDataLen()) 
		m_lMaxRcrdLen = l_Buf.getDataLen();
	l_RC = this->Read(l_Buf, false);
} /* End While */

this->Close();

return m_lMaxRcrdLen;
} /* End getMaxRcrdLen */





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
//  09/15/00 DRC Removed all CLog and CAny2 references
//  02/21/01 DRC Added second initSelf for flexability in constructors.
//---------------------------------------------------------------------+
void CFile::initSelf() {

m_iLastCC		= 0;
m_iOpenMode     = 0;
m_lMaxRcrdLen   = 0;
m_lRcrdCnt      = 0;
m_lBlkCnt       = 0;
m_bOpen			= FALSE;
m_bWasOpen      = FALSE;
m_bWritten      = FALSE;
m_szFileName.fill();

} /* End initSelf */




long CFile::initSelf(char* szFileName) {
	long ll_return = KOCC_GOOD;
	initSelf();
	m_szFileName = szFileName;
	return ll_return;
} /* End initSelf */






//---------------------------------------------------------------------+
//  Method:   GetName
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
const char*  CFile::GetName() { 
	return m_szFileName.getPointer(); 
} /* End GetName */





//---------------------------------------------------------------------+
//  Method:   getBlkCount
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CFile::getBlkCount(){
return m_lBlkCnt;
} /* end getBlkCount */





//---------------------------------------------------------------------+
//  Method:   getLastCC
//  Author:   Daniel R Creager
//  Written:  May 2002
//  
//  Dev Log:  
//  History: 
//  05/08/02 DRC Created original version.
//---------------------------------------------------------------------+
int CFile::getLastCC() {
return m_iLastCC;
} // end getLastCC



//---------------------------------------------------------------------+
//  Method:   getRcrdCount
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CFile::getRcrdCount(){
return m_lRcrdCnt;
} /* end getRcrdCount */





//---------------------------------------------------------------------+
//  Method:   Read
//  Author:   Daniel R Creager
//  Written:  April 2002
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
//                 
//  History: 
//  01/16/01 DRC Created original version for backward compatabililty.
//---------------------------------------------------------------------+
long CFile::Read(CBuffer& cBuffer) {

return Read(cBuffer, true);
} /* End Read */



//---------------------------------------------------------------------+
//  Method:   Read
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Reads line of data from file.  
//
//  Args:     CBuffer
//            ClrBufInd True = Clear the previous buffer contents. 
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
//  01)  Need to handle the condition if the blksize < rcrdLen. DRC
//                 
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Removed all CLog references
//  01/16/01 DRC Corrected incorrect calculation of final record length
//  04/02/02 DRC Added a parameter to control clearing the buffer as a 
//               performance boost when scanning records solely for len.
//---------------------------------------------------------------------+
long CFile::Read(CBuffer& cBuffer, BOOL a_ClrBufInd) {

	long	ll_return = KOCC_GOOD;
	long	ll_strlen = 0;
	long    ll_cursor = 0;

	if (a_ClrBufInd) cBuffer.clear();
	if (good()){
		if (cBuffer.isBlocked()){
			//---------------------------------------------------------+
			// On successful read with input blocking of fixed length
			//    records specified, fill the buffer with whole, fixed
			//    length records.
			//---------------------------------------------------------+
			if (cBuffer.isFixed()){
				getline(cBuffer.getPointer(), cBuffer.getSize());
				while (good() && (!cBuffer.isFilled())){
					cBuffer.setDataLen(cBuffer.getDataLen() + gcount() - 1);
					cBuffer.RFill(' ');
                    m_lRcrdCnt   += 1;
					if (!cBuffer.isFilled()){
						getline(cBuffer.getCursor(), cBuffer.getAvail());
					} /* end if */
				} /* End While */
				//
				//  Add in the length of the final block
				//
				if (eof() && (gcount() > 0)) {
					//cBuffer.setDataLen(cBuffer.getDataLen() + gcount() - 1);
					cBuffer.setDataLen(cBuffer.getDataLen() + gcount());
					cBuffer.RFill(' ');
                    m_lRcrdCnt   += 1;
				} /* End If */
			}
			else {
			//---------------------------------------------------------+
			// On successful read with input blocking of variable 
			//    length records specified:
			//       Fill the buffer 
			//       Convert Internal Buffer Delimiters
			//---------------------------------------------------------+
				while (good() && (!cBuffer.isFilled())){
					read(cBuffer.getCursor(), cBuffer.getAvail());
					if (!eof()){
						cBuffer.setDataLen(cBuffer.getDataLen() + gcount() - 1);
						cBuffer.CnvrtDlmtrs();
					} /* End If */
				} /* End While */
			} /* End If */
		}
		else{
			getline(cBuffer.getPointer(), cBuffer.getSize());
			m_lRcrdCnt   += 1;
			if (!eof()) cBuffer.setDataLen(gcount() - 1);
		}/* End If */
    } /* End If */



	//-----------------------------------------------------------------+
	// Did the read fail for some reason other than eof()?
	//-----------------------------------------------------------------+
	if (!good() && !eof()) {
		ll_return  = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_READ_FAILED;
	}




	//-----------------------------------------------------------------+
	// Did we just read a line of data that's wider than our Buffer?
	//
	// gcount() always returns # chars read plus 1 byte for "/n",
	// unless the line is wider than the buffer (then gcount = 
	// buffer size = strlen of what was read).
	//
	// Note that we check against BufferSize - 1 because we added 
	// 1 to what user specified as a Buffer when we initialized
	// cBuffer.
	//-----------------------------------------------------------------+
	ll_strlen = strlen(cBuffer.getPointer());

	if((gcount()              == cBuffer.getSize() - 1) &&
	   (cBuffer.getSize() - 1 == ll_strlen           )) {
		ll_return = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_READ_TRUNCATED;
	}




	//-----------------------------------------------------------------+
	// If not one of the errors above, did the last line of data
	// not ended by a CRLF?  
	//
	// This doesn't have to be an error, but making it so to be 
	// consistent with previous versions of this program.
	//
	// To accept the last line of a file that may or may not end in
	// CRLF, comment out this error and uncomment the gcount() == 0
	// check in the condition below.
	//-----------------------------------------------------------------+
	if ((ll_return == KOCC_GOOD) &&
		(gcount()  >  0        ) &&
		 eof()) {
		ll_return  = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_LINE_TERMINATOR_MISSING;
	}




	//-----------------------------------------------------------------+
	// If not one of the errors above and we hit the end of the file,
	// set return code to KOCC_WARNING so caller will know it's time
	// to stop.
	//
	// To accept the the last line of a file if it doesn't end in 
	// CRLF, uncomment out gcount() == 0 condition below and 
	// comment out the error above.
	//-----------------------------------------------------------------+
	if (eof() && (ll_return == KOCC_GOOD) /* && gcount() == 0 */) {
		ll_return = KOCC_WARNING;
		m_iLastCC = CFILE_EOF;
	}
	
    m_lBlkCnt   += 1;
	return ll_return;
} /* End Read */




//---------------------------------------------------------------------+
//  Method:   setRcrdCount
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  TBD
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CFile::setRcrdCount(){
CBuffer l_Buf;

l_Buf.initSelf(4 kb);
this->Open(ios::in && ios::nocreate);
while (!eof()) this->Read(l_Buf);
this->Close();
l_Buf.termSelf();
return getRcrdCount();
} /* end getRcrdCount */










//---------------------------------------------------------------------+
//  Method:   Write
//  Author:   Daniel R Creager
//  Written:  April 2002
//
//  Desc:     Writes the contents of CBuffer to the file and appends
//            and end-of-line character.  The buffer contents becomes
//            the next line in the file.
//
//  Args:     ref to CBuffer class
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
//                 
//  History:  
//  03/12/99 DMT Created original version.
//  01/16/01 DRC Created original version for backward compatabililty.
//---------------------------------------------------------------------+
long CFile::Write(CBuffer& cBuffer) {

return Write(cBuffer, true);
} /* End Write */




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
//  01)  Need to handle the condition if the blksize < rcrdLen. DRC
//
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Removed all CLog references
//---------------------------------------------------------------------+
long CFile::Write(CBuffer& cBuffer, BOOL a_ClrBufInd) {

	long	ll_return = KOCC_GOOD;

	m_bWritten = TRUE;

	try {
	//-----------------------------------------------------------------+
	// On blocked output specified, 
	//    On Fixed Length Records 
	//       write each logical record separately  
	//       stripping off trailing spaces
	//    On Variable Length Records
	//       Convert Internal Buffer Delimiters
	//       write the entire buffer 
	//       stripping off the buffer delimiter (CRLF)
	//-----------------------------------------------------------------+
		if (cBuffer.isBlocked()){ 
			if(cBuffer.isFixed()){
				cBuffer.getFirst();
				while (!cBuffer.isEndOfBuffer()){
					*this << cBuffer.RTrim(cBuffer.getCursor()) << endl;
			        m_lRcrdCnt   += 1;
            		cBuffer.next();
				} /* End While */
				*this << cBuffer.RTrim(cBuffer.getCursor()) << endl;
		        m_lRcrdCnt   += 1;
        		if (a_ClrBufInd) cBuffer.clear();
			}
			else{
				cBuffer.CnvrtDlmtrs();
				write(cBuffer.getPointer(), (int) cBuffer.getDataLen());
                m_lRcrdCnt   += 1;
			} /* End If */
		}
		else {
			*this << cBuffer.getPointer() << endl;
            m_lRcrdCnt   += 1;
		} /* End If */
	} /* End try */

	catch (...) {
		ll_return = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_WRITE_FAILED;
	}
    m_lBlkCnt   += 1;
	return ll_return;
} /* End Write */








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
//  09/15/00 DRC Removed all CLog references
//---------------------------------------------------------------------+
long CFile::Advance(CBuffer& cBuffer, long vl_lines) {

	long	ll_return = KOCC_GOOD;
	long	ll_counter = 0;


	for (ll_counter = 0; 
	     (ll_counter < vl_lines) && (ll_return == KOCC_GOOD); 
		 ++ll_counter) {

		ll_return = Read(cBuffer);
	}

	if (ll_counter != vl_lines) {

		ll_return = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_REPOSITION_FAILED;
	}
	return ll_return;
} /* End Advance */






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
//  09/15/00 DRC Removed all CLog references
//---------------------------------------------------------------------+
long CFile::Open(int vi_mode) {

	long    ll_return = KOCC_GOOD;

	m_iOpenMode = vi_mode;
	m_bWritten  = FALSE;

	open(m_szFileName.getPointer(), (ios::open_mode) vi_mode);

#ifndef _WIN32
	if (!rdbuf()->is_open()) {
#else
	if (!is_open()) {
#endif

		ll_return  = KOCC_FAIL;
		m_iLastCC = CFILE_ERR_OPEN_FAILED;
	}
	else {
		m_bWasOpen = TRUE;
		m_bOpen    = TRUE;
	}

	return ll_return;
} /* End Open */








//---------------------------------------------------------------------+
//  Method:   Close
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes the file if it's currently open.  Still returns
//            good if the file is already closed.
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
long CFile::Close() {

	long	ll_return = KOCC_GOOD;

#ifndef _WIN32
	if (rdbuf()->is_open()) {
#else
	if (is_open()) {
#endif
		close();
		m_bOpen		= FALSE;
	}

	return ll_return;
} /* End Close */










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
//  09/15/00 DRC Removed all CLog references
//---------------------------------------------------------------------+
long CFile::Remove() {
	
	long	ll_return = KOCC_GOOD;

	if (m_szFileName.vlen() > 0) {
		Close();
		remove(m_szFileName.getPointer());
	}
	else {
		// can't remove because filename was never set.
		ll_return = KOCC_WARNING;
		m_iLastCC = CFILE_ERR_REMOVE_FAILED;
	}

	return ll_return;	
} /* End Remove */




//---------------------------------------------------------------------+
//  Method:   termSelf
//  Author:   David M. Tamkun
//  Written:  March 1999
//
//  Desc:     Closes the file if it is open.  If File was opened
//            at some point for output and no-replace, but we
//            never wrote to it, the 0-byte file created at open time
//            is deleted.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful.
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//  09/15/00 DRC Removed all CLog and CAny2 references
//---------------------------------------------------------------------+
long CFile::termSelf() {

	long ll_return = KOCC_GOOD;


	if ((m_iOpenMode == (ios::out | ios::noreplace)) &&
		(!m_bWritten) && (m_bWasOpen)) {

		Remove();
	}
	else {
		Close();
	}


	return ll_return;
} /* End termSelf */



//---------------------------------------------------------------------+
//  Method:   <<  (Insertion Operator)
//  Desc:     TBD. 
//---------------------------------------------------------------------+
ostream&  operator<< ( ostream& au_stream, CFile& au_arg1){
	au_stream << au_arg1.GetName();
	return au_stream;
}


