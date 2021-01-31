#include "pch.h"
#include "MiniGameScene.h"
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
#include "UI_HPBar.h"

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
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_9)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_HEIGHT);
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_0)) {
	//	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_ALL);
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADPLUS)) {
	//	Engine::Clamp(&(m_fFogHeightDensity += 0.1f), 0.f, 1.f);
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADMINUS)) {
	//	Engine::Clamp(&(m_fFogHeightDensity -= 0.1f), 0.f, 1.f);
	//	Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADSTAR)) {
	//	Engine::Clamp(&(m_fFogSphereDensity += 0.1f), 0.f, 1.f);
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADSLASH)) {
	//	Engine::Clamp(&(m_fFogSphereDensity -= 0.1f), 0.f, 1.f);
	//	Engine::CRenderer::GetInstance()->SetSphereFogInfo(10.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	//}
	//else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_BACKSLASH)) {
	//	CPlateEffect* pEffect = CPlateEffect::Create(m_pGraphicDev);
	//	pEffect->SetPlateEffectInfo(L"ETF_HobbyHorseAttack", _vec3(14.f, -9.f, -11.f), _vec2(0.5f, 0.5f), _vec2(0.8f, 0.8f), 0.f, 0.2f, _vec3(1.f, 1.f, 1.f));
	//	Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);

	//	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	//	pCameraMgr->GetCamera()->Shake(0.3f, 0.4f, 20);
	//}

	if ((m_fGenTime -= fTimeDelta) <= 0.f) {
		CShark* pShark = CShark::Create(m_pGraphicDev);
		pShark->GetTransform()->SetPos(_vec3(SEA_HALF_WIDTH + 10.f, Engine::GetNumberBetweenMinMax(-(SEA_HALF_HEIGHT - 5.f), SEA_HALF_HEIGHT - 5.f), 0.f));
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Monster", pShark);
		m_fGenTime = Engine::GetNumberBetweenMinMax(2.f, 4.f);
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
	Engine::CRenderer::GetInstance()->SetSphereFogInfo(40.f, 100.f, _vec3(0.f, 0.3f, 0.63f), 1.f);

	Engine::GetTimer(L"Timer_FPS60")->Reset();
}

void CMiniGameScene::OnExited()
{
	Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_NONE);
	// 플레이어 데이터 저장
	/*auto* pDataMgr = CDataMgr::GetInstance();
	if (pDataMgr) {
		pDataMgr->SaveAliceWData(m_pPlayer);
		pDataMgr->SetValidData(true);
	}*/
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

	// 맵 생성
	/*CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		nullptr,
		L"../../Resource/Navi/Navi_Play2.navi",
		L"../../Resource/Map/Map_Play2.map"
	);*/
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);

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
	m_pShip->SetHPBar(m_pHPBar);
	

	// 커서 없애기
	ShowCursor(false);

	CEffectMgr* pEffectMgr = CEffectMgr::Create(m_pGraphicDev, Engine::GetTimer(L"Timer_FPS60"));
	NULL_CHECK_RETURN(pEffectMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectMgr", pEffectMgr), E_FAIL);



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
