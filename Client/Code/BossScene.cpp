#include "pch.h"
#include "BossScene.h"
#include "Terrain.h"
#include "AliceW.h"
#include "Monster.h"
#include "MadCapA.h"
#include "Boss.h"
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
#include "EffectMgr.h"
#include "Butterfly.h"
#include "UI_WeaponLock.h"
#include "UI_LockedWeapon.h"

#include "Attribute.h"

CBossScene::CBossScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CBossScene::CBossScene(const CBossScene & rhs)
	:
	CScene(rhs)
{
}

CBossScene::~CBossScene(void)
{
}

//LRESULT CBossScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CBossScene::ResetScene(void)
{

}

HRESULT CBossScene::Ready(void)
{


	return S_OK;
}

int CBossScene::Update(const _float& fTimeDelta)
{
	// TODO : �׸� �����̴� �ڵ带 �ۼ��մϴ�.
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
	//	// ��ŷ�� �ϱ� ���� �⺻ ������ ����.
	//	auto& pVertices = m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetVertices();
	//	auto stPickingRayInfo = Engine::GetPickingRayInfo(m_pGraphicDev, Engine::GetClientCursorPoint(g_hWnd));
	//	_float fU, fV, fDist;
	//	_vec3 vV1, vV2, vV3;
	//	
	//	// ��ŷ �˻縦 �����Ѵ�.
	//	for (auto& pIndex : m_pTerrain->GetComponent<Engine::CTerrainTex>()->GetIndexes()) {
	//		vV1 = pVertices[pIndex._0];
	//		vV2 = pVertices[pIndex._1];
	//		vV3 = pVertices[pIndex._2];
	//		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
	//			// ��ŷ�� �����ߴٸ�, ��ŷ ��Ʈ ������ �÷��̾� �̵� �������� ����
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

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}
	/*else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_7)) {
		Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_NONE);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_8)) {
		Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_SPHERE);
		Engine::CRenderer::GetInstance()->SetSphereFogInfo(3.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_9)) {
		Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_HEIGHT);
		Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_0)) {
		Engine::CRenderer::GetInstance()->SetFogType(Engine::CRenderer::FOG_ALL);
		Engine::CRenderer::GetInstance()->SetSphereFogInfo(3.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
		Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADPLUS)) {
		Engine::Clamp(&(m_fFogHeightDensity += 0.1f), 0.f, 1.f);
		Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADMINUS)) {
		Engine::Clamp(&(m_fFogHeightDensity -= 0.1f), 0.f, 1.f);
		Engine::CRenderer::GetInstance()->SetHeightFogInfo(-15.f, -30.f, _vec3(0.2f, 0.2f, 0.2f), m_fFogHeightDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADSTAR)) {
		Engine::Clamp(&(m_fFogSphereDensity += 0.1f), 0.f, 1.f);
		Engine::CRenderer::GetInstance()->SetSphereFogInfo(3.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_NUMPADSLASH)) {
		Engine::Clamp(&(m_fFogSphereDensity -= 0.1f), 0.f, 1.f);
		Engine::CRenderer::GetInstance()->SetSphereFogInfo(3.f, 100.f, _vec3(0.5f, 0.5f, 0.5f), m_fFogSphereDensity);
	}*/

	return CScene::Update(fTimeDelta);
}

//void CBossScene::Render(void)
//{
//	Engine::Get_Renderer()->Render_GameObject();
//
//	//Engine::Render_Font(L"Font_Jinji", L"Text Test", &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
//	//CScene::Render();
//}

void CBossScene::OnLoaded()
{
	Engine::GetTimer(L"Timer_FPS60")->RunToPause();
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
	Engine::GetTimer(L"Timer_FPS60")->PauseToRun();
}

void CBossScene::OnExited()
{
	// �÷��̾� ������ ����
	auto* pDataMgr = CDataMgr::GetInstance();
	if (pDataMgr) {
		pDataMgr->SaveAliceWData(m_pPlayer);
		pDataMgr->SaveInGameUIData(m_pUIInGame);
		pDataMgr->SetValidData(true);
	}
	Engine::CRenderer::GetInstance()->SetMotionBlurOn(false);
}

CBossScene * CBossScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossScene*	pInstance = new CBossScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

HRESULT CBossScene::Ready_LightInfo()
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

