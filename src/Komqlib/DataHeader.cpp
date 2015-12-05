//---------------------------------------------------------------------+
//  File:     DataHeader.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000
//  Desc:     Implementation file for the CDataHeader class.
//
//            CDataHeader encapsulates the knowledge of how to construct
//            and subsequently interpret the XML based header information 
//            that is inserted into/removed from data files modified by 
//            any of the File Modification Services.
//
//  Dev Log: 
//  01)  An extra unnecessary set of <SEQ> <SRCNAME> tags is being 
//       generated at the end of a list.
//
//  02)  We need to enhance to handle the situation if a file without
//       and xml document header is to be deblocked.
//
//  03)  Handle it if the file name is from a foreign OS
//
//  History: 
//  11/10/00 DRC Created original code.
//---------------------------------------------------------------------+
#include "DataHeader.h"

const char*   CDataHeader::m_szOS;
const char*   CDataHeader::m_szProgram;
const char*   CDataHeader::m_szVersion;
const char*   CDataHeader::m_szSetName;
const char*   CDataHeader::m_szSetClass;
      long    CDataHeader::m_lFileSeq;
const char*   CDataHeader::m_szSetFileName;
const char*   CDataHeader::m_szXfrTime;
const char*   CDataHeader::m_szSrcFileName;
const char*   CDataHeader::m_szSrcFileClass;
const char*   CDataHeader::m_szFileFormat;
      long    CDataHeader::m_lBlkLen;
      long    CDataHeader::m_lRcrdLen;
      long    CDataHeader::m_lRcrdCnt;


	  
//---------------------------------------------------------------------+
//  Table that defines the structure of the Data Header XMLDocument
//
//	NULL/TypeDlmtr entry indicates end of table.
//---------------------------------------------------------------------+
CDataHeader::SXMLDocLayout CDataHeader::sXMLDocLayout[] = {
//
//  Outbound variable mapping
//

// Tag Name				Data Type       Ptr to storage
//===================== =============== =================
{ "Outbound",			TypeBegMap,		NULL		      },
{ "FileXfr",			TypeGroup,		NULL              },

{ "Env",				TypeGroup,		NULL              },
{ "OS",					TypeString,		&m_szOS           },
{ "Pgm",				TypeString,		&m_szProgram      },
{ "Ver",				TypeString,		&m_szVersion      },
{ "Env",				TypeEndGrp,		NULL		      },

{ "Set",				TypeGroup,		NULL              },
{ "Name",				TypeString,		&m_szSetName      },
{ "Class",				TypeString,		&m_szSetClass     },
{ "File",				TypeList,		NULL              },
{ "Seq",				TypeLong,		&m_lFileSeq       },
{ "SrcName",			TypeString,		&m_szSetFileName  },
{ "File",				TypeEndLst,		NULL		      },
{ "Set",				TypeEndGrp,		NULL		      },

{ "File",				TypeGroup,		NULL              },
{ "DateTime",			TypeString,		&m_szXfrTime      },
{ "SrcName",			TypeString,		&m_szSrcFileName  },
{ "Class",				TypeString,		&m_szSrcFileClass },
{ "Fmt",				TypeString,		&m_szFileFormat   },
{ "BlkLen",				TypeLong,		&m_lBlkLen        },
{ "RcrdLen",			TypeLong,		&m_lRcrdLen       },
{ "RcrdCnt",			TypeLong,		&m_lRcrdCnt       },
{ "File",				TypeEndGrp,		NULL		      },
{ "FileXfr",			TypeEndGrp,		NULL		      },

{ NULL,					TypeEndMap,		NULL		      },

//
//  Inbound variable mapping
//

// Keyword				        Data Type   Ptr to storage
//============================= =========== =================
{ "Inbound",					TypeBegMap,	NULL				},
{ "FileXfr.Env.OS",				TypeString,	&m_szOS				},
{ "FileXfr.Env.Pgm",			TypeString,	&m_szProgram		},
{ "FileXfr.Env.Ver",			TypeString,	&m_szVersion		},
{ "FileXfr.Set.Name",			TypeString,	&m_szSetName		},
{ "FileXfr.Set.Class",			TypeString,	&m_szSetClass		},
{ "FileXfr.Set.File.Seq",		TypeLong,	&m_lFileSeq			},
{ "FileXfr.Set.File.SrcName",	TypeString,	&m_szSetFileName	},
{ "FileXfr.File.DateTime",		TypeString,	&m_szXfrTime		},
{ "FileXfr.File.SrcName",		TypeString,	&m_szSrcFileName	},
{ "FileXfr.File.Class",			TypeString,	&m_szSrcFileClass	},
{ "FileXfr.File.Fmt",			TypeString,	&m_szFileFormat		},
{ "FileXfr.File.BlkLen",		TypeLong,	&m_lBlkLen			},
{ "FileXfr.File.RcrdLen",		TypeLong,	&m_lRcrdLen			},
{ "FileXfr.File.RcrdCnt",		TypeLong,	&m_lRcrdCnt			},
{ NULL,							TypeEndMap,	NULL				},
{ NULL,							TypeEndTbl,	NULL				}
};





