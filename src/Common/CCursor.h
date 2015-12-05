#ifdef _WIN32
#pragma once
#else
#pragma options once
#endif

//#ifndef _CAny
//#define _CAny
#include "CAny.h"
//#endif

//#ifndef _CItem
//#define _CItem
#include "CItem.h"
//#endif


class CCursor : public CAny {

public:
	virtual void*  bottom(CItem* a_InitPos, long a_Count);
	CCursor();
	virtual ~CCursor();
	virtual BOOL   endOfList();
	virtual CItem* get();
	virtual void*  getData();
	virtual void*  next();
	virtual void*  prev();
	virtual void*  top(    CItem* a_InitPos, long a_Count);
	virtual void   set(    CItem* a_Item);
	virtual void   setData(void*  a_Item);

private:
    void*   resetCursor(CItem* a_InitPos, long a_Count);
	virtual void   operator++ (void);
	virtual void   operator-- (void);

protected:
	CItem* mp_Current;
    long   ml_Count;
	BOOL   mp_isForward;
};
