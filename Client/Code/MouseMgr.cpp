#include "MouseMgr.h"

USING(Client)

IMPLEMENT_SINGLETON(CMouseMgr)

CMouseMgr::CMouseMgr()
{
}


CMouseMgr::~CMouseMgr()
{
	Free();
}

void CMouseMgr::Free(void)
{
}
