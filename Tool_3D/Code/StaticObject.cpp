#include "pch.h"
#include "StaticObject.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CStaticObject::CStaticObject(const CStaticObject & rhs)
	:
	CGameObject(rhs)
{
}

CStaticObject::~CStaticObject(void)
{
}

HRESULT CStaticObject::Ready_Object(void)
{
	// Mesh
	//if(m_pMesh)
	//	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pMesh);

	// Renderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	//m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	return S_OK;
}

int CStaticObject::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (1 == CGameObject::Update_Object(_fDeltaTime))	// 1-> 비활성화
		return 1;

	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CStaticObject::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;
	m_pRenderer->Render();

	DWORD bIsZBufferEnabled;
	m_pGraphicDev->GetRenderState(D3DRS_ZENABLE, &bIsZBufferEnabled);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	_matrix matTrans;
	_vec3 vPos = GetTransform()->GetPos();
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matTrans);
	m_pGraphicDev->SetTexture(0, NULL);
	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_SphereCol");
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, bIsZBufferEnabled);
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

_bool CStaticObject::SetRenderInfo(const _tchar * _pMeshTag, Engine::RENDERID _eRenderID)
{
	if (!_pMeshTag || _eRenderID >= Engine::RENDER_END)
		return false;

	lstrcpy(m_tcMeshTag, _pMeshTag);
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, m_tcMeshTag));

	if (!m_pMesh)
		return false;

	Engine::CComponent* pComponent = m_mapComponent[Engine::ID_STATIC][Engine::CStaticMesh::GetComponentTag()];
	if (pComponent) {
		// 기존 세팅된 메쉬가 있다면 제거한다.
		Engine::Safe_Release(pComponent);
	}

	// 새로운 메시로 세팅한다.
	m_mapComponent[Engine::ID_STATIC][Engine::CStaticMesh::GetComponentTag()] = m_pMesh;
	m_pRenderer->SetRenderInfo(_eRenderID, m_pMesh);

	LoadCollidersInfo();

	return true;
}

void CStaticObject::Free(void)
{
	CGameObject::Free();
}

_bool CStaticObject::SaveInfo(HANDLE & _hfOut)
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

_bool CStaticObject::LoadInfo(HANDLE & _hfIn)
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

_bool CStaticObject::LoadCollidersInfo()
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, m_tcMeshTag);
	lstrcat(szDataPath, L".col");

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	CGameObject::LoadCollidersInfo(hFile);

	CloseHandle(hFile);
	return true;
}
