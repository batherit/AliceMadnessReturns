#include "pch.h"
#include "MiniGameScene.h"
#include "Play2Scene.h"
#include "Terrain.h"
#include "AliceW.h"
#include "Monster.h"
#include "MadCapA.h"
#include "MiniGrunt.h"
#include "BolterFly.h"
#include "Stone.h"
#include "Sword.h"
#include "SphereRenderer.h"
#include "SkyBox.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "CameraController_Player.h"
#include "CameraController_Crowd.h"
#include "CameraController_Sliding.h"
#include "CameraController_Target.h"
#include "CameraController_Gun.h"
#include "UI_InGame.h"
#include "Map.h"
#include "HPFlower.h"
#include "JumpPad.h"
#include "Tooth.h"
#include "Snail.h"
#include "Valve.h"
#include "PopDomino.h"
#include "UI_FadeInOut.h"
#include "PlateEffect.h"
#include "EffectMgr.h"
#include "Ship.h"
#include "Shark.h"
#include "Mine.h"
#include "UI_HPBar.h"
#include "UI_ShipProgress.h"
#include "UI_ToothShip.h"


#include "Attribute.h"

CMiniGameScene::CMiniGameScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CMiniGameScene::CMiniGameScene(const CMiniGameScene & rhs)
	:
	CScene(rhs)
{
}

CMiniGameScene::~CMiniGameScene(void)
{
}

//LRESULT CMiniGameScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CMiniGameScene::ResetScene(void)
{

}

HRESULT CMiniGameScene::Ready(void)
{


	return S_OK;
}

int CMiniGameScene::Update(const _float& fTimeDelta)
{
	static _bool m_bIsff = false;
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}

	if (m_pMap) {
		auto& rObjectList = m_pLayer->GetLayerList(L"MapObjects");
			
		for (auto& rObj : rObjectList) {
			rObj->GetTransform()->Translate(_vec3(-5.f * fTimeDelta, 0.f, 0.f));
		}
	}

	if (!m_bIsGameOver) {
		if ((m_fGenTime_Shark -= fTimeDelta) <= 0.f) {
			CShark* pShark = CShark::Create(m_pGraphicDev);
			pShark->GetTransform()->SetPos(_vec3(SEA_HALF_WIDTH + 10.f, Engine::GetNumberBetweenMinMax(-(SEA_HALF_HEIGHT - 5.f), SEA_HALF_HEIGHT - 5.f), 0.f));
			Engine::GetLayer(L"Environment")->Add_GameObject(L"Monster", pShark);
			m_fGenTime_Shark = Engine::GetNumberBetweenMinMax(1.f, 3.f);
		}

		if ((m_fGenTime_Mine -= fTimeDelta) <= 0.f) {
			CMine* pMine = CMine::Create(m_pGraphicDev);
			pMine->GetTransform()->SetPos(_vec3(SEA_HALF_WIDTH + 10.f, Engine::GetNumberBetweenMinMax(-(SEA_HALF_HEIGHT - 5.f), SEA_HALF_HEIGHT - 5.f), 0.f));
			Engine::GetLayer(L"Environment")->Add_GameObject(L"Monster", pMine);
			m_fGenTime_Mine = Engine::GetNumberBetweenMinMax(2.f, 4.f);
		}

		if ((m_fGameTime -= fTimeDelta) <= 0.f)
			m_fGameTime = 0.f;
		m_pShipProgress->UpdateProgress(1.f - (m_fGameTime / MINIGAME_TIME));

		if (m_pShip->GetAttribute()->IsDead() || m_fGameTime <= 0.f) {
			m_bIsGameOver = true;
			m_pFadeInOut->StartFadeInOut(2.f, false);
		}
	}
	else {
		if (!m_pFadeInOut->IsFadeIn() && !m_pFadeInOut->IsProcessing()) {
			Engine::CManagement::GetInstance()->GetSceneMgr()->SetNextScene(CPlay2Scene::Create(m_pGraphicDev));
		}
	}
	

	return CScene::Update(fTimeDelta);
}

