#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "ProtoMgr.h"
#include "DynamicMesh.h"
#include "ColliderObject.h"
#include "ColliderObject_Sphere.h"
#include "ColliderObject_AABB.h"
#include "ColliderObject_OBB.h"
#include "CollisionMgr.h"


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

	for (auto& rCollider : m_vecOptimizedColliders) {
		rCollider->Update_Object(_fDeltaTime);
	}

	/*for (auto& rPair : m_vecColliders) {
		for (auto& rObj : rPair.second) {
			rObj->Update_Object(_fDeltaTime);
		}
	}*/
	return 0;
}

void CGameObject::SetValid(_bool _bIsValid) {
	m_bIsValid = _bIsValid;

	for (auto& rChild : m_vecChildList) {
		rChild->SetValid(_bIsValid);
	}

	for (auto& rCollider : m_vecOptimizedColliders) {
		rCollider->SetValid(_bIsValid);
	}
}

void CGameObject::SetActivated(_bool _bIsActivated) {
	m_bIsActivated = _bIsActivated;

	for (auto& rChild : m_vecChildList) {
		rChild->SetActivated(_bIsActivated);
	}

	for (auto& rCollider : m_vecOptimizedColliders) {
		rCollider->SetActivated(_bIsActivated);
	}
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
			// ���� �ش��ϴ� �ݶ��̴� ����Ʈ�� auto ������ ���
			pColliders = &rPair.second;
			break;
		}
	}

	if(!pColliders){
		// ���� �ش��ϴ� �ݶ��̴� ����Ʈ�� ã�� ���ߴٸ� �ݶ��̴� ����Ʈ�� �����Ѵ�.
		GetColliderList().emplace_back(make_pair(strBoneName, vector<CColliderObject*>()));
		for (auto& rPair : m_vecColliders) {
			if (rPair.first == strBoneName) {
				pColliders = &rPair.second;
				break;
			}
		}
		
	}

	pColliders->emplace_back(_pCollider);					// ���� �ݶ��̴� ����Ʈ�� ����
	m_vecOptimizedColliders.emplace_back(_pCollider);		// ���� �ݶ��̴� ����Ʈ�� ����
		
	_pCollider->m_pParent = this;

	return true;
}

