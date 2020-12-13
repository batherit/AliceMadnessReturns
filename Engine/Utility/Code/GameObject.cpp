#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "ProtoMgr.h"
#include "DynamicMesh.h"
#include "ColliderObject.h"


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

_int CGameObject::Update_Object(const _float & _fDeltaTime)
{
	for (auto& rChild : m_vecChildList) {
		rChild->Update_Object(_fDeltaTime);
	}

	for (auto& rPair : m_vecColliders) {
		for (auto& rObj : rPair.second) {
			rObj->Update_Object(_fDeltaTime);
		}
	}
	return 0;
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

	for (auto& rChild : m_vecChildList) {
		Safe_Release(rChild);
	}

	m_vecChildList.clear();
	m_vecChildList.shrink_to_fit();

	ClearColliders();

	Safe_Release(m_pGraphicDev);
}

CComponent * Engine::CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

const _matrix * CGameObject::GetBoneMatrixPointer(const char * _pBoneName)
{
	// ��ȿ�� �θ� ���޵� ���
	if (_pBoneName) {
		// �� �̸��� �ش��ϴ� �������� ã�´�.
		CDynamicMesh* pParentDynamicMesh = GetComponent<CDynamicMesh>();
		if (!pParentDynamicMesh) 
			return nullptr;			// ���� �޽��� ���ٸ� false�� ��ȯ => ���� ����� ����
		const D3DXFRAME_DERIVED* pFrame = pParentDynamicMesh->Get_FrameByName(_pBoneName);
		if (!pFrame) 
			return nullptr;			// �������� ���ٸ� false�� ��ȯ => ���� ����� ����

		return &(pFrame->CombinedTransformationMatrix);	// ����� ������ ��ȯ
		//GetTransform()->SetParentBoneMatrix(&pFrame->CombinedTransformationMatrix);
	}
	
	return nullptr;
}

// �ش� ��ü�� �θ� _pParent�� �� _pBoneName�� ���δ�.
// _pBoneName�� nullptr�� ��� �׵������ ���õȴ�. => ���� ���� ������ ����.
void Engine::CGameObject::SetParent(CGameObject* _pParent, const char* _pBoneName) {
	if (!_pParent) {
		// �θ� NULL���� ������ �θ�κ��� �����ȴ�.
		ReleaseParent();
		return;
	}

	CGameObject* pTempParent = m_pParent;
	const _matrix* pTempParentBoneMatrixPointer = GetTransform()->GetParentBoneMatrixPointer();
	ReleaseParent();								// ���� �θ�κ��� ����
	if (!_pParent->AddChild(this, _pBoneName)) {	// ���ο� �θ� ����
		// ���ο� �θ���� ������ �����ߴٸ�, ���� �θ�� �ٽ� �����Ѵ�.
		m_pParent = pTempParent;
		//Safe_AddRef(m_pParent);
		GetTransform()->SetParentBoneMatrix(pTempParentBoneMatrixPointer);
	}

	return;
}

// �ڽ� _pChild�� ������ �� _pBoneName�� ���δ�.
// _pBoneName�� nullptr�� ��� �׵������ ���õȴ�. => ���� ���� ������ ����.
_bool CGameObject::AddChild(CGameObject * _pChild, const char* _pBoneName)
{
	if (!_pChild || IsChildExist(_pChild) || _pChild->GetParent())
		return false;	

	if (_pBoneName) {
		const _matrix* pBoneMatrixPointer = GetBoneMatrixPointer(_pBoneName);
		if (!pBoneMatrixPointer)
			return false;	// �� ����� �������� ������ ���� ���¸� �����Ѵ�.
		else
			_pChild->GetTransform()->SetParentBoneMatrix(pBoneMatrixPointer);
	}
	else {
		_pChild->GetTransform()->SetParentBoneMatrix(nullptr);
	}

	// _pChild�� ��ȿ�� ��ü�̸�, ���� �θ� �������� �ʴ� ��Ȳ
	m_vecChildList.emplace_back(_pChild);
	_pChild->m_pParent = this;
	//Safe_AddRef(_pChild->m_pParent);

	return true;
}

