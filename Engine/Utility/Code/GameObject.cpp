#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "ProtoMgr.h"
#include "DynamicMesh.h"


USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
	m_pTransform = AddComponent<CTransform>();
}

Engine::CGameObject::CGameObject(const CGameObject & rhs)
	:
	m_pGraphicDev(rhs.m_pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
	m_pTransform = AddComponent<CTransform>();
}

Engine::CGameObject::~CGameObject()
{
	//Free();
}

//CComponent * Engine::CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
//{
//	Engine::CComponent*	pComponent = Find_Component(pComponentTag, eID);
//
//	if (nullptr == pComponent)
//		return nullptr;
//
//	return pComponent;
//}

CComponent * Engine::CGameObject::GetComponent(const COMPONENTID eComponentID, const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(pComponentTag));
	if (iter != m_mapComponent[eComponentID].end())
		return iter->second;

	return nullptr;
}

CComponent* Engine::CGameObject::AddComponent(const COMPONENTID eComponentID, const _tchar * pComponentTag)
{
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(pComponentTag));
	if (iter != m_mapComponent[eComponentID].end())
		return iter->second;

	CComponent* pComp = CProtoMgr::GetInstance()->Clone(pComponentTag);
	if (pComp) {
		pComp->SetOwner(this);
		m_mapComponent[eComponentID].emplace(pComponentTag, pComp);
		return pComp;
	}

	return nullptr;
}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pParent);
	Safe_Release(m_pGraphicDev);
}

CComponent * Engine::CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

_bool Engine::CGameObject::SetParent(CGameObject* _pParent, const char* _pBoneName) {
	if (!_pParent) {
		// 부모가 NULL값이 들어오면 부모로부터 해제된다.
		Safe_Release(m_pParent);						// 기존 부모 해제
		m_pParent = _pParent;							// 부모를 nullptr로 세팅	
		GetTransform()->SetParentBoneMatrix(nullptr);	// 부모 뼈행렬도 nullptr로 세팅
		return true;
	}
	
	if (_pBoneName) {
		// 본 이름에 해당하는 프레임을 찾는다.
		CDynamicMesh* pParentDynamicMesh = _pParent->GetComponent<CDynamicMesh>();
		if (!pParentDynamicMesh) return false;	// 동적 메쉬가 없다면 false를 반환
		const D3DXFRAME_DERIVED* pFrame = pParentDynamicMesh->Get_FrameByName(_pBoneName);
		if (!pFrame) return false;				// 프레임이 없다면 false를 반환

		GetTransform()->SetParentBoneMatrix(&pFrame->CombinedTransformationMatrix);
	}

	// 부모 설정
	Safe_Release(m_pParent);
	m_pParent = _pParent;
	Safe_AddRef(m_pParent);
	return true;
}