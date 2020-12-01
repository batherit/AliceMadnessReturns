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
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, m_pMeshTag));
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pMesh);

	// Renderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	return S_OK;
}

int CStaticObject::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CStaticObject::Render_Object(void)
{
	m_pRenderer->Render();
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* _pMeshTag)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev);

	pInstance->m_pMeshTag = _pMeshTag;
	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CStaticObject::Free(void)
{
	CGameObject::Free();
}
