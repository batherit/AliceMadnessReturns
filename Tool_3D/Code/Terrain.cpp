#include "pch.h"
#include "Terrain.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	:
	CGameObject(rhs)
{
}

CTerrain::~CTerrain(void)
{
}


HRESULT CTerrain::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();

	m_pTerrain = AddComponent<Engine::CTerrainTex>();
	m_pTerrain->SetTerrainInfo(129, 129, 129.f, 129.f, L"../Bin/Resource/Texture/Terrain/Height1.bmp");

//	pComponent = m_pTextureCom = ;
//	Engine::GetOriResource()
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent, );
	
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pTerrain, dynamic_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Height")));
	return S_OK;
}

int CTerrain::Update_Object(const _float & fTimeDelta)
{
	//m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRenderer->Update(fTimeDelta);

	return 0;
}

void CTerrain::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	//Engine::Render_Texture(Engine::RESOURCE_STATIC, L"Height", 0);
	//m_pTerrain->Render_Buffer();
	m_pRenderer->Render();
}

void CTerrain::CreateTerrain(_uint _iNumOfVerticesW, _uint _iNumOfVerticesH, _float _fWidth, _float _fHeight, const _tchar * _szHeightMapFileName)
{
	m_pTerrain->SetTerrainInfo(_iNumOfVerticesW, _iNumOfVerticesH, _fWidth, _fHeight, _szHeightMapFileName);
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*	pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	CGameObject::Free();
}
