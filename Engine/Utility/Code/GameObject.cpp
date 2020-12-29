#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "ProtoMgr.h"
#include "DynamicMesh.h"
#include "ColliderObject.h"
#include "ColliderObject_Sphere.h"
#include "ColliderObject_AABB.h"
#include "ColliderObject_OBB.h"


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
	// 유효한 부모가 전달된 경우
	if (_pBoneName) {
		// 본 이름에 해당하는 프레임을 찾는다.
		CDynamicMesh* pParentDynamicMesh = GetComponent<CDynamicMesh>();
		if (!pParentDynamicMesh) 
			return nullptr;			// 동적 메쉬가 없다면 false를 반환 => 기존 본행렬 유지
		const D3DXFRAME_DERIVED* pFrame = pParentDynamicMesh->Get_FrameByName(_pBoneName);
		if (!pFrame) 
			return nullptr;			// 프레임이 없다면 false를 반환 => 기존 본행렬 유지

		return &(pFrame->CombinedTransformationMatrix);	// 본행렬 포인터 반환
		//GetTransform()->SetParentBoneMatrix(&pFrame->CombinedTransformationMatrix);
	}
	
	return nullptr;
}

// 해당 객체를 부모 _pParent의 뼈 _pBoneName에 붙인다.
// _pBoneName이 nullptr일 경우 항등행렬이 세팅된다. => 본에 의한 영향이 없음.
void Engine::CGameObject::SetParent(CGameObject* _pParent, const char* _pBoneName) {
	if (!_pParent) {
		// 부모가 NULL값이 들어오면 부모로부터 해제된다.
		ReleaseParent();
		return;
	}

	CGameObject* pTempParent = m_pParent;
	const _matrix* pTempParentBoneMatrixPointer = GetTransform()->GetParentBoneMatrixPointer();
	ReleaseParent();								// 기존 부모로부터 해제
	if (!_pParent->AddChild(this, _pBoneName)) {	// 새로운 부모에 편입
		// 새로운 부모로의 편입을 실패했다면, 기존 부모로 다시 세팅한다.
		m_pParent = pTempParent;
		//Safe_AddRef(m_pParent);
		GetTransform()->SetParentBoneMatrix(pTempParentBoneMatrixPointer);
	}

	return;
}

// 자식 _pChild를 본인의 뼈 _pBoneName에 붙인다.
// _pBoneName이 nullptr일 경우 항등행렬이 세팅된다. => 본에 의한 영향이 없음.
_bool CGameObject::AddChild(CGameObject * _pChild, const char* _pBoneName)
{
	if (!_pChild || IsChildExist(_pChild) || _pChild->GetParent())
		return false;	

	if (_pBoneName) {
		const _matrix* pBoneMatrixPointer = GetBoneMatrixPointer(_pBoneName);
		if (!pBoneMatrixPointer)
			return false;	// 본 행렬이 존재하지 않으면 기존 상태를 유지한다.
		else
			_pChild->GetTransform()->SetParentBoneMatrix(pBoneMatrixPointer);
	}
	else {
		_pChild->GetTransform()->SetParentBoneMatrix(nullptr);
	}

	// _pChild는 유효한 객체이며, 현재 부모가 존재하지 않는 상황
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
			return false;	// 본 행렬이 존재하지 않으면 기존 상태를 유지한다.
		else
			_pCollider->GetTransform()->SetParentBoneMatrix(pBoneMatrixPointer);
	}
	else {
		_pCollider->GetTransform()->SetParentBoneMatrix(nullptr);
	}

	// _pChild는 유효한 객체이며, 현재 부모가 존재하지 않는 상황
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
		return;	// 부모의 자식 리스트에서 본인을 찾지 못했다면 애초에 부모 자식 관계가 아니다.

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
		return; // 부모의 자식 리스트에서 본인을 찾지 못했다면 애초에 부모 자식 관계가 아니다.

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

CColliderObject * CGameObject::GetColliderFromTag(const _tchar * _pColliderTag)
{
	for (auto& rPair : m_vecColliders) {
		for (auto& rObj : rPair.second) {
			if (lstrcmp(rObj->GetColliderTag(), _pColliderTag) == 0) {
				return rObj;
			}
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

	WriteFile(_hfOut, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);					// 뼈 개수 저장
	// 콜라이더가 부착된 뼈 개수만큼 순회
	for (_int i = 0; i < iBoneNameSize; ++i) {
		strcpy_s(cBoneName, m_vecColliders[i].first.c_str());
		iStrLen = strlen(cBoneName);
		WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);							// 뼈 이름 길이 저장
		WriteFile(_hfOut, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);			// 뼈 이름 저장

		// 하나의 뼈에 붙은 콜라이더 수를 저장한다.
		iCollidersSize = m_vecColliders[i].second.size();
		WriteFile(_hfOut, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);			// 콜라이더 수 저장

		for (_int j = 0; j < iCollidersSize; ++j) {
			pCollider = m_vecColliders[i].second[j];

			lstrcpy(tcColliderTag, pCollider->GetColliderTag());
			iStrLen = lstrlen(pCollider->GetColliderTag());
			WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
			WriteFile(_hfOut, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);// 콜라이더 태그 저장
			// 어떤 콜라이더 타입인지를 저장한다.
			eColliderType = pCollider->GetColliderType();
			WriteFile(_hfOut, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);			// 콜라이더 타입 저장

			pCollider->SaveInfo(_hfOut);														// 콜라이더 트랜스폼 저장
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

	ReadFile(_hfIn, &iBoneNameSize, sizeof(iBoneNameSize), &dwByte, nullptr);						// 뼈 개수 읽기
	// 콜라이더가 부착된 뼈 개수만큼 순회
	for (_int i = 0; i < iBoneNameSize; ++i) {
		ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);								// 뼈 이름 길이 읽기
		ReadFile(_hfIn, &cBoneName, sizeof(char) * (iStrLen + 1), &dwByte, nullptr);				// 뼈 이름 읽기

		// 하나의 뼈에 붙은 콜라이더 수를 저장한다.
		ReadFile(_hfIn, &iCollidersSize, sizeof(iCollidersSize), &dwByte, nullptr);					// 콜라이더 수 읽기

		for (_int j = 0; j < iCollidersSize; ++j) {
			ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
			ReadFile(_hfIn, &tcColliderTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);		// 콜라이더 태그 읽기

			// 어떤 콜라이더 타입인지를 저장한다.
			ReadFile(_hfIn, &eColliderType, sizeof(eColliderType), &dwByte, nullptr);				// 콜라이더 타입 읽기

			switch (eColliderType) {																// 콜라이더 트랜스폼 읽기
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

	return true;
}