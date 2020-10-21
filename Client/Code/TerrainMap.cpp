#include "pch.h"
#include "TerrainMap.h"


CTerrainMap::CTerrainMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTerrainMap::CTerrainMap(const CTerrainMap & rhs)
	:
	CGameObject(rhs)
{
}

CTerrainMap::~CTerrainMap(void)
{
}


HRESULT CTerrainMap::Ready_Object(void)
{
	AddComponent<Engine::CMoveComponent>();
	m_pMoveComponent = GetComponent<Engine::CMoveComponent>();

	AddComponent<Engine::CTerrain>();
	m_pTerrain = GetComponent<Engine::CTerrain>();
	m_pTerrain->SetTerrainInfo(_vec3(5.f, 5.f, 5.f), 129, 129, 129.f, 129.f, L"../Bin/Resource/Texture/Terrain/Height1.bmp");

	return S_OK;
}

int CTerrainMap::Update_Object(const _float & fTimeDelta)
{
	return 0;
}

void CTerrainMap::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pMoveComponent->GetObjectMatrix());

	Engine::Render_Texture(Engine::RESOURCE_STATIC, L"Height", 0);
	m_pTerrain->Render_Buffer();
}

CTerrainMap * CTerrainMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrainMap*	pInstance = new CTerrainMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CTerrainMap::Free(void)
{
	CGameObject::Free();
}