//---------------------------------------------------------------------+
//  Method:   CDataHeader (default constructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Initialize the attributes of this instance of this class.
//
//  Args:     CExtSrvcsParamFile*
//  Returns:  Nothing.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
CDataHeader::CDataHeader(CExtSrvcsParamFile* a_pcParms){
time_t l_time;
struct tm* l_Today;

//
// Initialize the Header Document
//
is_InstanceName	= "Data Header";
*((CString*) this) = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>";
m_pcFileList = NULL;
m_pCursor    = NULL;
m_lXfrLen    = 0;
m_bEOF       = false;


//
//  Set the time String 
//  Format: YYYY-MM-DDTHH:MM:SS
//  Sample: 2000-11-10T13:45:23
//
time(&l_time);
l_Today = localtime(&l_time);
strftime(m_szToday, 20,"%Y-%m-%dT%H:%M:%S", l_Today);
setXfrTime(m_szToday);

m_szOS           = NULL;
m_szProgram      = NULL;
m_szVersion      = NULL;
m_szSetName      = NULL;
m_szSetClass     = NULL;
m_lFileSeq       = 0;
m_szSetFileName  = NULL;
m_szXfrTime      = NULL;
m_szSrcFileName  = NULL;
m_szSrcFileClass = NULL;
m_szFileFormat   = NULL;
m_lBlkLen        = 0;
m_lRcrdLen       = 0;
m_lRcrdCnt       = 0;
m_XMLDoc         = "";
m_szWork         = "";
m_Stack.top();
m_pcFNameSrvc = new CFileNameSrvc(a_pcParms);
}





//---------------------------------------------------------------------+
//  Method:   ~CDataHeader (default destructor)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Release resources held by this instance of this class.
//
//  Args:     None.
//  Returns:  Nothing.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
CDataHeader::~CDataHeader(){
m_Stack.clear();
delete m_pcFNameSrvc;
}





//---------------------------------------------------------------------+
//  Method:   clear
//  Desc:     clear the Data Header
//
//  Args:     None
//  Returns:  Nothing
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CDataHeader::clear(){
	m_XMLDoc.fill();
}  /* End clear */ 



//---------------------------------------------------------------------+
//  Method:   ConvDlmtrs
//  Desc:     Convert foreign file/path delimiters to the local system.
//         
//  Note:     The delimiter value in m_pcFNameSrvc->m_FileDlmtr contains
//            the appropriate delimiter value for the local system.
//
//  Args:     char* address of a data buffer (unconverted)
//  Returns:  char* address of a data buffer (converted)
//  
//  Dev Log:           
//  History: 
//  03/06/01 DRC Created original version.
//---------------------------------------------------------------------+
char*   CDataHeader::ConvDlmtrs(char* a_szData){

#ifdef _WIN32
 for (uint l_iIndx=0; l_iIndx < strlen(a_szData); l_iIndx++){
    if (*(a_szData+l_iIndx) == UNIX_DLMTR) *(a_szData+l_iIndx) = WIN_DLMTR; 
 } /* end for */
#else
 for (uint l_iIndx=0; l_iIndx< strlen(a_szData); l_iIndx++){
    if (*(a_szData+l_iIndx) == WIN_DLMTR) *(a_szData+l_iIndx) = UNIX_DLMTR;  
 } /* end for */
#endif
 
return a_szData;
} /* End ConvDlmtrs */


