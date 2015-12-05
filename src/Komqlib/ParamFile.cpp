/*
 *  Copyright 1999, The Coca-Cola Company  All Rights Reserved.
 *
 *  Reproduction or use of this file without express written consent
 *  is prohibited.
 *
 *  SCCS Information:
 *  Date: 99/01/29	@(#) ParamFile.C 1.2@(#)
 */
//---------------------------------------------------------------------+
//  File:     ParamFile.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//  Desc:     This file contains the implementation of the CParamFile
//            Class.
//
//            This is a generic class derived from fstream and CAny2.
//            It does not know about the log file, so callers 
//            have to get ParamFile errors out of it's Errors 
//            Collection.
//
//            Descendents of this class define a static table that
//            describes the specific parameters in a parameter file.
//            Descendents also have member variables for storing
//            each parameter.
//
//            This ancestor contains the parameter file processing
//            logic.
//  
//  Dev Log:  Consider making this a specialization of CFile and 
//            provide CLog access.  Will have to address
//            "chicken and egg" issues.
//
//            Note that TranslateErrno is duplicated here and CLog.
//            see TranslateErrno doc-block for additional info.
//
//  History: 
//  01/15/99 CAH Created original version.
//  02/19/99 DMT Added standard comment blocks.
//---------------------------------------------------------------------+
#include "ParamFile.h"



//---------------------------------------------------------------------+
//---------------------------------------------------------------------+
// Note Limit on Line Width of Parameter File - max is 1024 bytes.
//---------------------------------------------------------------------+
//---------------------------------------------------------------------+
const int CParamFile::m_iReadMax = 1024;





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Doesn't initialize anything, other than setting members 
//            to NULL.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CParamFile::CParamFile() : CFile()
{
	is_InstanceName = "Parameter File Ancestor";
	m_szFileName = NULL;
	m_szReadBuf  = NULL;
	m_szLastError= NULL;
}





//---------------------------------------------------------------------+
//  Method:   Constructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Opens Parameter File on Construction - 
//            Throws Exception if the File Doesn't Exist.
//
//  Args:     char* FileName
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CParamFile::CParamFile(const char *name) 
           : CFile ((char*) name, ios::in | ios::nocreate)
{
	CString	ls_msg;
	int		li_error = 0;
	CString	ls_errno_desc;

	is_InstanceName = "Parameter File Ancestor";

	m_szReadBuf  = NULL;
	m_szLastError= NULL;
	
	try {
		
		m_szFileName = new char[strlen(name)+1];
		strcpy((char*)m_szFileName,name);

	
		if (!good())
		{
			TranslateErrno(ls_errno_desc, errno);

			ls_msg << "Failed in constructor.  Cannot open parameter file '"
				   << (char*) name << "'.  " 
				   << ls_errno_desc;

			iu_errors.AddError(KORC_OPEN_INPUT_FAIL, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Constructor:1", 
				is_InstanceName.getPointer());

			li_error = 1;

			throw li_error;
				   
		}
		close();
	}

	catch (int) {
		// do nothing, error already added to the collection
	}

	catch (...) {
		// something unexpected happened.
		ls_msg << "Unexpected Error!!";

		iu_errors.AddError(KORC_UNSPECIFIED, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Constructor:2", 
				is_InstanceName.getPointer());
	}
}







//---------------------------------------------------------------------+
//  Method:   initSelf
//  Author:   David Tamkun
//  Written:  February? 1999
//
//  Desc:     Allocates memory for File Name member variable and sets
//            its value.
//
//  Args:     char* FileName - name of parameter file.
//
//  Returns:  KOCC_GOOD if successful, 
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  02/??/99 DMT Created original version.
//---------------------------------------------------------------------+
long CParamFile::initSelf(char* szFileName) {

	long ll_return = KOCC_GOOD;


	ll_return = CAny::initSelf();

	if (ll_return == KOCC_GOOD) {

		try {
			m_szFileName = new char[strlen(szFileName)+1];

			strcpy((char*)m_szFileName, szFileName);
		}

		catch(...) {

			ll_return = KOCC_FAIL;

			iu_errors.AddError(KORC_MEMORY_ALLOC_FAIL, ll_return, 
				"Error Allocating Memory!",
				"CParamFile::initSelf:1", 
				is_InstanceName.getPointer());
		}

	}


	return ll_return;

}






