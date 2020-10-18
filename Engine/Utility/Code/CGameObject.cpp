#include "CGameObject.h"
#include "Component.h"
#include "ProtoMgr.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CGameObject::CGameObject(const CGameObject & rhs)
	:
	m_pGraphicDev(rhs.m_pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CGameObject::~CGameObject()
{
	//Free();
}

CComponent * Engine::CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	Engine::CComponent*	pComponent = Find_Component(pComponentTag, eID);

	if (nullptr == pComponent)
		return nullptr;

	return pComponent;
}

CComponent * Engine::CGameObject::GetComponent(const COMPONENTID eComponentID, const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(pComponentTag));
	if (iter != m_mapComponent[eComponentID].end())
		return iter->second;

	return nullptr;
}

void Engine::CGameObject::AddComponent(const COMPONENTID eComponentID, const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(pComponentTag));
	if (iter != m_mapComponent[eComponentID].end())
		return;

	CComponent* pComp = CProtoMgr::GetInstance()->Clone(pComponentTag);
	if (pComp) {
		m_mapComponent[eComponentID].emplace(pComponentTag, pComp);
	}
}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

CComponent * Engine::CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}
