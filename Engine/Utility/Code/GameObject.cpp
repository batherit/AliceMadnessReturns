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
		// �θ� NULL���� ������ �θ�κ��� �����ȴ�.
		Safe_Release(m_pParent);						// ���� �θ� ����
		m_pParent = _pParent;							// �θ� nullptr�� ����	
		GetTransform()->SetParentBoneMatrix(nullptr);	// �θ� ����ĵ� nullptr�� ����
		return true;
	}
	
	if (_pBoneName) {
		// �� �̸��� �ش��ϴ� �������� ã�´�.
		CDynamicMesh* pParentDynamicMesh = _pParent->GetComponent<CDynamicMesh>();
		if (!pParentDynamicMesh) return false;	// ���� �޽��� ���ٸ� false�� ��ȯ
		const D3DXFRAME_DERIVED* pFrame = pParentDynamicMesh->Get_FrameByName(_pBoneName);
		if (!pFrame) return false;				// �������� ���ٸ� false�� ��ȯ

		GetTransform()->SetParentBoneMatrix(&pFrame->CombinedTransformationMatrix);
	}

	// �θ� ����
	Safe_Release(m_pParent);
	m_pParent = _pParent;
	Safe_AddRef(m_pParent);
	return true;
}