//---------------------------------------------------------------------+
//  Method:   eof  (EndOfFile)
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     End of File (list) indicator.  Used when reading the 
//            header as simulated file input.  
//
//  Args:     None
//
//  Returns:  Boolean = True  = End of List has been reached
//                    = False = End of List has not occured
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CDataHeader::eof(){
return 	m_bEOF;
}





//---------------------------------------------------------------------+
//  Method:   isBlocked
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Used to determine if this data is blocked.  
//
//  Args:     None
//
//  Returns:  Boolean = True  = Data is blocked.
//                    = False = Data is unblocked.
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CDataHeader::isBlocked(){
return (strnicmp(m_szFileFormat, "FixedBlocked", 6) == 0);
}  /* End isBlocked */





//---------------------------------------------------------------------+
//  Method:   isFileSetComplete
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Determine if each file in the FileXfr.Set is present 
//            within the local filing system.  
//
//  Args:     None.
//
//  Returns:  Boolean  =  True  = All files are present.
//                        False = NOT all files are present.
//
//  Dev Log: 
//           
//  History: 
//  11/10/00 DRC Created original version.
//  01/15/01 DRC Copy and Restore the XML Document to enable 
//               reparsing at a later time. 
//---------------------------------------------------------------------+
BOOL CDataHeader::isFileSetComplete(){
CStringList	l_Result;
BOOL        l_bResult = true;
BOOL        l_bFirstErr  = true;
CString*    l_pcEntry;
CString     l_szErrText;
CFile       l_File;
char*       l_szFileSetTag = "FileXfr.Set.File.SrcName=";

//
//  Convert the XML Document into a keyword/data list
//  skipping over the document's XML header.
//
Push();
ParseXML(m_XMLDoc.StrStr("<FileXfr>"), "", &l_Result, ".");
Pop();

//
//  Reset the Data Header attributes from the keyword/data list.
//
l_pcEntry = (CString*) l_Result.top();
while (!l_Result.endOfList()){
    //
	//  On all FileSet entries, 
	//     Check if the indicated file can be successfully 
	//     opened on the local system to determine its presence.
	//
	if (l_pcEntry->replace(l_szFileSetTag, "") > 0){
		l_File.initSelf(m_pcFNameSrvc->getLocalFileName(l_pcEntry));

		l_bResult = (l_File.Open(ios::in | ios::nocreate) == KOCC_GOOD);

		if (!l_bResult){
			if (l_bFirstErr) {
				l_szErrText = "Warning: FileSet received is NOT complete! Missing:";
				l_szErrText << m_pcFNameSrvc->getLocalFileName(l_pcEntry) << " ";
				l_bFirstErr = false;
			}
			else {
				l_szErrText << ", " << m_pcFNameSrvc->getLocalFileName(l_pcEntry);
			}  /* end If */
		} /* End If */
	} /* End If */

	l_pcEntry = (CString*) l_Result.next();
} /* End While */
if (!l_bFirstErr) 
	iu_errors.AddError(KORC_BATCH_OUT_OF_BALANCE, KOCC_WARNING, 
		l_szErrText, "CDataHeader::isFileSetComplete:1", 
		is_InstanceName.getPointer());

return l_bFirstErr;
} /* End isFileSetComplete */


//---------------------------------------------------------------------+
//  Method:   isHeaderComplete
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Used to determine if the entire header has been
//            loaded from an external source.  
//
//  Args:     None
//
//  Returns:  Boolean = True  = End of Header has been loaded
//                    = False = End of Header has not been loaded
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CDataHeader::isHeaderComplete(){
return (m_XMLDoc.pos("</FileXfr>") > 0);
}  /* End isHeaderComplete */





//---------------------------------------------------------------------+
//  Method:   getXXX
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Attribute getter functions.  
//
//  Args:     None.
//
//  Returns:  void*  =  Address of attribute value.
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CDataHeader::getConvertedFileName(char* m_szFileName){
if (isLocalOS())
	return (char*) m_szFileName;
else {
	return (char*) chngFileDlmtrs((char*) m_szFileName);
} /* End If */
} /* End getConvertedFileName */

