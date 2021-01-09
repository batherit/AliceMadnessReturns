#include "pch.h"
#include "TitleScene.h"
#include "StaticCamera.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "UI_Image.h"
#include "UI_Button.h"
#include "UI_FadeInOut.h"
#include "UI_OrthoPlane.h"
#include "Attribute.h"
#include "PlayScene.h"

CTitleScene::CTitleScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CTitleScene::CTitleScene(const CTitleScene & rhs)
	:
	CScene(rhs)
{
}

CTitleScene::~CTitleScene(void)
{
}

//LRESULT CTitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	
//	return LRESULT();
//}

void CTitleScene::ResetScene(void)
{

}

HRESULT CTitleScene::Ready(void)
{


	return S_OK;
}

int CTitleScene::Update(const _float& fTimeDelta)
{
	if (!m_pFadeInOutUI->IsFadeIn() && !m_pFadeInOutUI->IsProcessing()) {
		Engine::CManagement::GetInstance()->GetSceneMgr()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
	}

	return CScene::Update(fTimeDelta);
}

void CTitleScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	Engine::CCollisionMgr::GetInstance()->SetColliderVisible(false);

//	FAILED_CHECK_RETURN(Ready_Resource(Engine::RESOURCE_END), );
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), );

	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void CTitleScene::OnExited()
{
}

CTitleScene * CTitleScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitleScene*	pInstance = new CTitleScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

//HRESULT CTitleScene::Ready_LightInfo()
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

HRESULT CTitleScene::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);


	// 카메라 생성
	CStaticCamera* pCamera = CStaticCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pCamera, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pCamera), E_FAIL);
	//m_pCamera->GetTransform()->SetPos(0.2f, 1.5f, -0.4f);
	pCamera->GetTransform()->SetPos(0.f, 0.f, -1.5f);

	// 타이틀 백 플랜 생성
	CUI_OrthoPlane* pBackPlane = CUI_OrthoPlane::Create(m_pGraphicDev);
	pBackPlane->SetTexture(L"UI_AliceTitleBackPlane");
	pBackPlane->SetSize(WINCX, WINCY);
	pBackPlane->SetPos(WINCX >> 1, WINCY >> 1);
	pBackPlane->SetType(CUI_OrthoPlane::TYPE_BACKGROUND);
	pLayer->Add_GameObject(pBackPlane);

	//// 플레이어 생성
	CDynamicObject* pAlice = CDynamicObject::Create(m_pGraphicDev);
	pAlice->SetRenderInfo(L"AliceW");
	pAlice->GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_Idle);
	pAlice->GetTransform()->SetScaleXYZ(1.0f, 1.0f, 1.0f);
	pAlice->GetTransform()->SetPos(-0.2f, -1.4f, -0.5f);
	pAlice->GetTransform()->RotateByUp(D3DX_PI * 0.9f);
	pLayer->Add_GameObject(pAlice);

	// 빨간 곡선 생성
	CUI_Image* pRedCurve = CUI_Image::Create(m_pGraphicDev);
	pRedCurve->SetTexture(L"UI_RedCurve");
	pRedCurve->SetPos(static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), WINCY >> 1);
	pLayer->Add_GameObject(pRedCurve);

	// 버튼 생성
	m_pStartButton = CUI_Button<CTitleScene>::Create(m_pGraphicDev, this, static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) - WINCY /7.f), 222, 72);
	m_pExitButton = CUI_Button<CTitleScene>::Create(m_pGraphicDev, this, static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) + WINCY / 7.f), 180, 60);
	pLayer->Add_GameObject(m_pStartButton);
	pLayer->Add_GameObject(m_pExitButton);

	// 버튼 이벤트 연결
	m_pStartButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_CLICKED, &CTitleScene::ClickStartButton, nullptr);
	m_pStartButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_HOVERED, &CTitleScene::HoveredOnStartButton, nullptr);
	m_pStartButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_UNHOVERED, &CTitleScene::UnhoveredOnStartButton, nullptr);
	m_pExitButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_CLICKED, &CTitleScene::ClickQuitButton, nullptr);
	m_pExitButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_HOVERED, &CTitleScene::HoveredOnExitButton, nullptr);
	m_pExitButton->SetEvent(CUI_Button<CTitleScene>::BUTTON_STATE_UNHOVERED, &CTitleScene::UnhoveredOnExitButton, nullptr);

	// 시작 버튼 관련 이미지 생성
	m_pStartButtonOn = CUI_Image::Create(m_pGraphicDev);
	m_pStartButtonOn->SetTexture(L"UI_NewGameOn");
	m_pStartButtonOn->SetPos(static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) - WINCY / 7.f));
	m_pStartButtonOn->SetActivated(false);
	m_pStartButtonOff = CUI_Image::Create(m_pGraphicDev);
	m_pStartButtonOff->SetTexture(L"UI_NewGameOff");
	m_pStartButtonOff->SetPos(static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) - WINCY / 7.f));
	pLayer->Add_GameObject(m_pStartButtonOn);
	pLayer->Add_GameObject(m_pStartButtonOff);
	
	// 종료 버튼 관련 이미지 생성
	m_pExitButtonOn = CUI_Image::Create(m_pGraphicDev);
	m_pExitButtonOn->SetTexture(L"UI_GameExitOn");
	m_pExitButtonOn->SetPos(static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) + WINCY / 7.f));
	m_pExitButtonOn->SetActivated(false);
	m_pExitButtonOff = CUI_Image::Create(m_pGraphicDev);
	m_pExitButtonOff->SetTexture(L"UI_GameExitOff");
	m_pExitButtonOff->SetPos(static_cast<LONG>((WINCX >> 1) + WINCX / 6.f), static_cast<LONG>((WINCY >> 1) + WINCY / 7.f));
	pLayer->Add_GameObject(m_pExitButtonOn);
	pLayer->Add_GameObject(m_pExitButtonOff);

	m_pFadeInOutUI = CUI_FadeInOut::Create(m_pGraphicDev);
	pLayer->Add_GameObject(m_pFadeInOutUI);

	return S_OK;
}

