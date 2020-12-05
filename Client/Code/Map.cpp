#include "pch.h"
#include "Map.h"
#include "Terrain.h"


CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMap::CMap(const CMap & rhs)
	:
	CGameObject(rhs)
{
}

CMap::~CMap(void)
{
}


HRESULT CMap::Ready_Object(void)
{
	m_pNaviMesh = Engine::CNaviMesh::Create(m_pGraphicDev);
	m_mapComponent[Engine::ID_STATIC].emplace(L"COM_NAVIMESH", m_pNaviMesh);

	m_pTerrain = CTerrain::Create(m_pGraphicDev);

	AddChild(m_pTerrain);

	return S_OK;
}

int CMap::Update_Object(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	return 0;
}

void CMap::Render_Object(void)
{
}

CMap * CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap*	pInstance = new CMap(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMap::Free(void)
{
	CGameObject::Free();
}

void CMap::LoadMap(const _tchar * _pTerrainFilePath, const _tchar * _pNaviFilePath, const _tchar * _pObjectsFilePath)
{
	LoadTerrain(_pTerrainFilePath);
	LoadNaviMesh(_pNaviFilePath);
	LoadStaticObjects(_pObjectsFilePath);
}

void CMap::LoadTerrain(const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 터레인 파일을 읽기 위한 코드를 작성합니다.
}

void CMap::LoadNaviMesh(const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 네비메쉬 파일을 읽기 위한 코드를 작성합니다.
}

void CMap::LoadStaticObjects(const _tchar * _pFilePath)
{
	if (!_pFilePath)
		return;

	// TODO : 정적 객체 파일을 읽기 위한 코드를 작성합니다.
}