_bool CGameObject::IsChildExist(CGameObject * _pChild)
{
	if (!_pChild)
		return false;

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

	/*for (auto& rPair : m_vecColliders) {
		for (auto& rCollider : rPair.second) {
			if (rCollider == _pCollider) {
				return true;
			}
		}
	}*/
	for (auto& rCollider : m_vecOptimizedColliders) {
		if (rCollider == _pCollider) {
			return true;
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

//CColliderObject * CGameObject::ExtractColliderFromList(const _tchar * _pColliderTag)
//{
//	if (!_pColliderTag)
//		return nullptr;
//		
//	_int iSize = m_vecColliders.size()
//	for(_int i = 0; i < )
//	for (auto& rPair : m_vecColliders) {
//		auto find_iter = find_if(rPair.second.begin(), rPair.second.end(), [_pColliderTag](CColliderObject* _pCollider) {
//			return lstrcmp(_pCollider->GetColliderTag(), _pColliderTag) == 0;
//		});
//		if (find_iter == rPair.second.end())
//			continue; // ã�� ���ߴٸ� ���� ������ ã�ƺ��ϴ�.
//		
//		// ���� �ݶ��̴� ����Ʈ���� ����
//		CColliderObject* pCollider = *find_iter;
//		rPair.second.erase(find_iter);		
//
//		// ���� �ݶ��̴� ����Ʈ���� ����
//		for (auto iter = m_vecOptimizedColliders.begin(); iter != m_vecOptimizedColliders.end(); ++iter) {
//			if (lstrcmp((*iter)->GetColliderTag(), _pColliderTag) == 0) {
//				m_vecOptimizedColliders.erase(iter);
//				break;
//			}
//		}
//
//		//return pCollider;
//		return nullptr;
//	}
//	return nullptr;
//}

void CGameObject::ClearColliders()
{
	for (auto& rPair : m_vecColliders) {
		for_each(rPair.second.begin(), rPair.second.end(), Engine::CDeleteObj());
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_vecColliders.clear();
	m_vecOptimizedColliders.clear();
}

CColliderObject * CGameObject::GetColliderFromTag(const _tchar * _pColliderTag)
{
	/*for (auto& rPair : m_vecColliders) {
		for (auto& rObj : rPair.second) {
			if (lstrcmp(rObj->GetColliderTag(), _pColliderTag) == 0) {
				return rObj;
			}
		}
	}*/

	for (auto& rCollider : m_vecOptimizedColliders) {
		if (lstrcmp(rCollider->GetColliderTag(), _pColliderTag) == 0) {
			return rCollider;
		}
	}

	return nullptr;
}

_bool CGameObject::SaveCollidersInfo(HANDLE& _hfOut) {
	DWORD dwByte = 0;

	char cBoneName[MAX_PATH] = "";
	_tchar tcColliderTag[MAX_PATH] = L"";
	Engine::E_COLLIDER_TYPE eColliderType = Engine::TYPE_END;

	_int iBoneNameSize = m_vecColliders.size();
	_int iCollidersSize = 0;
	_int iStrLen = 0;
	Engine::CColliderObject* pCollider = nullptr;

	WriteFile(_hfOut, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);					// �� ���� ����
	// �ݶ��̴��� ������ �� ������ŭ ��ȸ
	for (_int i = 0; i < iBoneNameSize; ++i) {
		strcpy_s(cBoneName, m_vecColliders[i].first.c_str());
		iStrLen = strlen(cBoneName);
		WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);							// �� �̸� ���� ����
		WriteFile(_hfOut, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);			// �� �̸� ����

		// �ϳ��� ���� ���� �ݶ��̴� ���� �����Ѵ�.
		iCollidersSize = m_vecColliders[i].second.size();
		WriteFile(_hfOut, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);			// �ݶ��̴� �� ����

		for (_int j = 0; j < iCollidersSize; ++j) {
			pCollider = m_vecColliders[i].second[j];

			lstrcpy(tcColliderTag, pCollider->GetColliderTag());
			iStrLen = lstrlen(pCollider->GetColliderTag());
			WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
			WriteFile(_hfOut, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);// �ݶ��̴� �±� ����
			// � �ݶ��̴� Ÿ�������� �����Ѵ�.
			eColliderType = pCollider->GetColliderType();
			WriteFile(_hfOut, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);			// �ݶ��̴� Ÿ�� ����

			pCollider->SaveInfo(_hfOut);														// �ݶ��̴� Ʈ������ ����
		}
	}

	return true;
}

_bool CGameObject::LoadCollidersInfo(HANDLE& _hfIn) {
	ClearColliders();

	DWORD dwByte = 0;

	char cBoneName[MAX_PATH] = "";
	_tchar tcColliderTag[MAX_PATH] = L"";
	Engine::E_COLLIDER_TYPE eColliderType = Engine::TYPE_END;

	_int iBoneNameSize = 0;
	_int iCollidersSize = 0;
	_int iStrLen = 0;
	Engine::CColliderObject* pCollider = nullptr;

	ReadFile(_hfIn, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);						// �� ���� �б�
	// �ݶ��̴��� ������ �� ������ŭ ��ȸ
	for (_int i = 0; i < iBoneNameSize; ++i) {
		ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);								// �� �̸� ���� �б�
		ReadFile(_hfIn, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);				// �� �̸� �б�

		// �ϳ��� ���� ���� �ݶ��̴� ���� �����Ѵ�.
		ReadFile(_hfIn, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);					// �ݶ��̴� �� �б�

		for (_int j = 0; j < iCollidersSize; ++j) {
			ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
			ReadFile(_hfIn, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);		// �ݶ��̴� �±� �б�

			// � �ݶ��̴� Ÿ�������� �����Ѵ�.
			ReadFile(_hfIn, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);				// �ݶ��̴� Ÿ�� �б�

			switch (eColliderType) {																// �ݶ��̴� Ʈ������ �б�
			case Engine::TYPE_SPHERE:
				pCollider = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
				pCollider->LoadInfo(_hfIn);
				if (strcmp(cBoneName, "None") == 0) {
					if (!AddCollider(pCollider))
						abort();
				}
				else {
					if (!AddCollider(pCollider, cBoneName))
						abort();
				}
				break;
			case Engine::TYPE_AABB:
				pCollider = Engine::CColliderObject_AABB::Create(m_pGraphicDev);
				pCollider->LoadInfo(_hfIn);
				if (strcmp(cBoneName, "None") == 0) {
					if (!AddCollider(pCollider))
						abort();
				}
				else {
					if (!AddCollider(pCollider, cBoneName))
						abort();
				}
				if (static_cast<Engine::CColliderObject_AABB*>(pCollider)->IsAutoGenerating())
					static_cast<Engine::CColliderObject_AABB*>(pCollider)->GenerateAABB();
				break;
			case Engine::TYPE_OBB:
				pCollider = Engine::CColliderObject_OBB::Create(m_pGraphicDev);
				pCollider->LoadInfo(_hfIn);
				if (strcmp(cBoneName, "None") == 0) {
					if (!AddCollider(pCollider))
						abort();
				}
				else {
					if (!AddCollider(pCollider, cBoneName))
						abort();
				}
				break;
			}

			pCollider->SetColliderTag(tcColliderTag);
		}
	}

	m_pCullingSphere = dynamic_cast<Engine::CColliderObject_Sphere*>(GetColliderFromTag(L"CULL"));
	CCollisionMgr::GetInstance()->AddGameObject(this);

	return true;
}