char* CDataHeader::getOS(){
return (char*) m_szOS;
}

char* CDataHeader::getProgram(){
return (char*) m_szProgram;
}

char* CDataHeader::getVersion(){
return (char*) m_szVersion;
}

char* CDataHeader::getSetName(){
return (char*) m_szSetName;
}

char* CDataHeader::getSetClass(){
return (char*) m_szSetClass;
}

char* CDataHeader::getSetFileName(){
return getConvertedFileName((char*) m_szSetFileName);
}

long CDataHeader::getFileSeq(){
return m_lFileSeq;
}

CFileList* CDataHeader::getSetList(){
return m_pcFileList;
}

char* CDataHeader::getXfrTime(){
return (char*) m_szXfrTime;
}


char* CDataHeader::getSrcFileName(){
return getConvertedFileName((char*) m_szSrcFileName);
}

char* CDataHeader::getSrcFileClass(){
return (char*) m_szSrcFileClass;
}

char* CDataHeader::getFileFormat(){
return (char*) m_szFileFormat;
}

long  CDataHeader::getBlkLen(){
return m_lBlkLen;
}

long  CDataHeader::getRcrdLen(){
return m_lRcrdLen;
}

long  CDataHeader::getRcrdCnt(){
return m_lRcrdCnt;
}


//---------------------------------------------------------------------+
//  Method:   isLocalOS
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Determines if a transfered originated from the local
//            operating system or a foreign one.  
//
//  Args:     None.
//
//  Returns:  Bool  True  = File from the local OS
//                  False = File from a foreign OS
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
BOOL CDataHeader::isLocalOS(){
#ifdef _WIN32
return (strnicmp(m_szOS, "Win", 3) == 0);
#else
return (strnicmp(m_szOS, "Uni", 3) == 0);
#endif
}


//---------------------------------------------------------------------+
//  Method:   LoadXML
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Construct a Header in XML Document format from the current
//            attributes in this class.  
//
//  Args:     None.
//
//  Returns:  Nothing.
//
//  Dev Log: 
//  01)  Convert to using ArchivalFileName so that the fully qualified
//       pathname is not sent to the receiving system.  It should only
//       be the Path/file name that is subordinate to the SrcFiles= mask.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CDataHeader::LoadXML(){
SXMLDocLayout*	l_pDocLayout;
CString*        l_pcBuffer = (CString*) &m_XMLDoc;
//char            l_WrkBuf[33];
char*			l_pDataValue = NULL;


//-------------------------------------------------------------+
//  Construct an XML Header from the current contents of the 
//  attributes in this class.
//-------------------------------------------------------------+
for (l_pDocLayout = (SXMLDocLayout*) sXMLDocLayout; 
	 l_pDocLayout->eDataType != TypeEndMap; l_pDocLayout++){
	//
	//  Set an local pointer for convience
	//
	if (l_pDocLayout->pDataValue != NULL) 
		l_pDataValue = *((char**) l_pDocLayout->pDataValue);

	switch (l_pDocLayout->eDataType){
	case TypeString:
		if (l_pDataValue != NULL){
		   *(l_pcBuffer) << "<"  << l_pDocLayout->szTagName << ">" 
						 <<  l_pDataValue       
						 << "</" << l_pDocLayout->szTagName << ">";
		} /* End If */
			break;

	case TypeLong:
		if (l_pDataValue != NULL){
			*(l_pcBuffer) << "<"  << l_pDocLayout->szTagName << ">" 
						  << *((long*)l_pDocLayout->pDataValue)
//						  << _ltoa(*((long*)l_pDocLayout->pDataValue), 
//								   l_WrkBuf, 10)       
						  << "</" << l_pDocLayout->szTagName << ">";
		} /* End If */
			break;

	case TypeGroup:
		*(l_pcBuffer) << "<"  << l_pDocLayout->szTagName << ">";
		break;

	case TypeEndGrp:
		*(l_pcBuffer) << "</" << l_pDocLayout->szTagName << ">";
		break;

	case TypeList:
		if (m_pcFileList != NULL) {
			//
			//  For each entry in the File List
			//
			CFile* l_pcFile = (CFile*) m_pcFileList->top();
			long l_lFilSeq = 1;
			while (!m_pcFileList->endOfList()){
				//
				//  For each entry in the Set definition 
				//

				//  Open Tag
				*(l_pcBuffer) << "<"  << l_pDocLayout->szTagName << ">";

				//
				//  Insert Tags defined for each list item
				//
				for (SXMLDocLayout*	l_pTemp = l_pDocLayout;
				     l_pTemp->eDataType != TypeEndLst;
					 l_pTemp++){
					switch (l_pTemp->eDataType){
					case TypeString:
						m_szWork = (char*) l_pcFile->GetName();
						*(l_pcBuffer) << "<"  << l_pTemp->szTagName << ">" 
						<< m_pcFNameSrvc->getArchiveName(&m_szWork)
				        << "</" << l_pTemp->szTagName << ">";
						break;

					case TypeLong:
						*(l_pcBuffer) << "<"  << l_pTemp->szTagName << ">" 
						<< l_lFilSeq++       
						<< "</" << l_pTemp->szTagName << ">";
						break;
					} /* End switch */
				} /* End For */

				//  Close Tag
				*(l_pcBuffer) << "</" << l_pDocLayout->szTagName << ">";

				l_pcFile = (CFile*) m_pcFileList->next();
			} /* end while */
		} /* End If */
	} /* End switch */
} /* End For */
m_pCursor	= m_XMLDoc.m_Buf1;
m_lXfrLen	= m_XMLDoc.vlen();
m_bEOF		= false;
} /* End LoadXML */


