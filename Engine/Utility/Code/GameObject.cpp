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

_int CGameObject::Update_Object(const _float & _fDeltaTime)
{
	for (auto& rChild : m_vecChildList) {
		rChild->Update_Object(_fDeltaTime);
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

