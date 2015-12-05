#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CAny
//#define _CAny
#include "CAny.h"
//#endif

class CItem : public CAny{

friend class CCollection;
friend class CCursor;
 
public:
	virtual long  appendLink(CItem* a_Cursor);
	CItem(                   void*  a_Item);
	~CItem();
	virtual long  insertLink(CItem* a_Cursor);
	virtual void* operator= (CItem& a_Item);
	virtual long  removeLink();

protected:
	void*  mp_Item;
	CItem* mp_Prev;
	CItem* mp_Next;
};