//---------------------------------------------------------------------+
//  Method:   ParseXML
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Convert the XML Document into a keyword/data list.  
//
//  Args:     char*        = Address of Work Buffer
//            char*        = Address of Keyword Qualifier String
//            CStringList* = Address of Keyword/Data List (Result)
//            char*        = Address of Keyword Qualifier Delimiter
//
//  Returns:  char*        = Address of Work Buffer
//
//  History: 
//  11/10/00 DRC Created original version.
//  03/08/01 DRC Added logic to convert foreign OS filenames to local
//---------------------------------------------------------------------+
char* CDataHeader::ParseXML(char*		 a_szBuffer,
						    char*		 a_szQual, 
						    CStringList* a_pcResult,
						    char*        a_szQualDlmtr){

while(isValidPointer(a_szBuffer) && (*a_szBuffer == '<')){
	char    l_szDlmtr[] = "</\0                         ";
	char*	l_szBeg  = NULL;
	char*   l_szEnd  = NULL;
	char*   l_szData = NULL;
	CString l_szQual;



	//
	//  Isolate Keyword
	//
	l_szBeg  = m_szWork.StrStr(a_szBuffer, ">");
	(l_szBeg == NULL) ? 0 : *l_szBeg = '\0';

	//
	//  Remove closing Tag
	//
	strcat(l_szDlmtr,a_szBuffer + 1);
	strcat(l_szDlmtr,">");
	l_szEnd  = m_szWork.StrStr(l_szBeg+1, l_szDlmtr);
	(l_szEnd == NULL) ? 0 : *l_szEnd = '\0';

	//
	//  Build a Qualifier by appending a new level to the 
	//  passed value of Qualifier
	//
	if (strlen(a_szQual) > 0) {
		l_szQual = a_szQual;
		l_szQual += a_szQualDlmtr;
	}
	else {
		l_szQual.fill();
	} /* end If */

//	(strlen(a_szQual) > 0) ? l_szQual = a_szQual       
//		                   : l_szQual.fill();
//	(strlen(a_szQual) > 0) ? l_szQual += a_szQualDlmtr 
//		                   : 0;
	l_szQual += a_szBuffer + 1;

	//
	//  Reset Pointer for the next level of ParseXML
	//
	a_szBuffer = l_szBeg + 1;

	//
	//  Call Parse
	//
	l_szData = ParseXML(a_szBuffer, l_szQual.getPointer(),
		                a_pcResult, a_szQualDlmtr);
	if ((l_szData != NULL) 
	&&  (strlen(l_szData) > 0)){
		//
		//  Add the Keyword/Data string to the list
		//
		CString* l_Temp = new CString(l_szQual);
		*l_Temp << "=";
		*l_Temp << ConvDlmtrs(l_szData);
		a_pcResult->add(l_Temp);
	}
	//
	//  Reset Pointer for the next loop of this ParseXML
	//
	a_szBuffer = l_szEnd + strlen(l_szDlmtr);

} /* End While */
return a_szBuffer;
} /* End ParseXML */


