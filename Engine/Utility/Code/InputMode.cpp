#include "InputMode.h"
#include "InputModeMgr.h"

USING(Engine)

CInputMode::CInputMode(CInputModeMgr* _pInputModeMgr)
	:
	m_pInputModeMgr(_pInputModeMgr)
{
}

CInputMode::~CInputMode()
{
	//Free();
}


void CInputMode::Free(void)
{
}
