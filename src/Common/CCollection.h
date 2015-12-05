#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

#include "CAny.h"
#include "CItem.h"
#include "CCursor.h"

class CCollection : public CAny  {

public:
	virtual long   add(CItem* a_Item);
	virtual long   add(void*  a_Arg);
	virtual void*  bottom();
	virtual long   clear();
	virtual void   clearHeader();
	virtual long   count();
	virtual BOOL   emptyList();
	virtual BOOL   endOfList();
	virtual void*  get();
	virtual long   insert(CItem* a_Item);
	virtual long   insert(void*  a_Arg);
	virtual void*  next();
	virtual void   popCursor();
	virtual void*  prev();
	virtual void   pushCursor();
	virtual long   remove();
	virtual long   remove(CItem* a_Item);
	virtual void*  top();
	virtual void*  operator[] (int Index) const;
	CCollection();
	virtual ~CCollection();

protected:
	void  initHeader (CItem* a_Item, CItem* a_Cursor);
    void* resetCursor(CItem* a_InitPos);
	void  resetHeader(void);

protected:
	CCursor* mp_Cursor;
	CCursor* mp_PrevCursor;
	CItem*   mp_First;
	CItem*   mp_Last;
	long     ml_Count;
	BOOL     mp_isChanged;
};
