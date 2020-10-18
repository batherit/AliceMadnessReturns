#include "ProtoMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CProtoMgr)

Engine::CProtoMgr::CProtoMgr(void)
{

}

Engine::CProtoMgr::~CProtoMgr(void)
{
	Free();
}

HRESULT Engine::CProtoMgr::Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*		pPrototype = Find_Proto(pProtoTag);

	if (nullptr != pPrototype)
		return E_FAIL;

	m_mapPrototype.emplace(pProtoTag, pInstance);

	return S_OK;
}

Engine::CComponent* Engine::CProtoMgr::Clone(const _tchar* pProtoTag)
{
	CComponent*	pPrototype = Find_Proto(pProtoTag);

	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone();
}

Engine::CComponent* Engine::CProtoMgr::Find_Proto(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

void Engine::CProtoMgr::Free(void)
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CDeleteMap());
	m_mapPrototype.clear();
}