HRESULT CBossScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	// �� ����
	CMap* pMap = CMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pMap, E_FAIL);
	pMap->LoadMap(
		pLayer,
		nullptr,
		L"../../Resource/Navi/Navi_Boss.navi",
		L"../../Resource/Map/Map_Boss.map"
	);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pMap), E_FAIL);

	// ī�޶� �Ŵ��� ����
	Engine::CCameraMgr* pCameraMgr = Engine::CCameraMgr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraMgr", pCameraMgr), E_FAIL);

	// ī�޶� ����
	m_pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", m_pCamera), E_FAIL);
	pCameraMgr->SetCamera(m_pCamera);

	// �÷��̾� ����
	m_pPlayer = CAliceW::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", m_pPlayer), E_FAIL);
	m_pPlayer->GetTransform()->SetPos(pMap->GetCurSpawnPoint());

	// �÷��̾� ī�޶� ��Ʈ�ѷ� ����(0)
	Engine::CCameraController* pCameraController = CCameraController_Player::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Player*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);
	pCameraMgr->ChangeCameraController(0, 0.5f);

	// �����̵� ī�޶� ��Ʈ�ѷ� ����(1)
	pCameraController = CCameraController_Sliding::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Sliding*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);
	//pCameraMgr->ChangeCameraController(0, 0.5f);

	// Ÿ���� ī�޶� ��Ʈ�ѷ� ����(2)
	pCameraController = CCameraController_Target::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Target*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);

	// �Ǹ�� ī�޶� ��Ʈ�ѷ� ����(3)
	pCameraController = CCameraController_Gun::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	static_cast<CCameraController_Gun*>(pCameraController)->SetPlayer(m_pPlayer);
	pCameraMgr->AddCameraController(pCameraController);

	// ������ ī�޶� ��Ʈ�ѷ� ����
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

	//// �׽�Ʈ ����(MadCapA) ����
	//Engine::CGameObject* pGameObject = CMadCapA::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, 0.f));

	//pGameObject = CMadCapA::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(4.f, 55.f, -2.f));

	//// �׽�Ʈ �ö�� ����
	//pGameObject = CHPFlower::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(0.f, 1.f, 0.f));

	//// �׽�Ʈ �����е� ����
	//pGameObject = CJumpPad::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, 0.f));

	//// �׽�Ʈ ������ ����
	//pGameObject = CSnail::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(0.f, 1.f, -3.f));

	//// �׽�Ʈ �̻� ����
	//pGameObject = CTooth::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, -3.f));

	//// �׽�Ʈ ��� ����
	//pGameObject = CValve::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(pGameObject), E_FAIL);
	//pGameObject->GetTransform()->Translate(_vec3(3.f, 1.f, -5.f));

	//// �׽�Ʈ �� ���̳� ����
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

	m_pUIInGame = CUI_InGame::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pUIInGame, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UI_InGame", m_pUIInGame), E_FAIL);
	m_pUIInGame->SetPlayer(m_pPlayer);
	m_pUIInGame->GetFadeInOut()->StartFadeInOut(2.f, true);
	m_pPlayer->SetInGameUI(m_pUIInGame);

	if (CDataMgr::GetInstance()->IsValidData()) {
		CAttribute* pAttribute = m_pPlayer->GetComponent<CAttribute>();
		auto* pDataMgr = CDataMgr::GetInstance();
		pAttribute->SetHP(pDataMgr->GetCurHP(), pDataMgr->GetMaxHP());
		m_pPlayer->SetToothNum(pDataMgr->GetToothNum());
		m_pUIInGame->GetWeaponLock()->GetVorpalBladeUI()->SetLocked(pDataMgr->IsVorpalBladeLocked());
		m_pUIInGame->GetWeaponLock()->GetHobbyHorseUI()->SetLocked(pDataMgr->IsHobbyHorseLocked());
		m_pUIInGame->GetWeaponLock()->GetGunUI()->SetLocked(pDataMgr->IsGunLocked());
		m_pUIInGame->GetWeaponLock()->GetBunnyBombUI()->SetLocked(pDataMgr->IsBunnyBombLocked());
		m_pPlayer->SetWeaponType(static_cast<CAliceW::E_WEAPON_TYPE>(pDataMgr->GetWeaponType()));
	}

	Engine::CGameObject* pGameObject = CBoss::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	pGameObject->GetTransform()->Translate(_vec3(0.f, 10.f, 0.f));
	dynamic_cast<CBoss*>(pGameObject)->SetHPBarUI(m_pUIInGame->GetHPBar());

	CEffectMgr* pEffectMgr = CEffectMgr::Create(m_pGraphicDev, Engine::GetTimer(L"Timer_FPS60"));
	NULL_CHECK_RETURN(pEffectMgr, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectMgr", pEffectMgr), E_FAIL);

	// ��ī�� �ڽ� ����
	m_pSkyBox = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pSkyBox, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", m_pSkyBox), E_FAIL);
	m_pSkyBox->GetComponent<Engine::CPolygonRenderer>()->SetTextureIndex(0);

	// �׽�Ʈ ���� ����
	/*CButterfly* pButterfly = CButterfly::Create(m_pGraphicDev);
	pButterfly->SetButterflyInfo(_vec3(0.f, 2.f, 0.f), WORLD_X_AXIS, 0.5f, 1.f, 100.f);
	NULL_CHECK_RETURN(pButterfly, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(pButterfly), E_FAIL);*/

	return S_OK;
}

HRESULT CBossScene::Ready_Resource(Engine::RESOURCETYPE eType)
{
	return S_OK;
}

void CBossScene::Free(void)
{
	//Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	CScene::Free();
}
