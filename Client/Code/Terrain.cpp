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

	//m_pTexture = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_SkyBox"));
	//m_mapComponent[Engine::CTexture::GetComponentID()].emplace(Engine::CTexture::GetComponentTag(), m_pTexture);
	//dynamic_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Height"));

	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pTerrain, nullptr);
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

void CTerrain::CreateTerrain(_uint _iNumOfVerticesW, _uint _iNumOfVerticesD, _float _fWidth, _float _fDepth, _float _fU, _float _fV, const _tchar * _szHeightMapFileName)
{
	m_pTerrain->SetTerrainInfo(_iNumOfVerticesW, _iNumOfVerticesD, _fWidth, _fDepth, _fU, _fV, _szHeightMapFileName);
}

void CTerrain::SetHeightMapFileName(const _tchar * _szHeightMapFileName)
{
	if (!_szHeightMapFileName)
		return;
	m_szHeightMapFileName = _szHeightMapFileName;
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		_szHeightMapFileName);
}

void CTerrain::SetVtxNum(_uint _iNumOfVerticesW, _uint _iNumOfVerticesD)
{
	if (_iNumOfVerticesW < 2)
		_iNumOfVerticesW = 2;
	if (_iNumOfVerticesD < 2)
		_iNumOfVerticesD = 2;
	m_pTerrain->SetTerrainInfo(
		_iNumOfVerticesW,
		_iNumOfVerticesD,
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		m_szHeightMapFileName);
}

void CTerrain::SetSize(_float _fWidth, _float _fDepth)
{
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		_fWidth,
		_fDepth,
		m_pTerrain->GetU(),
		m_pTerrain->GetV(),
		m_szHeightMapFileName);
}

void CTerrain::SetUV(_float _fU, _float _fV)
{
	m_pTerrain->SetTerrainInfo(
		m_pTerrain->GetNumOfVerticesW(),
		m_pTerrain->GetNumOfVerticesH(),
		m_pTerrain->GetWidth(),
		m_pTerrain->GetHeight(),
		_fU,
		_fV,
		m_szHeightMapFileName);
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
