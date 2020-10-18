#include "pch.h"
#include "CLogoScene.h"
//#include "CLogoObject.h"
//#include "CPlayScene.h"
//#include "CMonster.h"
#include "TerrainMap.h"

CLogoScene::CLogoScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CLogoScene::CLogoScene(const CLogoScene & rhs)
	:
	CScene(rhs)
{
}

CLogoScene::~CLogoScene(void)
{
}

LRESULT CLogoScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	
	return LRESULT();
}

void CLogoScene::ResetScene(void)
{
	
}

HRESULT CLogoScene::Ready(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}

int CLogoScene::Update(const _float& fTimeDelta)
{
	// TODO : 네모를 움직이는 코드를 작성합니다.
	//m_pLogoObject->Update_Object(fTimeDelta);
	//m_pMonster->Update_Object(fTimeDelta);

	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON)) {
		Engine::CManagement::GetInstance()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
	}*/

	return CScene::Update(fTimeDelta);
}

void CLogoScene::Render(void)
{
	//m_pLogoObject->Render_Object();
	//m_pMonster->Render_Object();
	CScene::Render();
}

CLogoScene * CLogoScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoScene*	pInstance = new CLogoScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CLogoScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	//m_pLogoObject = CLogoObject::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(m_pLogoObject, E_FAIL);
	//m_pMonster = CMonster::Create(m_pGraphicDev, m_pLogoObject);
	//NULL_CHECK_RETURN(m_pMonster, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pLogoObject), E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", m_pMonster), E_FAIL);

	// 카메라 생성
	m_pCamera = CCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	// 지형 생성
	m_pTerrain = CTerrainMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pTerrain, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", m_pTerrain), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLogoScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CLogoScene::Free(void)
{
	//Client::Safe_Release(m_pLogoObject);
	//Client::Safe_Release(m_pMonster);
	Client::Safe_Release(m_pCamera);
	Client::Safe_Release(m_pTerrain);
	CScene::Free();
}