void   CDataHeader::Pop(){
m_XMLDoc = m_szStack;
#ifdef _xxx_ 
//CItem*   l_pcDocItem;
CString* l_pcXMLDoc;

//
// Retrieve the XML Document copy and refresh the Document
//
l_pcXMLDoc  = (CString*) m_Stack.bottom();
m_XMLDoc    = l_pcXMLDoc;
//
// Clean up this entry
//
m_Stack.remove();
//delete l_pcDocItem;
delete l_pcXMLDoc;
#endif
}  /* End Pop */





void   CDataHeader::Push(){
m_szStack = m_XMLDoc;
#ifdef _xxx_ 
CString* l_pcXMLDoc;
//CItem*   l_pcDocItem;
	
//  Make a copy of the XMLDoc
//  Add the copy to the Stack
l_pcXMLDoc  = new CString(m_XMLDoc);
//l_pcDocItem = new CItem(l_pcXMLDoc);
m_Stack.add(l_pcXMLDoc);
#endif
}




//---------------------------------------------------------------------+
//  Method:   Read
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Reads XMLDocument from DataHeader into Buffer.  
//
//  Args:     CBuffer
//
//  Returns:  KOCC_WARNING - if EOF
//
//            KOCC_FAIL    - if error 
//
//            KOCC_GOOD    - if read line of data.
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CDataHeader::Read(CBuffer& cBuffer) {

	long	ll_return = KOCC_GOOD;
	long	ll_strlen = 0;
	long    ll_cursor = 0;

	cBuffer.clear();
	if (cBuffer.isBlocked()){
		//---------------------------------------------------------+
		// On successful read with input blocking of fixed length
		//    records specified, fill the buffer with whole, fixed
		//    length records.
		//---------------------------------------------------------+
		if (cBuffer.isFixed()){
			ll_strlen = getline(cBuffer.getPointer(), cBuffer.getRcrdLen());
			while ((!eof()) && (!cBuffer.isFilled())){
				cBuffer.setDataLen(cBuffer.getDataLen() + ll_strlen);
				cBuffer.RFill(' ');
				ll_strlen = getline(cBuffer.getCursor(), 
					(cBuffer.getRcrdLen() <= cBuffer.getAvail() ) 
					 ? cBuffer.getRcrdLen() : cBuffer.getAvail());
			} /* End While */
			//
			//  Add in the length of the final block
			//
			if (eof() && (ll_strlen > 0)){
				cBuffer.setDataLen(cBuffer.getDataLen() + ll_strlen);
				cBuffer.RFill(' ');
			} /* End If */
		}
		else {
			//
			//  Variable Length blocking is unsupported at this time.
			//
			ll_return = KOCC_FAIL;
		} /* End If */
	}
	else{
		ll_strlen = getline(cBuffer.getPointer(), cBuffer.getSize());
		if (!eof()) cBuffer.setDataLen(ll_strlen);
	}/* End If */


	//
	//  Check for End of File condition
	//
	if (eof() && (ll_return == KOCC_GOOD)){
		ll_return = KOCC_WARNING;
	} /* End If */

	return ll_return;
} /* End Read */




//---------------------------------------------------------------------+
//  Method:   getline
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Reads XMLDocument from DataHeader into Buffer.  
//
//  Args:     char*  =  Address of a work buffer 
//            long   =  Length of the work buffer
//
//  Returns:  long   =  Number of characters reset in the work buffer.
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CDataHeader::getline(char* a_szBuf, long a_lLen){
long l_lXfrLen = 0;

if (m_lXfrLen >= a_lLen) {
	memcpy(a_szBuf,m_pCursor,a_lLen);
	m_pCursor += a_lLen;
	m_lXfrLen -= a_lLen;
	return a_lLen;
}
else {
    memcpy(a_szBuf,m_pCursor,m_lXfrLen);
	m_pCursor = m_XMLDoc.m_Buf1;
	l_lXfrLen = m_lXfrLen;
	m_lXfrLen = m_XMLDoc.vlen();
	m_bEOF    = true;
	return l_lXfrLen;
} /* End If */
} /* End getline */


