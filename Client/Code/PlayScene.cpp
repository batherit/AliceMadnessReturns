#include "pch.h"
#include "PlayScene.h"
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
#include "CameraController_HPFlower.h"
#include "UI_InGame.h"
#include "Map.h"
#include "HPFlower.h"
#include "JumpPad.h"
#include "Tooth.h"
#include "Snail.h"
#include "Valve.h"
#include "PopDomino.h"
#include "UI_FadeInOut.h"
#include "EffectMgr.h"

#include "Attribute.h"

CPlayScene::CPlayScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CPlayScene::CPlayScene(const CPlayScene & rhs)
	:
	CScene(rhs)
{
}

CPlayScene::~CPlayScene(void)
{
}

//LRESULT CPlayScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CPlayScene::ResetScene(void)
{
	
}

HRESULT CPlayScene::Ready(void)
{
	

	return S_OK;
}

int CPlayScene::Update(const _float& fTimeDelta)
{
//	_float fDelta = fTimeDelta;
	//int a = 10;
	// TODO : 네모를 움직이는 코드를 작성합니다.
	//_float fHeight = m_pTerrain->GetHeight(m_pPlayer->GetComponent<Engine::CTransform>()->GetPos());
	//m_pPlayer->GetComponent<Engine::CTransform>()->SetPosY(fHeight);

	//if (Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree().z != 0) {
	//	_int iW = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetNumOfVerticesW();
	//	_int iH = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetNumOfVerticesH();
	//	if (Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree().z > 0) {
	//		m_pTerrain->CreateTerrain(iW + 1, iH + 1, 129.f, 129.f, 1.f, 1.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");
	//	}
	//	else {
	//		if(iW > 10 && iH > 10)
	//			m_pTerrain->CreateTerrain(iW - 1, iH - 1, 129.f, 129.f, 1.f, 1.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");
	//	}
	//}

	//if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
	//	// 픽킹을 하기 위한 기본 변수들 세팅.
	//	auto& pVertices = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetVertices();
	//	auto stPickingRayInfo = Engine::GetPickingRayInfo(m_pGraphicDev, Engine::GetClientCursorPoint(g_hWnd));
	//	_float fU, fV, fDist;
	//	_vec3 vV1, vV2, vV3;
	//	
	//	// 픽킹 검사를 진행한다.
	//	for (auto& pIndex : m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetIndexes()) {
	//		vV1 = pVertices[pIndex._0];
	//		vV2 = pVertices[pIndex._1];
	//		vV3 = pVertices[pIndex._2];
	//		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
	//			// 픽킹이 성공했다면, 픽킹 히트 지점을 플레이어 이동 지점으로 세팅
	//			//m_pPlayer->SetTartgetPos(stPickingRayInfo.vRayPos + stPickingRayInfo.vRayDir * fDist);
	//			m_pPlayer->SetTartgetPos(Engine::GetHitPos(vV1, vV2, vV3, fU, fV));
	//			break;
	//		}
	//	}
	//}	

	/*if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_0)) {
		Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
		pCameraMgr->ChangeCameraController(0, 0.1f);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_1)) {
		Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
		pCameraMgr->ChangeCameraController(2, 0.1f);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_2)) {
		Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
		pCameraMgr->ChangeCameraController(3, 0.1f);
	}
*/
	static _bool m_bIsff = false;
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}
	/*else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_O)) {
		if (!m_bIsff) {
			ShowCursor(true);
			Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(false);
			m_bIsff = true;
		}
		else {
			ShowCursor(false);
			Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(true);
			m_bIsff = false;
		}
	}*/

	//if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_O)) {
	//	//m_pPlayer->GetComponent<CAttribute>()->DecreaseHP(10.f);
	//	CUI_InGame* pUI = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
	//	pUI->GetFadeInOut()->StartFadeInOut(1.f, true);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_P)) {
	//	//m_pPlayer->GetComponent<CAttribute>()->IncreaseHP(10.f);
	//	CUI_InGame* pUI = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
	//	pUI->GetFadeInOut()->StartFadeInOut(1.f, false);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
	//	if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
	//		Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
	//	}
	//	else {
	//		Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
	//	}
	//}

	//if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_7)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_NONE);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_8)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_SPHERE);
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 400.f, _vec3(0.5f, 0.5f, 0.5f));
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_9)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_HEIGHT);
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-10.f, -300.f, _vec3(0.7f, 0.5f, 0.5f));
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_0)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_ALL);
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 400.f, _vec3(0.5f, 0.5f, 0.5f));
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-10.f, -300.f, _vec3(0.7f, 0.5f, 0.5f));
	//}

	return CScene::Update(fTimeDelta);
}

