#include "Resources.h"

USING(Engine)

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pGraphicDev)
	: 
	m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CResources::CResources(const CResources& rhs)
	: 
	CComponent(rhs),
	m_pGraphicDev(rhs.m_pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CResources::~CResources(void)
{
}

void Engine::CResources::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