//void CMiniGameScene::Render(void)
//{
//	Engine::Get_Renderer()->Render_GameObject();
//
//	//Engine::Render_Font(L"Font_Jinji", L"Text Test", &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
//	//CScene::Render();
//}

void CMiniGameScene::OnLoaded()
{
	//Engine::Set_TimeDelta(L"Timer_FPS60");
	//Engine::GetTimer(L"Timer_FPS60")->RunToPause();
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );
	FAILED_CHECK_RETURN(Ready_LightInfo(), );

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Engine::GetTimer(L"Timer_FPS60")->PauseToRun();

	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_SPHERE);
	Engine::CRenderer::GetInstance()->SetSphereFogInfo(40.f, 120.f, _vec3(0.f, 0.3f, 0.63f), 1.f);

	CSoundMgr::Get_Instance()->PlayBGM(L"Water.ogg");
	CSoundMgr::Get_Instance()->PlayBGM1(L"MiniGameScene_BGM.ogg");
	//CSoundMgr::Get_Instance()->PlayBGM(L"MiniGameScene_BGM.ogg");

	Engine::GetTimer(L"Timer_FPS60")->Reset();
}

void CMiniGameScene::OnExited()
{
	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_NONE);
	Engine::CRenderer::GetInstance()->SetMotionBlurOn(false);
	// 플레이어 데이터 저장
	auto* pDataMgr = CDataMgr::GetInstance();
	if (pDataMgr) {
		pDataMgr->SaveShipData(m_pShip);
		pDataMgr->SetValidData(true);
	}
	//CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->StopAll();
}

CMiniGameScene * CMiniGameScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniGameScene*	pInstance = new CMiniGameScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CMiniGameScene::Ready_LightInfo()
{
	/*D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;

	return S_OK;*/

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

HRESULT CMiniGameScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);
	m_pLayer = pLayer;

	// 맵 생성
	m_pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMap, E_FAIL);
	m_pMap->LoadMap(
		pLayer,
		nullptr,
		nullptr,
		L"../../Resource/Map/Map_MiniGame.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", m_pMap), E_FAIL);

	// 카메라 생성
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	m_pCamera->GetTransform()->SetPos(_vec3(0.f, 0.f, -50.f));

	// HP바 생성
	m_pHPBar = CUI_HPBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pHPBar, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", m_pHPBar), E_FAIL);

	// 배 생성
	m_pShip = CShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pShip, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ship", m_pShip), E_FAIL);
	m_pShip->GetTransform()->SetPos(-SEA_HALF_WIDTH + 5.f, 0.f, 0.f);
	m_pShip->SetHPBar(m_pHPBar);
	if (CDataMgr::GetInstance()->IsValidData()) {
		m_pShip->IncreaseToothNum(CDataMgr::GetInstance()->GetToothNum());
	}

	// 배 진행 UI 생성
	m_pShipProgress = CUI_ShipProgress::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pShipProgress, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", m_pShipProgress), E_FAIL);
	m_pShipProgress->GetTransform()->SetPos(WINCX >> 1, (WINCY * 0.1f), 0.f);
	
	// 이빨 UI 생성
	m_pToothShip = CUI_ToothShip::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pToothShip, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI", m_pToothShip), E_FAIL);
	m_pToothShip->SetShip(m_pShip);

	// 커서 없애기
	ShowCursor(false);

	CEffectMgr* pEffectMgr = CEffectMgr::Create(m_pGraphicDev, Engine::GetTimer(L"Timer_FPS60"));
	NULL_CHECK_RETURN(pEffectMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectMgr", pEffectMgr), E_FAIL);

	// FadeInOut
	m_pFadeInOut = CUI_FadeInOut::Create(m_pGraphicDev);
	pLayer->Add_GameObject(m_pFadeInOut);



	return S_OK;
}

HRESULT CMiniGameScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CMiniGameScene::Free(void)
{
	//Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	CScene::Free();
}