//void CPlayScene::Render(void)
//{
//	Engine::Get_Renderer()->Render_GameObject();
//
//	//Engine::Render_Font(L"Font_Jinji", L"Text Test", &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
//	//CScene::Render();
//}

void CPlayScene::OnLoaded()
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
	Engine::GetTimer(L"Timer_FPS60")->Reset();
}

void CPlayScene::OnExited()
{
}

CPlayScene * CPlayScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayScene*	pInstance = new CPlayScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CPlayScene::Ready_LightInfo()
{
	/*D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0)))
		return E_FAIL;*/

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

HRESULT CPlayScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	// 맵 생성
	CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		nullptr,
		L"../../Resource/Navi/Navi_Play.navi",
		L"../../Resource/Map/Map_Play.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);
	
	// 카메라 매니져 생성
	Engine::CCameraMgr* pCameraMgr = Engine::CCameraMgr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraMgr", pCameraMgr), E_FAIL);

	// 카메라 생성
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	pCameraMgr->SetCamera(m_pCamera);

	// 플레이어 생성
	m_pPlayer = CAliceW::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
	m_pPlayer->GetTransform()->SetPos(pMap->GetCurSpawnPoint());

	// 플레이어 카메라 컨트롤러 생성(0)
	Engine::CCameraController* pCameraController = CCameraController_Player::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Player*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);
	pCameraMgr->ChangeCameraController(0, 0.5f);

	// 슬라이딩 카메라 컨트롤러 생성(1)
	pCameraController = CCameraController_Sliding::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Sliding*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);
	//pCameraMgr->ChangeCameraController(0, 0.5f);

	// 타겟팅 카메라 컨트롤러 생성(2)
	pCameraController = CCameraController_Target::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Target*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);

	// 건모드 카메라 컨트롤러 생성(3)
	pCameraController = CCameraController_Gun::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Gun*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);

	// HPFlower 카메라 컨트롤러 생성(4)
	pCameraController = CCameraController_HPFlower::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_HPFlower*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);


	// 관중형 카메라 컨트롤러 생성
	/*pCameraController = CCameraController_Crowd::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Crowd*>(pCameraController)->SetTarget(m_pPlayer);
	pCameraController->GetTransform()->SetPos(_vec3(20.f, 10.f, 10.f));
	pCameraMgr->AddCameraController(pCameraController);

	pCameraController = CCameraController_Crowd::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Crowd*>(pCameraController)->SetTarget(m_pPlayer);
	pCameraController->GetTransform()->SetPos(_vec3(-10.f, 30.f, 10.f));
	pCameraMgr->AddCameraController(pCameraController);*/

	// 테스트 몬스터(MadCapA) 생성
	/*Engine::CGameObject* pGameObject = CMadCapA::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, 0.f));*/

	//pGameObject = CMadCapA::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, -2.f));


	// 테스트 미니 그룬트 생성
	/*pGameObject = CMiniGrunt::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, 0.f));*/

	// 테스트 나사파리 생성
	/*pGameObject = CBolterFly::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(4.f, 15.f, 0.f));

	pGameObject = CBolterFly::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(4.f, 15.f, 0.f));*/


	// UI
	/*CUI_HPGauge* pHPGauge = CUI_HPGauge::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pHPGauge, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_HPGauge", pHPGauge), E_FAIL);
	pHPGauge->SetPlayer(m_pPlayer);

	CUI_Targeting* pTargeting = CUI_Targeting::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pTargeting, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_Targeting", pTargeting), E_FAIL);
	pTargeting->GetTransform()->SetPos(0.f, 10.f, 0.f);
	pTargeting->SetPlayer(m_pPlayer);*/

	// 커서 없애기
	ShowCursor(false);

	CUI_InGame* pUIInGame = CUI_InGame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUIInGame, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_InGame", pUIInGame), E_FAIL);
	pUIInGame->SetPlayer(m_pPlayer);
	pUIInGame->GetFadeInOut()->StartFadeInOut(2.f, true);
	m_pPlayer->SetInGameUI(pUIInGame);

	CEffectMgr* pEffectMgr = CEffectMgr::Create(m_pGraphicDev, Engine::GetTimer(L"Timer_FPS60"));
	NULL_CHECK_RETURN(pEffectMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectMgr", pEffectMgr), E_FAIL);

	// 스카이 박스 생성
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);
	m_pSkyBox->GetComponent<Engine::CPolygonRenderer>()->SetTextureIndex(2);

	return S_OK;
}

HRESULT CPlayScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CPlayScene::Free(void)
{
	//Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	CScene::Free();
}
