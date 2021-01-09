#ifndef MouseMgr_h__
#define MouseMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "framework.h"

BEGIN(Client)

class CMouseMgr : public CBase
{
	DECLARE_SINGLETON(CMouseMgr)

private:
	explicit CMouseMgr(void);
	virtual ~CMouseMgr(void);

public:
	void MouseFix(_bool );

private:
	virtual void Free(void);
};

END

#endif // MouseMgr_h__
