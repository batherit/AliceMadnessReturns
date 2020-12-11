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
	m_pMesh->Play_Animation(_fDeltaTime);
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CDynamicObject::Render_Object(void)
{
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

	Engine::CComponent* pComponent = m_mapComponent[Engine::ID_STATIC][L"Com_Mesh"];
	if (pComponent) {
		// ���� ���õ� �޽��� �ִٸ� �����Ѵ�.
		Engine::Safe_Release(pComponent);
	}

	// ���ο� �޽÷� �����Ѵ�.
	m_mapComponent[Engine::ID_STATIC][L"Com_Mesh"] = m_pMesh;
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

	// 1) �޽� �±�
	_int iStrLen = lstrlen(m_tcMeshTag);
	WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	WriteFile(_hfOut, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) ���� ���̵�
	Engine::RENDERID eRenderID = m_pRenderer->GetRenderID();
	WriteFile(_hfOut, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) Ʈ������ ���� ����
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

	// 1) �޽� �±� �ε�
	_int iStrLen = 0;
	ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	ReadFile(_hfIn, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	// 2) ���� ���̵�
	Engine::RENDERID eRenderID = Engine::RENDER_END;
	ReadFile(_hfIn, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 3) Ʈ������ ���� ����
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	return SetRenderInfo(m_tcMeshTag, eRenderID);
}