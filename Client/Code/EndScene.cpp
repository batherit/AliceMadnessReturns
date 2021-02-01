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
#include "CameraController_Giant.h"
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
	if (m_pBoss && m_pPlayer) {
		// 보스는 항상 플레이어를 바라보도록 한다.
		_vec3 vToTargetDir = m_pPlayer->GetTransform()->GetPos() - m_pBoss->GetTransform()->GetPos();
		_vec3 vLook = m_pBoss->GetTransform()->GetLook();
		_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
		D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
		_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
		_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
		m_pBoss->GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		if (Engine::CCollisionMgr::GetInstance()->IsColliderVisible()) {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);
		}
		else {
			Engine::CCollisionMgr::GetInstance()->SetColliderVisible(true);
		}
	}

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
	Engine::CRenderer::GetInstance()->SetMotionBlurOn(false);
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
	m_pPlayer->GetTransform()->RotateByUp(-D3DX_PI * 0.5f);
	//m_pPlayer->GetTransform()->SetPosY(1.f);

	// 보스 생성
	m_pBoss = CDynamicObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pBoss, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", m_pBoss), E_FAIL);
	m_pBoss->GetTransform()->Translate(_vec3(0.f, 0.f, 0.f));
	m_pBoss->SetRenderInfo(L"Boss");
	Engine::CCollisionMgr::GetInstance()->ExtractGameObject(m_pBoss);
	m_pBoss->LoadCollidersInfo(L"EndBoss");
	m_pBoss->GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Giant_Laugh03);
	//dynamic_cast<CBoss*>(pGameObject)->SetHPBarUI(pUIInGame->GetHPBar());

	//// 플레이어 카메라 컨트롤러 생성(0)
	CCameraController_Giant* pCameraController = CCameraController_Giant::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCameraController, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CameraController", pCameraController), E_FAIL);
	pCameraController->SetTargetObject(m_pPlayer->GetHead());
	pCameraController->SetBenchmarkObject(m_pBoss);
	pCameraMgr->AddCameraController(pCameraController);
	pCameraMgr->ChangeCameraController(0, 0.5f);

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