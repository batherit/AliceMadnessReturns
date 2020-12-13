#include "pch.h"
#include "DynamicObject.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject & rhs)
	:
	CGameObject(rhs)
{
}

CDynamicObject::~CDynamicObject(void)
{
}

HRESULT CDynamicObject::Ready_Object(void)
{
	// Mesh
	//if(m_pMesh)
	//	m_mapComponent[Engine::ID_Dynamic].emplace(L"Com_Mesh", m_pMesh);

	// Renderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	//m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	return S_OK;
}

int CDynamicObject::Update_Object(const _float & _fDeltaTime)
{
	CGameObject::Update_Object(_fDeltaTime);
	m_pMesh->Play_Animation(_fDeltaTime);

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return 0;
	}

	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CDynamicObject::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
}

CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicObject*	pInstance = new CDynamicObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

_bool CDynamicObject::SetRenderInfo(const _tchar * _pMeshTag, Engine::RENDERID _eRenderID)
{
	if (!_pMeshTag || _eRenderID >= Engine::RENDER_END)
		return false;

	lstrcpy(m_tcMeshTag, _pMeshTag);
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, m_tcMeshTag));

	if (!m_pMesh)
		return false;

	Engine::CComponent* pComponent = m_mapComponent[Engine::ID_STATIC][Engine::CDynamicMesh::GetComponentTag()];
	if (pComponent) {
		// 기존 세팅된 메쉬가 있다면 제거한다.
		Engine::Safe_Release(pComponent);
	}

	// 새로운 메시로 세팅한다.
	m_mapComponent[Engine::ID_STATIC][Engine::CDynamicMesh::GetComponentTag()] = m_pMesh;
	m_pRenderer->SetRenderInfo(_eRenderID, m_pMesh);
	return true;
}

void CDynamicObject::Free(void)
{
	CGameObject::Free();
}

_bool CDynamicObject::SaveInfo(HANDLE & _hfOut)
{
	DWORD dwByte = 0;

	// 1) 메시 태그
	_int iStrLen = lstrlen(m_tcMeshTag);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) 렌더 아이디
	Engine::RENDERID eRenderID = m_pRenderer->GetRenderID();
	WriteFile(_hfOut, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) 트랜스폼 정보 세팅
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	return true;
}

_bool CDynamicObject::LoadInfo(HANDLE & _hfIn)
{
	DWORD dwByte = 0;

	// 1) 메시 태그 로드
	_int iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) 렌더 아이디
	Engine::RENDERID eRenderID = Engine::RENDER_END;
	ReadFile(_hfIn, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) 트랜스폼 정보 세팅
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	return SetRenderInfo(m_tcMeshTag, eRenderID);
}

_bool CDynamicObject::SaveCollidersInfo(HANDLE & _hfOut)
{
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

_bool CDynamicObject::LoadCollidersInfo(HANDLE & _hfIn)
{
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
				if (!AddCollider(pCollider, cBoneName))
					abort();
				break;
			case Engine::TYPE_AABB:
				pCollider = Engine::CColliderObject_AABB::Create(m_pGraphicDev);
				pCollider->LoadInfo(_hfIn);
				if (!AddCollider(pCollider, cBoneName))
					abort();
				break;
			case Engine::TYPE_OBB:
				pCollider = Engine::CColliderObject_OBB::Create(m_pGraphicDev);
				pCollider->LoadInfo(_hfIn);
				if (!AddCollider(pCollider, cBoneName))
					abort();
				break;
			}

			pCollider->SetColliderTag(tcColliderTag);
		}
	}

	m_pCullingSphere = dynamic_cast<Engine::CColliderObject_Sphere*>(GetColliderFromTag(L"CULL"));

	return true;
}