//---------------------------------------------------------------------+
//  Method:   Open
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     
//            All reads of a param file are done as a part of
//            an atonomous unit of work; 
//                that is file open; get stuff; file close.
//
//            Therefore opening the file here is simply a verification 
//            that the file exists or doesn't exist, depending on the
//            specified open mode.
//
//  Args:     OpenMode
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CParamFile::Open(const int mode)
{

	CString		ls_msg;
	long		ll_return = KOCC_GOOD;
	CString		ls_errno_desc;


	if (NULL == m_szFileName) {

		ll_return = KOCC_FAIL;

		ls_msg << "No filename provided.  Cannot open parameter file.";				   

		iu_errors.AddError(KORC_OPEN_INPUT_FAIL, ll_return, 
			ls_msg,
			"CParamFile::Open:1",
			is_InstanceName.getPointer());

	}

	if (ll_return == KOCC_GOOD) {
		
		open(m_szFileName, (ios::open_mode)mode);

	}

#ifndef _WIN32
	if (rdbuf()->is_open()) {
#else
	if (is_open()) {
#endif

		close();

	}
	else {

		ll_return = KOCC_FAIL;

		TranslateErrno(ls_errno_desc, errno);

		ls_msg << "Cannot open parameter file '"
			   << m_szFileName << "'.  " 
			   << ls_errno_desc;

		iu_errors.AddError(KORC_OPEN_INPUT_FAIL, ll_return, 
			ls_msg,
			"CParamFile::Open:2", 
			is_InstanceName.getPointer());

	}

	return ll_return;
}





//---------------------------------------------------------------------+
//  Method:   Open
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     All reads of a param file are done as a part of
//            an atonomous unit of work; that is file open; get stuff; 
//            file close.  Therefore opening the file here is simply a 
//            verification that the file exists.
//
//  Args:     char* filename, 
//            int	openmode
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
long CParamFile::Open(const char* filename, const int mode)
{

	long ll_return	= KOCC_GOOD;
	//
	// All reads of a param file are done as a part of
	// an atonomous unit of work; that is file open; get stuff; file close.
	// Therefore opening the file here is simply a verification that the
	// file exists.
	//
	if (NULL != m_szFileName)
		delete[] m_szFileName;

	m_szFileName = new char[strlen(filename)+1];
	strcpy(m_szFileName, filename);

	ll_return = Open((ios::open_mode)mode);

	return ll_return;
}





//---------------------------------------------------------------------+
//  Method:   Destructor
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Deletes Dynamically allocated member variables.
//
//            Descendent Destructor should've deleted "String" 
//            member variables that point to dynamically allocated
//            blocks of memory.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
CParamFile::~CParamFile()
{
	if(m_szFileName != NULL) {
		delete[] m_szFileName;
		m_szFileName = NULL;
	}

	if (m_szReadBuf != NULL) {
		delete[] m_szReadBuf;
		m_szReadBuf = NULL;
	}

	
	

}





//---------------------------------------------------------------------+
//  Method:   Load
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Loads File values into Parameter Table.
//            Actual Parameter Table for a parameter table is declared
//            in the descendent.
//
//            File is read, for each line, we spin through the 
//            parameter table looking for a match.
//
//  Args:     None
//
//  Returns:  KOCC_GOOD if successful,
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/19/99 DMT Changed code if no value for string parm.
//               Was throwing an exception accessing NULL pointer
//               Also, moved "loaded" code.  If we don't have a value,
//               ie, keyword there, but no parm.  Don't consider it
//               loaded.
//  04/28/99 DMT Allowing space in parms for long Win32 filenames.
//---------------------------------------------------------------------+
long CParamFile::Load()
{
	SParamTable*	lpPt;
	char*			szArgP;
	CString			ls_msg;
	int				li_error = 0;
	long			ll_return = KOCC_GOOD;



	//-----------------------------------------------------------------+
	// Get Decendent's Parmeter Table
	//-----------------------------------------------------------------+
	const SParamTable*	sParamTable = GetParamTable();


	try {

	if (NULL == sParamTable) {
	
		ls_msg << "Internal Error.  No Parameter Table.";

		iu_errors.AddError(KORC_INTERNAL_ERROR, KOCC_FAIL, ls_msg,
			"CParamFile::Load:1", is_InstanceName.getPointer());

		li_error = 1;

		throw li_error;

		//throw CExcBadParamTable(*this);
	}


	open(m_szFileName, ios::in|ios::nocreate);


	//-----------------------------------------------------------------+
	// Allocate Read Buffer that's a member of this class if it's
	// currently NULL.  The "member" read buffer can be accessed by
	// the exception handler and can display the line in error, if
	// needed.
	//-----------------------------------------------------------------+
	if (NULL == m_szReadBuf)
		m_szReadBuf =  new char[m_iReadMax+1];


	//-----------------------------------------------------------------+
	// Allocate a locally declared Read Buffer.  As we use strtok, this
	// will get chopped up, so we wouldn't want to use this one to 
	// report error messages.
	//
	// auto_ptr deletes char array when ptr goes out of scope.
	//-----------------------------------------------------------------+
	auto_ptr<char> szReadBuf = new char[m_iReadMax+1];



	//-----------------------------------------------------------------+
	// Read Parameter File Line-By-Line into the "member" read 
	// buffer.
	//-----------------------------------------------------------------+
	while (getline(m_szReadBuf, m_iReadMax) && good())
	{
		// ignore lines beginning with ';'
		if (';' == m_szReadBuf[0] )
			continue;

		// copy the line from the member read buffer into the local
		// read buffer.
		strncpy(szReadBuf, m_szReadBuf, m_iReadMax);

		// Get Pointer to KeyWord
		szArgP = strtok(szReadBuf,"=\0");


		if ( NULL == szArgP )
		{
			ls_msg << "Could not interpret line in parameter file: '" 
				   << m_szReadBuf << "'.  Parameter file "
				   << m_szFileName << ".";

			iu_errors.AddError(KORC_PARM_SYNTAX_ERROR, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Load:2", 
				is_InstanceName.getPointer());

			li_error = 2;

			throw li_error;

			//throw CExcBadParam ("Could not understand line.", *this);
		}


		//-------------------------------------------------------------+
		// FOR loop with no lines in it.
		// 
		// Spin through the Parameter Table until we get to the
		// "TypeInvalid" entry signaling the end of the table, or until
		// the keyword in the table matches the keyword we just
		// retrieved.
		//-------------------------------------------------------------+
		for (lpPt = (SParamTable*)sParamTable; 

		     lpPt->eParamType != TypeInvalid && 
			 0 != strcmp(lpPt->szParamName, szArgP); 

			 lpPt++);


		// If we got to TypeInvalid, we have an undefined keyword.
		if (TypeInvalid == lpPt->eParamType)
		{
			ls_msg << "Unrecognized Parameter '"
				   << m_szReadBuf << "' in parameter file "
				   << m_szFileName << ".";

			iu_errors.AddError(KORC_PARM_SYNTAX_ERROR, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Load:3", 
				is_InstanceName.getPointer());

			li_error = 3;

			throw li_error;
			//throw CExcBadParam("Invalid parameter in file.", *this);
		}


		//-------------------------------------------------------------+
		// Get the Parameter Value.
		// 
		// Parm Value can be ended by a space, a ; or NULL
		//-------------------------------------------------------------+
		//szArgP = strtok(NULL, " ;\0");// DMT 4/28/99
		szArgP = strtok(NULL, ";\0");	// DMT Allowing space in parms 
										// for long Win32 filenames.

		rtrim(szArgP);	// DMT 4/28 remove trailing spaces.


		//-------------------------------------------------------------+
		// Process Parameter Based on it's Type in the Parameter Table.
		//-------------------------------------------------------------+
		switch ( lpPt->eParamType )
		{

			//---------------------------------------------------------+
			// Is it a string?
			//---------------------------------------------------------+
			case TypeString:
			{
				// declare pointer to pointer of chars so we can
				// easily get to the parm value in the table.
				char** szParamStore = (char **)lpPt->paramStore;


				SetString(szParamStore, szArgP);


				// if the parameter value from the file is NULL
				if (NULL != szArgP)
				{
					lpPt->bLoaded = TRUE;	// DMT 02/19/99
				}
			}
			break;

			//---------------------------------------------------------+
			// Is it a Long?
			//---------------------------------------------------------+
			case TypeLong:
			{
				if (TRUE != isdec(szArgP))
				{
					ls_msg << "Expected numeric value '"
						   << m_szReadBuf  << "' not numeric in "
						   << "parameter file " << m_szFileName << ".";

					iu_errors.AddError(KORC_NONNUMERIC_PARM, KOCC_FAIL, 
						ls_msg,
						"CParamFile::Load:4", 
						is_InstanceName.getPointer());

					li_error = 4;

					throw li_error;
				}

				ulong* lParamStore = (ulong*)lpPt->paramStore;

				if (NULL == szArgP)
				{
					*lParamStore = 0L;
				}
				else
				{
					*lParamStore = atol(szArgP);
					lpPt->bLoaded = TRUE;	// DMT 02/19/99
				}
			}
			break;
		}

	}	// end while

	close();


	//
	// Check to insure all required parameters have been loaded.
	// The ParamTable is used for this as well.
	//
	BOOL bParamNotFound = FALSE;

	for (lpPt = (SParamTable*)sParamTable; lpPt->eParamType != TypeInvalid; lpPt++)
	{
		if (TRUE == lpPt->bRequired && lpPt->bLoaded == FALSE)
		{

			ls_msg = "";

			ls_msg << "Required parameter '" << (char*) lpPt->szParamName
				   << "' not present in parameter file " 
				   << m_szFileName << ".";

			iu_errors.AddError(KORC_MISSING_PARMS, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Load:5", 
				is_InstanceName.getPointer());

			bParamNotFound = TRUE;
		}
	}


	} // end try


	catch (int) {

		close();
	}


	catch (...) {
		// something unexpected happened.
		ls_msg << "Unexpected Error!!";

		iu_errors.AddError(KORC_UNSPECIFIED, KOCC_FAIL, 
				ls_msg,
				"CParamFile::Load:6", 
				is_InstanceName.getPointer());
	}


	if (iu_errors.HasError()) {
		ll_return = KOCC_FAIL;
	}

	return ll_return;

}





//---------------------------------------------------------------------+
//  Method:   Remove
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Deletes Parameter File - The "Restart" File is also
//            a parameter file and we want to remove it when we
//            complete a good batch.
//
//            If the file is open, we close it before removing it.
//
//  Args:     None
//
//  Returns:  Nothing
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/23/99 DMT Added is_open check.
//---------------------------------------------------------------------+
void CParamFile::Remove()
{
	if (NULL != m_szFileName)
	{
#ifndef _WIN32
		if (rdbuf()->is_open()) {
#else
		if (is_open()) {	// DMT 02/23/99
#endif
			close();
		}

		remove(m_szFileName);
	}
}





//---------------------------------------------------------------------+
//  Method:   operator <<
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     TBD
//
//  Args:     TBD
//
//  Returns:  ostream
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//---------------------------------------------------------------------+
ostream& operator << (ostream& s, CParamFile& p)
{
	s << "Name: " << p.m_szFileName << endl
	  << "Buffer: " << ((NULL != p.m_szReadBuf) ? p.m_szReadBuf : "<NULL>") << " " << ends;

	return s;
}





//---------------------------------------------------------------------+
//  Method:   SetString
//  Author:   David Tamkun
//  Written:  March 1999
//
//  Desc:     Used to "Set" a value pointed to by one of the 
//            Member String Variables.  
//
//            Deletes and reallocates Memory to hold new value.
//
//            If Source is NULL or an empty string,
//            Dest will point to a 2-byte string containing
//            NULL values to be consistent with processing in
//            the Load() method.
//
//  Args:     Member String Variable (a pointer)
//            Pointer to New Value
//
//  Returns:  KOCC_GOOD if okay
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  
//  History: 
//  03/12/99 DMT Created original version.
//---------------------------------------------------------------------+
long CParamFile::SetString( char** pszDest, char* pszSource) {


	long	ll_newlen		= 0;
	long    ll_current_len	= 0;
	long    ll_return       = KOCC_GOOD;


	if(*pszDest != NULL) {
		delete [] *pszDest;
		*pszDest = NULL;
	}


	if (pszSource == NULL) {
		// alloc 2 byte string and set to NULL values
		try {
			*pszDest = new char[2];
		}
		catch (...) {

			*pszDest = NULL;
			ll_return = KOCC_FAIL;
			
			iu_errors.AddError(KORC_MEMORY_ALLOC_FAIL, KOCC_FAIL,
				"Error allocating memory.",
				"CParamFile::SetString:1",
				is_InstanceName.getPointer());
		}

		if (*pszDest != NULL) {
			**pszDest = NULL;
			(*pszDest)[1] = NULL;
		}

	}
	else {
		// alloc strlen + 1 string and copy value
		try {
			*pszDest = new char[strlen(pszSource) + 1];
		}
		catch (...) {

			*pszDest = NULL;
			ll_return = KOCC_FAIL;

			iu_errors.AddError(KORC_MEMORY_ALLOC_FAIL, KOCC_FAIL,
				"Error allocating memory.",
				"CParamFile::SetString:2",
				is_InstanceName.getPointer());
		}

		if (*pszDest != NULL) {
			strcpy(*pszDest, pszSource);
		}
	}

	return ll_return;

}



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
void CParamFile::TranslateErrno (CString& rs_msg, int vi_errno) {

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
		       << "is invalid ("
			   << vi_errno
			   << ").";

		break;


	case ENOENT:
		rs_msg << "The specified filename or part of its path "
		       << "is invalid ("
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

	} // end switch

}





//---------------------------------------------------------------------+
//  Method:   CalcPassword
//  Author:   David M Tamkun
//  Written:  March 1999
//
//  Desc:     This function calculates and returns a valid
//            Administrator password.
//
//            This algorithm matches Dan Creager's mainframe code.
//
//            Format:
//
//            ADMHDMYM
//            |  ||||+> First digit of the minute, assuming a leading
//            |  ||||   zero, converted into alpha representation.
//            |  |||+-> Last digit of the year, converted into alpha
//            |  |||    representation.        
//            |  ||+--> Last digit of the month
//            |  |+---> Last digit of the day  
//            |  +----> Last digit of the hour 
//            +-------> Literal value for the Administrator (ADM).  
//            
//
//  Args:     Ref to CString - Valid Admin Password
//
//  Returns:  KOCC_GOOD if okay, 
//            KOCC_FAIL if an error occurs.
//  
//  Dev Log:  This password stuff could be a separate class, but for
//            now it's only needed to validate a password parameter
//            and determine if Admin Access is allowed.
//
//            Consider breaking out into a separate class in the future.
//
//  History: 
//  03/31/99  DMT Created original version.
//---------------------------------------------------------------------+
long CParamFile::CalcPassword(CString& sPwd) {

	CString	ls_msg;
	long	ll_return = KOCC_GOOD;
	time_t	lt_time;
	tm*		lstr_time	= NULL;
	char	lpc_timestamp[15];		// YYYYMMDDHHMMSS string
									// If you change this size, 
									// make sure you change the
									// size passed to strftime
									// below

	char	lpc_pwd[] = {'A', 'D', 'M', '_', '_', '_', '_', '_', '\0'};

	//                       0123456789
	char	lpc_toalpha[] = "ABCDEFGHIJ";
	char    lpc_char[]    = {'_', '\0'};



	time(&lt_time);
	lstr_time = localtime(&lt_time);

	if (lstr_time != NULL) {

		// formats timestamp string to "YYYYMMDDHHMMSS"
		if(strftime(lpc_timestamp, sizeof(lpc_timestamp), 
			"%Y" "%m%d%H" "%M%S",
			lstr_time) == 0) {

			ll_return = KOCC_FAIL;
			ls_msg = "Error Formatting Timestamp.";

			iu_errors.AddError(KORC_ERROR_FORMATTING_TIMESTMP, ll_return,
						ls_msg,
						"CParamFile::CalcPassword:1",
						is_InstanceName.getPointer());
		}
	}
	else {

		ll_return = KOCC_FAIL;
		ls_msg = "Error determining current time.";

		iu_errors.AddError(KORC_ERROR_FORMATTING_TIMESTMP, ll_return,
						ls_msg,
						"CParamFile::CalcPassword:2",
						is_InstanceName.getPointer());
	}

	if (ll_return == KOCC_GOOD) {

		// ADMHDMYM
		// |  ||||+> First digit of the minute, assuming a leading
		// |  ||||   zero, converted into alpha representation.[7]
		// |  |||+-> Last digit of the year, converted into alpha
		// |  |||    representation.        [6]
		// |  ||+--> Last digit of the month[5]
		// |  |+---> Last digit of the day  [4]
		// |  +----> Last digit of the hour [3]
		// +-------> Literal value for the Administrator (ADM).  
		//
		//                    01234567890123
        // timestamp layout: "YYYYMMDDHHMMSS"
		lpc_pwd[3] = lpc_timestamp[9];
		lpc_pwd[4] = lpc_timestamp[7];
		lpc_pwd[5] = lpc_timestamp[5];


		lpc_char[0] = lpc_timestamp[3];
		lpc_pwd[6]  = lpc_toalpha[atoi(lpc_char)];

		lpc_char[0] = lpc_timestamp[10];
		lpc_pwd[7]  = lpc_toalpha[atoi(lpc_char)];

		sPwd = lpc_pwd;
	}

	return ll_return;
}






//---------------------------------------------------------------------+
//  Method:   rtrim
//  Author:   David M Tamkun
//  Written:  April 1999
//
//  Desc:     Trims trailing spaces off normal "C-Style" NULL 
//            terminated strings.
//
//            Starts at end of string and replaces spaces with NULLs
//            until first character > space is encountered, or until
//            the beginning of the line is reached.
//
//            Does nothing if passed in pointer is NULL.
//
//
//  Args:     ptr to string, the string to trim
//
//  Returns:  Nothing
//  
//  Dev Log:  
//
//  History: 
//  04/28/99  DMT Created original version.
//---------------------------------------------------------------------+
void CParamFile::rtrim(char* vszArg) {

	int li_len = 0;
	int li_counter = 0;


	if (vszArg != NULL) {

		for (li_counter = strlen(vszArg) - 1; li_counter >= 0; li_counter--) {

			if (vszArg[li_counter] == ' ') {
				vszArg[li_counter] = '\0';
			}

			if (vszArg[li_counter] > ' ') {
				li_counter = -1;
			}
		}
	}

	return;
}






//---------------------------------------------------------------------+
//  Method:   valid_bool
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Convert C style boolean values into string literals. 
//            0 = <Off>
//            non-0 = <On>
//
//  Args:     char* - Pointer to Member Variable to Log.
//
//  Returns:  String literal of boolean value
//  
//  Dev Log:  
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
const char* CParamFile::valid_bool(const long l)
{
	static const char* szOn = "<On>";
	static const char* szOff = "<Off>";
	if (0 != l)
		return szOn;
	else
		return szOff;

}




//---------------------------------------------------------------------+
//  Method:   valid_str
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     If specified pointer is NULL, returns ptr to "<Null>".
//            Used to log parameters and print "<Null>" if parameter
//            really is null.
//
//  Args:     char* - Pointer to Member Variable to Log.
//
//  Returns:  Specified Pointer if it's not NULL, or a pointer to 
//            "<Null>" if specified pointer is NULL.
//  
//  Dev Log:  
//  History: 
//  01/15/99 CAH Created original version.
//  02/12/00 DRC Moved into the ancestor so it can be shared with 
//               CRestartParms.
//---------------------------------------------------------------------+
const char* CParamFile::valid_str(const char* c)
{
	static const char* szNull = "<Null>";
	if (NULL != c)
		return c;
	else
		return szNull;

}





