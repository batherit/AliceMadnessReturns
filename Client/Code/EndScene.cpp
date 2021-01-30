#include "pch.h"
#include "EndScene.h"
#include "StaticCamera.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TitleAlice.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_FadeInOut.h"
#include "UI_OrthoPlane.h"
#include "UI_Cursor.h"
#include "Attribute.h"
#include "GiantAliceW.h"
#include "CameraController_Player.h"
//#include "PlayScene.h"
#include "PoolScene.h"
#include "Map.h"
#include "SkyBox.h"

CEndScene::CEndScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CEndScene::CEndScene(const CEndScene & rhs)
	:
	CScene(rhs)
{
}

CEndScene::~CEndScene(void)
{
}

//LRESULT CEndScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CEndScene::ResetScene(void)
{

}

HRESULT CEndScene::Ready(void)
{


	return S_OK;
}

int CEndScene::Update(const _float& fTimeDelta)
{

	return CScene::Update(fTimeDelta);
}

void CEndScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

	FAILED_CHECK_RETURN(Ready_LightInfo(), );
	//	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CEndScene::OnExited()
{
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
}

CEndScene * CEndScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEndScene*	pInstance = new CEndScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

//HRESULT CEndScene::Ready_LightInfo()
//{
//	D3DLIGHT9		tLightInfo;
//	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));
//
//	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
//
//	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
//
//	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);
//
//	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CEndScene::Ready_LightInfo()
{
	Engine::ClearLights();

	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tLightInfo.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);

	tLightInfo.Direction = _vec3(-1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEndScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		nullptr,
		L"../../Resource/Navi/Navi_Boss.navi",
		L"../../Resource/Map/Map_Boss.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);

	//// 카메라 매니져 생성
	Engine::CCameraMgr* pCameraMgr = Engine::CCameraMgr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraMgr", pCameraMgr), E_FAIL);

	// 카메라 생성
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	pCameraMgr->SetCamera(m_pCamera);
	m_pCamera->GetTransform()->SetPosZ(-30.f);
	m_pCamera->GetTransform()->SetPosY(3.f);

	// 플레이어 생성
	m_pPlayer = CGiantAliceW::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
	m_pPlayer->GetTransform()->SetPos(_vec3(10.f, 0.f, 0.f));
	//m_pPlayer->GetTransform()->SetPosY(1.f);

	//// 플레이어 카메라 컨트롤러 생성(0)
	//Engine::CCameraController* pCameraController = CCameraController_Player::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pCameraController, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	//static_cast<CCameraController_Player*>(pCameraController)->SetPlayer(m_pPlayer);
	//pCameraMgr->AddCameraController(pCameraController);
	//pCameraMgr->ChangeCameraController(0, 0.5f);

	CDynamicObject* pGameObject = CDynamicObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(0.f, 0.f, 0.f));
	pGameObject->SetRenderInfo(L"Boss");
	pGameObject->LoadCollidersInfo(L"EndBoss");
	pGameObject->GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Giant_Laugh03);
	//dynamic_cast<CBoss*>(pGameObject)->SetHPBarUI(pUIInGame->GetHPBar());

	// 스카이 박스 생성
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);
	m_pSkyBox->GetComponent<Engine::CPolygonRenderer>()->SetTextureIndex(0);

	return S_OK;
}

void CEndScene::Free(void)
{
	CScene::Free();
}