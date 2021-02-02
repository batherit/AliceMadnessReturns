#include "pch.h"
#include "UI_InGame.h"
#include "UI_HPGauge.h"
#include "UI_HPBar.h"
#include "UI_Tooth.h"
#include "UI_Targeting.h"
#include "UI_TargetingMode.h"
#include "UI_Image.h"
#include "UI_GunGauge.h"
#include "UI_FadeInOut.h"
#include "UI_BloodScreen.h"
#include "UI_PurpleScreen.h"
#include "UI_BunnyBomb.h"
#include "UI_WeaponLock.h"
#include "UI_Cursor.h"
#include "Attribute.h"
#include "AliceW.h"

CUI_InGame::CUI_InGame(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_InGame::CUI_InGame(const CUI_InGame & rhs)
	:
	CGameObject(rhs)
{
}

CUI_InGame::~CUI_InGame(void)
{
}

HRESULT CUI_InGame::Ready_Object(void)
{
	m_pAimImage = CUI_Image::Create(m_pGraphicDev);
	m_pAimImage->SetTexture(L"UI_Aim");
	m_pAimImage->SetPos(WINCX >> 1, WINCY >> 1);
	m_pAimImage->SetActivated(false);
	AddChild(m_pAimImage);

	m_pGunGauge = CUI_GunGauge::Create(m_pGraphicDev);
	m_pGunGauge->SetActivated(false);
	AddChild(m_pGunGauge);

	// Player HP
	m_pHPGauge = CUI_HPGauge::Create(m_pGraphicDev);
	//m_pHPGauge->SetPlayer(m_pPlayer);
	AddChild(m_pHPGauge);

	m_pTooth = CUI_Tooth::Create(m_pGraphicDev);
	AddChild(m_pTooth);

	

	m_pTargeting = CUI_Targeting::Create(m_pGraphicDev);
	m_pTargeting->GetTransform()->SetPos(0.f, 10.f, 0.f);
	//m_pTargeting->SetPlayer(m_pPlayer);
	AddChild(m_pTargeting);

	m_pBunnyBomb = CUI_BunnyBomb::Create(m_pGraphicDev);
	m_pBunnyBomb->SetActivated(false);
	AddChild(m_pBunnyBomb);

	m_pPurpleScreen = CUI_PurpleScreen::Create(m_pGraphicDev);
	m_pPurpleScreen->Off();
	AddChild(m_pPurpleScreen);

	m_pTargetingMode = CUI_TargetingMode::Create(m_pGraphicDev);
	//m_pTargetingMode->SetPlayer(m_pPlayer);
	AddChild(m_pTargetingMode);

	m_pWeaponLock = CUI_WeaponLock::Create(m_pGraphicDev);
	m_pWeaponLock->SetActivated(false);
	AddChild(m_pWeaponLock);

	m_pBloodScreen = CUI_BloodScreen::Create(m_pGraphicDev);
	AddChild(m_pBloodScreen);
	
	// Boss HP
	m_pHPBar = CUI_HPBar::Create(m_pGraphicDev);
	m_pHPBar->Off();
	AddChild(m_pHPBar);

	m_pCursor = CUI_Cursor::Create(m_pGraphicDev);
	m_pCursor->SetActivated(false);
	Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(true, WINCX >> 1, WINCY >> 1);
	AddChild(m_pCursor);

	m_pHelpImage = CUI_Image::Create(m_pGraphicDev);
	m_pHelpImage->SetTexture(L"UI_Help");
	m_pHelpImage->SetPos(WINCX >> 1, WINCY >> 1);
	_float fRatio = static_cast<_float>(m_pHelpImage->GetHeight()) / m_pHelpImage->GetWidth();
	m_pHelpImage->SetOutputAreaWidth(WINCX * 1.2f);
	m_pHelpImage->SetOutputAreaHeight(m_pHelpImage->GetWidth() * fRatio);
	m_pHelpImage->SetVisible(false);
	AddChild(m_pHelpImage);

	// FadeInOut은 화면 전체를 덮어야하기 때문에 자식 중 가장 맨 뒤에 있어야 한다.
	m_pFadeInOut = CUI_FadeInOut::Create(m_pGraphicDev);
	AddChild(m_pFadeInOut);

	return S_OK;
}

_int CUI_InGame::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_C)) {
		if (m_pWeaponLock->IsActivated()) {
			Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(true, WINCX >> 1, WINCY >> 1);
			m_pCursor->SetActivated(false);
			m_pWeaponLock->SetActivated(false);
		}
		else {
			Engine::CDirectInputMgr::GetInstance()->SetMouseFixed(false, WINCX >> 1, WINCY >> 1);
			m_pCursor->SetActivated(true);
			m_pWeaponLock->SetActivated(true);
			CSoundMgr::Get_Instance()->PlaySound(L"Tutorial.ogg", CSoundMgr::UI);
		}
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_F1)) {
		if (!m_pHelpImage->IsVisible()) {
			m_pHelpImage->SetVisible(true);
			CSoundMgr::Get_Instance()->PlaySound(L"Tutorial.ogg", CSoundMgr::UI);
		}
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyUp(DIK_F1)) {
		if (m_pHelpImage->IsVisible())
			m_pHelpImage->SetVisible(false);
	}

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CUI_InGame::Render_Object(void)
{
}

CUI_InGame * CUI_InGame::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_InGame*	pInstance = new CUI_InGame(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_InGame::Free(void)
{
	CGameObject::Free();
}

void CUI_InGame::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
	m_pHPGauge->SetPlayer(m_pPlayer);
	m_pTargeting->SetPlayer(m_pPlayer);
	m_pTargetingMode->SetPlayer(m_pPlayer);
	m_pTooth->SetPlayer(dynamic_cast<CAliceW*>(m_pPlayer));
	m_pWeaponLock->SetPlayer(dynamic_cast<CAliceW*>(m_pPlayer));
}