void CTitleScene::Free(void)
{
	CScene::Free();
}

void CTitleScene::HoveredOnStartButton(void *)
{
	// 시작 버튼 온
	if (!m_bIsOnStartButtonHovered) {
		m_pStartButtonOn->SetActivated(true);
		m_pStartButtonOff->SetActivated(false);
		m_bIsOnStartButtonHovered = true;
	}
}

void CTitleScene::UnhoveredOnStartButton(void *)
{
	if (m_bIsOnStartButtonHovered) {
		// 시작 버튼 오프
		m_pStartButtonOn->SetActivated(false);
		m_pStartButtonOff->SetActivated(true);
		m_bIsOnStartButtonHovered = false;
	}
	
}

void CTitleScene::ClickStartButton(void *)
{
	if (m_pFadeInOutUI->IsProcessing())
		return;
	//Engine::CManagement::GetInstance()->GetSceneMgr()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
	m_pFadeInOutUI->StartFadeInOut(2.f, false);
}

void CTitleScene::HoveredOnExitButton(void *)
{
	// 종료 버튼 온
	if (!m_bIsOnExitButtonHovered) {
		m_pExitButtonOn->SetActivated(true);
		m_pExitButtonOff->SetActivated(false);
		m_bIsOnExitButtonHovered = true;
	}
}

void CTitleScene::UnhoveredOnExitButton(void *)
{
	if (m_bIsOnExitButtonHovered) {
		// 종료 버튼 오프
		m_pExitButtonOn->SetActivated(false);
		m_pExitButtonOff->SetActivated(true);
		m_bIsOnExitButtonHovered = false;
	}
}

void CTitleScene::ClickQuitButton(void *)
{
	if (m_pFadeInOutUI->IsProcessing())
		return;
	DestroyWindow(g_hWnd);
}
