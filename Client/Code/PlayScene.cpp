#include "pch.h"
#include "PlayScene.h"
#include "Terrain.h"
#include "AliceW.h"
#include "Monster.h"
#include "MadCapA.h"
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

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_O)) {
		//m_pPlayer->GetComponent<CAttribute>()->DecreaseHP(10.f);
		CUI_InGame* pUI = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
		pUI->GetFadeInOut()->StartFadeInOut(1.f, true);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_P)) {
		//m_pPlayer->GetComponent<CAttribute>()->IncreaseHP(10.f);
		CUI_InGame* pUI = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
		pUI->GetFadeInOut()->StartFadeInOut(1.f, false);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}

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
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );
	FAILED_CHECK_RETURN(Ready_LightInfo(), );

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
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
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

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
		L"../../Resource/Navi/NaviTest.navi",
		L"../../Resource/Map/Map.map"
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

	// 관중형 카메라 컨트롤러 생성
	pCameraController = CCameraController_Crowd::Create(m_pGraphicDev);
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
	pCameraMgr->AddCameraController(pCameraController);

	// 테스트 몬스터(MadCapA) 생성
	Engine::CGameObject* pGameObject = CMadCapA::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, 0.f));

	//pGameObject = CMadCapA::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, -2.f));

	//// 테스트 플라워 생성
	//pGameObject = CHPFlower::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(0.f, 1.f, 0.f));

	//// 테스트 점프패드 생성
	//pGameObject = CJumpPad::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, 0.f));

	//// 테스트 스네일 생성
	//pGameObject = CSnail::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(0.f, 1.f, -3.f));

	//// 테스트 이빨 생성
	//pGameObject = CTooth::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, -3.f));

	//// 테스트 밸브 생성
	//pGameObject = CValve::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, -5.f));

	//// 테스트 팝 도미노 생성
	//pGameObject = CPopDomino::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(0.f, 1.f, -5.f));
	//dynamic_cast<CPopDomino*>(pGameObject)->SetPlayer(m_pPlayer);


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

	CUI_InGame* pUIInGame = CUI_InGame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUIInGame, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_InGame", pUIInGame), E_FAIL);
	pUIInGame->SetPlayer(m_pPlayer);
	pUIInGame->GetFadeInOut()->StartFadeInOut(2.f, true);

	// 스카이 박스 생성
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);

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