//CDataHeader& CDataHeader::operator<< (CDataHeader&, char*){{
//}



//---------------------------------------------------------------------+
//  Method:   setXXX
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Attribute setter functions.  
//
//  Args:     void*  =  Address of attribute value 
//
//  Returns:  Nothing.
//  
//  Dev Log:           
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CDataHeader::setOS(char* a_szArg){
setString((char**) &m_szOS, a_szArg);
}

void CDataHeader::setProgram(char* a_szArg){
setString((char**) &m_szProgram, a_szArg);
}

void CDataHeader::setVersion(char* a_szArg){
setString((char**) &m_szVersion, a_szArg);
}

void CDataHeader::setSetName(char* a_szArg){
setString((char**) &m_szSetName, a_szArg);
}

void CDataHeader::setSetClass(char* a_szArg){
setString((char**) &m_szSetClass, a_szArg);
}

void CDataHeader::setSetList(CFileList* a_pcArg){
m_pcFileList = a_pcArg;
}

void CDataHeader::setXfrTime(char* a_szArg){
setString((char**) &m_szXfrTime, a_szArg);
}

void CDataHeader::setSrcFileName(char* a_szArg){
setString((char**) &m_szSrcFileName, a_szArg);
}

void CDataHeader::setSrcFileClass(char* a_szArg){
setString((char**) &m_szSrcFileClass, a_szArg);
}

void CDataHeader::setFileFormat(char* a_szArg){
setString((char**) &m_szFileFormat, a_szArg);
}

void CDataHeader::setBlkLen(long a_lArg){
m_lBlkLen = a_lArg;
}

void CDataHeader::setRcrdLen(long a_lArg){
m_lRcrdLen = a_lArg;
}

void CDataHeader::setRcrdCnt(long a_lArg){
m_lRcrdCnt = a_lArg;}


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
//  11/10/00 DRC Removed references to the CAny2 error collection.
//---------------------------------------------------------------------+
long CDataHeader::setString( char** pszDest, char* pszSource) {


	long	ll_newlen		= 0;
	long    ll_current_len	= 0;
	long    ll_return       = KOCC_GOOD;



	if(*pszDest != NULL) {
		delete [] *pszDest;
		*pszDest = NULL;
	}


	if (pszSource == NULL) {
		// alloc 2 byte string and set to NULL values
		*pszDest = new char[2];
		if (*pszDest != NULL) {
			**pszDest = NULL;
			(*pszDest)[1] = NULL;
		}

	}
	else {
		// alloc strlen + 1 string and copy value
		*pszDest = new char[strlen(pszSource) + 1];
		if (*pszDest != NULL) {
			strcpy(*pszDest, pszSource);
		}
	}

	return ll_return;
} /* End SetString */





//---------------------------------------------------------------------+
//  Method:   setXMLAttribute
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Reset an attributes in this class from a single 
//            keyword/data entry from the XML Document header.  
//
//  Args:     CString*  =  Address of a single Keyword/Data entry.
//
//  Returns:  Nothing.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
void CDataHeader::setXMLAttribute(CString* a_szEntry){
SXMLDocLayout*	l_pDocLayout;
CString*        l_pcBuffer = (CString*) this;
char*			l_pKeyword;
char*			l_pData;

//
//  Separate the Keyword and Data strings
//
l_pKeyword = strtok(a_szEntry->getPointer(), "=");
l_pData = strtok(NULL, "=");
   

//
//  Skip past the outbound map
//
for(l_pDocLayout = (SXMLDocLayout*) sXMLDocLayout;
    l_pDocLayout->eDataType != TypeEndMap;
	l_pDocLayout++);

//
//  Scan the inbound map entries for a match on Keyword
//
for (l_pDocLayout++;l_pDocLayout->eDataType != TypeEndMap; l_pDocLayout++){
	if (strnicmp( l_pDocLayout->szTagName, l_pKeyword, strlen(l_pKeyword) ) == 0){
		//
		//  and Process the attribute by dataType
		//
		switch (l_pDocLayout->eDataType){
		case TypeString:
			if (l_pData     != NULL){
				setString( (char**) l_pDocLayout->pDataValue, l_pData);
			} /* End If */
			break;

		case TypeLong:
			if (l_pData     != NULL){
				*((long*) l_pDocLayout->pDataValue) = atol(l_pData);
			} /* End If */
			break;
		} /* End switch */
		goto exitPt;
	} /* End If */
} /* End For */
exitPt:
	return;
} /* End setXMLAttribute */