_bool CGameObject::AddCollider(CColliderObject * _pCollider, const char * _pBoneName)
{
	if (!_pCollider || IsColliderExist(_pCollider) || _pCollider->GetParent())
		return false;

	if (_pBoneName) {
		const _matrix* pBoneMatrixPointer = GetBoneMatrixPointer(_pBoneName);
		if (!pBoneMatrixPointer)
			return false;	// �� ����� �������� ������ ���� ���¸� �����Ѵ�.
		else
			_pCollider->GetTransform()->SetParentBoneMatrix(pBoneMatrixPointer);
	}
	else {
		_pCollider->GetTransform()->SetParentBoneMatrix(nullptr);
	}

	// _pChild�� ��ȿ�� ��ü�̸�, ���� �θ� �������� �ʴ� ��Ȳ
	//m_vecChildList.emplace_back(_pCollider);
	string strBoneName = "None";
	vector<CColliderObject*>* pColliders = nullptr;
	if (_pBoneName) {
		strBoneName = _pBoneName;
	}

	for (auto& rPair : m_vecColliders) {
		if (rPair.first == strBoneName) {
			pColliders = &rPair.second;
			break;
		}
	}

	if (pColliders)
		pColliders->emplace_back(_pCollider);
	else {
		GetColliderList().emplace_back(make_pair(strBoneName, vector<CColliderObject*>()));
		for (auto& rPair : m_vecColliders) {
			if (rPair.first == strBoneName) {
				pColliders = &rPair.second;
				break;
			}
		}
		pColliders->emplace_back(_pCollider);
	}
		
	_pCollider->m_pParent = this;

	return true;
}

_bool CGameObject::IsChildExist(CGameObject * _pChild)
{
	if (!_pChild)
		abort();

	for (auto& rChild : m_vecChildList) {
		if (rChild == _pChild) {
			return true;
		}
	}
	return false;
}

_bool CGameObject::IsColliderExist(CColliderObject * _pCollider)
{
	if (!_pCollider)
		abort();

	for (auto& rPair : m_vecColliders) {
		for (auto& rCollider : rPair.second) {
			if (rCollider == _pCollider) {
				return true;
			}
		}
	}

	return false;
}

void CGameObject::ReleaseParent()
{
	if (!m_pParent)
		return;

	auto& rChildListOfParent = m_pParent->m_vecChildList;
	auto find_iter = find(rChildListOfParent.begin(), rChildListOfParent.end(), this);
	if (find_iter == rChildListOfParent.end())
		return;	// �θ��� �ڽ� ����Ʈ���� ������ ã�� ���ߴٸ� ���ʿ� �θ� �ڽ� ���谡 �ƴϴ�.

	rChildListOfParent.erase(find_iter);
	//Safe_Release(m_pParent);
	m_pParent = nullptr;
	GetTransform()->SetParentBoneMatrix(nullptr);
}

void CGameObject::ReleaseChild(CGameObject * _pChild)
{
	if (!_pChild)
		return;

	auto find_iter = find(m_vecChildList.begin(), m_vecChildList.end(), _pChild);
	if (find_iter == m_vecChildList.end())
		return; // �θ��� �ڽ� ����Ʈ���� ������ ã�� ���ߴٸ� ���ʿ� �θ� �ڽ� ���谡 �ƴϴ�.

	m_vecChildList.erase(find_iter);
	//Safe_Release(_pChild->m_pParent);
	_pChild->m_pParent = nullptr;
	_pChild->GetTransform()->SetParentBoneMatrix(nullptr);
}

void CGameObject::ClearColliders()
{
	for (auto& rPair : m_vecColliders) {
		for_each(rPair.second.begin(), rPair.second.end(), Engine::CDeleteObj());
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_vecColliders.clear();
}
