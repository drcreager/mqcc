//---------------------------------------------------------------------+
//  File:     CBatchList.cpp
//  Author:   Daniel R Creager
//  Written:  November 2000  
//  Desc:     Implementaion of the CBatchList class, which is a collection
//            of message batches from an MQ Series message queue that 
//            meet the search criterial provided upon instantiation.  
//
//  History: 
//  11/10/00  DRC Created original code.
//---------------------------------------------------------------------+
#include "BatchList.h"

//---------------------------------------------------------------------+
//  Method:   add
//  Desc:     Add a message batch into this collection. 
//  Args:     char* (Name of the batch to associate)
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CBatchList::add(char* a_szBatchId){
//
//  Instantiate a File Object 
//  Store file name in the CFile object

//  Instantiate a Item 
//  Set pointer to the file in the list element (CItem)
//  Add list element to the CBatchList
//
m_pBatch        = new CString(a_szBatchId);
m_pBatchElement = new CItem(m_pBatch);
CCollection::add(m_pBatchElement);
} /* End add */




//---------------------------------------------------------------------+
//  Method:   CBatchList (default Constructor)
//  Desc:     Load the contents of this collection based upon the 
//            specifications provide on invocation. 
//  Args:     char* (a file search argument valid to the host file system)
//            BOOL  (Indicator about searching sub-directories)
//                   True  = Open and search all sub-directories
//                   False = Ignore all sub-directories.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CBatchList::CBatchList(	CLog*				a_pcLog,
						CExtSrvcsParamFile* a_pcParms,
				        char*				a_szBatchIdMask){
m_pcLog    = a_pcLog;
m_pcParms  = a_pcParms;
is_InstanceName = "Messages Batch List";
m_cBuffer.initSelf(a_pcParms->GetBufferSize());
load(a_szBatchIdMask);
m_pBatchList = new CString();
}





//---------------------------------------------------------------------+
//  Method:   ~CBatchList (destructor)
//  Desc:     Unload the contents of this collection releasing all 
//            of its resources. 
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
CBatchList::~CBatchList(){
unload();
m_pcLog    = NULL;
m_pcParms  = NULL;
delete m_pBatchList;
}





//---------------------------------------------------------------------+
//  Method:   load 
//  Desc:     Load the batch collection with the sub-set of message 
//            batches retrieved by the user specified search specs.
//  Args:     char* (a message batch search argument)
//  Returns:  Nothing.
//---------------------------------------------------------------------+
long CBatchList::load(char* a_szBatchIdMask){
long		ll_return		= KOCC_GOOD;
BOOL		lb_queue_open	= FALSE;
CQueMgr     cQueMgr;
CCtlQue	    cCtlQue;
CString		ls_msg;


//-----------------------------------------------------------------+
// Open Queue Manger
//-----------------------------------------------------------------+
ll_return = cQueMgr.initSelf(m_pcLog,
				     (char*) m_pcParms->GetQueMgrName());

//-----------------------------------------------------------------+
//  Initialize I/O with an MQ Series "Control" Queue
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
	ll_return = cCtlQue.initSelf(m_pcLog, &cQueMgr,
					     (char*) m_pcParms->GetCtlQueName(), 
						         true, true);
	if (ll_return == KOCC_GOOD) 
		ll_return = cCtlQue.QOpen(CQue::Browse);
} /* End If */


//-----------------------------------------------------------------+
//  Initialize the I/O loop
//-----------------------------------------------------------------+
if (ll_return == KOCC_GOOD) {
	lb_queue_open = true;
	ll_return = cCtlQue.BrowseFirst(m_cBuffer);
} /* End If */

//-----------------------------------------------------------------+
//  Load the Batch list from the Control Queue
//-----------------------------------------------------------------+
while (ll_return == KOCC_GOOD) {
	//
	//  Insert selected batches into the list
	//
	if (ls_msg.grep((char*) cCtlQue.GetMsgID(), a_szBatchIdMask, "*"))
		add((char*) cCtlQue.GetMsgID());
	ll_return = cCtlQue.BrowseNext(m_cBuffer);
} /* End While */

// [todo:  reset the return code from terminating the i/o loop]

//-----------------------------------------------------------------+
// Close Control Queue and Queue Manager
//-----------------------------------------------------------------+
if (lb_queue_open) cCtlQue.Close();
lb_queue_open = false;
cCtlQue.termSelf();
cQueMgr.termSelf();

return ll_return;
} /* End load */



//---------------------------------------------------------------------+
//  Method:   unload 
//  Desc:     Unload the batches collection releasing all items 
//            currently found in the collection.
//  Args:     None.
//  Returns:  Nothing.
//---------------------------------------------------------------------+
void CBatchList::unload(void){
//
//  Release all associated batchIds.
//
m_pBatch = (CString*) top();
while (!endOfList()){
	delete m_pBatch;
	m_pBatch = (CString*) next();
} /* End while */
//
//  Release all items in this collection.
//
this->clear();
} /* End unload */




//---------------------------------------------------------------------+
//  Method:   operator<<
//  Author:   Daniel R Creager
//  Written:  May 2002
//  Desc:     Concatenates a CBatchList onto a CString
//
//  Args:     CBatchList string value to concatenate onto a CString
//  Returns:  TBD
//  
//  Dev Log: 
//  History: 
//  05/05/02 DRC Created original version.
//---------------------------------------------------------------------+
CString*  CBatchList::getBatchList ( ){
CString*  l_pBatch = (CString*) top();

while (!endOfList()){
	*m_pBatchList << l_pBatch->c_str() << "\n";
	l_pBatch = (CString*) next();
} // End while
return m_pBatchList;
} // end getBatchList


//---------------------------------------------------------------------+
//  Method:   <<  (Insertion Operator)
//  Desc:     Insert the BatchList into an output stream. 
//---------------------------------------------------------------------+
ostream&  operator<< ( ostream& au_stream, CBatchList& au_arg1){
CString*  l_pBatch = (CString*) au_arg1.top();

while (!au_arg1.endOfList()){
	au_stream << l_pBatch->c_str() << endl;
	l_pBatch = (CString*) au_arg1.next();
} // End while
return au_stream;
}