//---------------------------------------------------------------------+
//  Method:   chngFileDlmtrs
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Toggle between Windows and Unix style file delimiters. 
//
//  Args:     char*  Address of the FileName string.
//
//  Returns:  char*  Address of the FileName string.
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
char* CDataHeader::chngFileDlmtrs(char* a_szFileName){
m_szWork = a_szFileName;

//
//  Set the delimiters
//
#ifdef _WIN32
	m_szWork.replace("/","\\");
#else
	m_szWork.replace("\\","/");
#endif
m_szWork >> a_szFileName;
return a_szFileName;
} /* End chngFileDlmtrs */


//---------------------------------------------------------------------+
//  Method:   UnloadXML
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Reset the attributes in this class from the Header, in XML 
//            Document format, found in the current buffer. 
//            "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" 
//
//  Args:     None.
//
//  Returns:  Nothing.
//
//  History: 
//  11/10/00 DRC Created original version.
//  01/15/01 DRC Copy and Restore the XML Document to enable 
//               reparsing at a later time. 
//  01/16/01 DRC Added validation of the XMLDoc's presence 
//---------------------------------------------------------------------+
void CDataHeader::UnloadXML(void){
CStringList	l_Result;
CString*    l_pcEntry;

//
//  Validate that the XML document has been loaded
//
if ((m_XMLDoc.vlen() > 0) 
&&  (m_XMLDoc.StrStr("<FileXfr>") != NULL)){
	//
	//  Convert the XML Document into a keyword/data list
	//  skipping over the document's XML header.
	//
	Push();
	ParseXML(m_XMLDoc.StrStr("<FileXfr>"), "", &l_Result, ".");
	Pop();

	//
	//  Reset the Data Header attributes from the keyword/data list.
	//
	l_pcEntry = (CString*) l_Result.top();
	while (!l_Result.endOfList()){
		setXMLAttribute(l_pcEntry);
		l_pcEntry = (CString*) l_Result.next();
	} /* End While */
	setXMLAttribute(l_pcEntry);
} /* End If */
} /* End UnloadXML */




//---------------------------------------------------------------------+
//  Method:   Write
//  Author:   Daniel R Creager
//  Written:  November 2000
//
//  Desc:     Writes XMLDocument from Buffer into DataHeader.  
//
//  Args:     ref to CBuffer class
//
//  Returns:  KOCC_WARNING - if EOF
//
//            KOCC_FAIL    - if error 
//
//            KOCC_GOOD    - if read line of data.
//  
//  Dev Log:  
//
//  History: 
//  11/10/00 DRC Created original version.
//---------------------------------------------------------------------+
long CDataHeader::Write(CBuffer& cBuffer) {

	long	ll_return = KOCC_GOOD;

	//m_bWritten = TRUE;

	//-----------------------------------------------------------------+
	// On blocked output specified, 
	//    On Fixed Length Records 
	//       write each logical record separately  
	//       stripping off trailing spaces
	//-----------------------------------------------------------------+
		if (cBuffer.isBlocked()){ 
			if(cBuffer.isFixed()){
				cBuffer.getFirst();
				while (!cBuffer.isEndOfBuffer()){
					m_XMLDoc.operator+=(cBuffer.RTrim(cBuffer.getCursor()));
					cBuffer.next();
				} /* End While */
				m_XMLDoc.operator+=(cBuffer.RTrim(cBuffer.getCursor()));
				cBuffer.clear();
			}
			else{
				//
				//  Variable Length blocking is unsupported at this time.
				//
				ll_return = KOCC_FAIL;
			} /* End If */
		}
		else {
	//		*this << cBuffer.getPointer() << endl;
		} /* End If */

	return ll_return;
} /* End Write */









