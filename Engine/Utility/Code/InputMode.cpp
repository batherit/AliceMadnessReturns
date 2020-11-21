#include "InputMode.h"
#include "InputModeMgr.h"

USING(Engine)

CInputMode::CInputMode(CInputModeMgr* _pInputModeMgr)
	:
	m_pInputModeMgr(_pInputModeMgr)
{
	if (m_pInputModeMgr)
		Safe_AddRef(m_pInputModeMgr);
}

CInputMode::~CInputMode()
{
	//Free();
}


void CInputMode::Free(void)
{
	Safe_Release(m_pInputModeMgr);
}
