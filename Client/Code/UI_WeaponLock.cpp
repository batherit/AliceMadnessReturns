#include "pch.h"
#include "UI_WeaponLock.h"
#include "UI_Tooth.h"
#include "UI_LockedWeapon.h"
#include "AliceW.h"

CUI_WeaponLock::CUI_WeaponLock(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
}

CUI_WeaponLock::CUI_WeaponLock(const CUI_WeaponLock & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_WeaponLock::~CUI_WeaponLock(void)
{
}

HRESULT CUI_WeaponLock::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"UI_WeaponLock_Board");
	SetOutputArea(RECT{ 0, 0, WINCX, WINCY });

	m_pToothUI = CUI_Tooth::Create(m_pGraphicDev);
	m_pToothUI->SetPos(-(WINCX >> 1) + 100, (WINCY >> 1) - 120);
	AddChild(m_pToothUI);

	CUI_Image* pHelpImage = nullptr;
	m_pVorpalBladeUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pVorpalBladeUI->SetWeaponTexture(L"UI_WeaponLock_VorpalBlade");
	m_pVorpalBladeUI->SetToothNum(0);
	m_pVorpalBladeUI->GetTransform()->SetPos(-WINCX / 5.f - WINCX / 10.f, 0.f, 0.f);
	m_pVorpalBladeUI->SetLocked(false);
	pHelpImage = CUI_Image::Create(m_pGraphicDev);
	pHelpImage->SetTexture(L"UI_Help_VorpalBlade");
	pHelpImage->SetPos(-WINCX / 5.f - WINCX / 10.f + 250, 0.f);
	pHelpImage->SetOutputAreaWidth(pHelpImage->GetWidth() * 0.5f);
	pHelpImage->SetOutputAreaHeight(pHelpImage->GetHeight() * 0.5f);
	pHelpImage->SetVisible(false);
	m_pVorpalBladeUI->SetHelpImage(pHelpImage);
	AddChild(m_pVorpalBladeUI);//m_pVorpalBladeUI->SetHelpImage(L"UI_Help_VorpalBlade", -WINCX / 5.f - WINCX / 10.f, 0.f);

	m_pHobbyHorseUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pHobbyHorseUI->SetWeaponTexture(L"UI_WeaponLock_HobbyHorse");
	m_pHobbyHorseUI->SetToothNum(50);
	m_pHobbyHorseUI->GetTransform()->SetPos(-WINCX / 10.f , 0.f, 0.f);
	pHelpImage = CUI_Image::Create(m_pGraphicDev);
	pHelpImage->SetTexture(L"UI_Help_HobbyHorse");
	pHelpImage->SetPos(-WINCX / 10.f+ 250, 0.f);
	pHelpImage->SetOutputAreaWidth(pHelpImage->GetWidth() * 0.5f);
	pHelpImage->SetOutputAreaHeight(pHelpImage->GetHeight() * 0.5f);
	pHelpImage->SetVisible(false);
	m_pHobbyHorseUI->SetHelpImage(pHelpImage);
	AddChild(m_pHobbyHorseUI);//m_pHobbyHorseUI->SetHelpImage(L"UI_Help_HobbyHorse", -WINCX / 10.f, 0);

	m_pGunUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pGunUI->SetWeaponTexture(L"UI_WeaponLock_Gun");
	m_pGunUI->SetToothNum(50);
	m_pGunUI->GetTransform()->SetPos(WINCX / 10.f , 0.f, 0.f);
	pHelpImage = CUI_Image::Create(m_pGraphicDev);
	pHelpImage->SetTexture(L"UI_Help_Gun");
	pHelpImage->SetPos(WINCX / 10.f- 250, 0.f);
	pHelpImage->SetOutputAreaWidth(pHelpImage->GetWidth() * 0.5f);
	pHelpImage->SetOutputAreaHeight(pHelpImage->GetHeight() * 0.5f);
	pHelpImage->SetVisible(false);
	m_pGunUI->SetHelpImage(pHelpImage);
	AddChild(m_pGunUI);//m_pGunUI->SetHelpImage(L"UI_Help_Gun", 0, 0);

	m_pBunnyBombUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pBunnyBombUI->SetWeaponTexture(L"UI_WeaponLock_BunnyBomb");
	m_pBunnyBombUI->SetToothNum(50);
	m_pBunnyBombUI->GetTransform()->SetPos(WINCX / 5.f + WINCX / 10.f , 0.f, 0.f);
	pHelpImage = CUI_Image::Create(m_pGraphicDev);
	pHelpImage->SetTexture(L"UI_Help_BunnyBomb");
	pHelpImage->SetPos(WINCX / 5.f + WINCX / 10.f- 250, 0.f);
	pHelpImage->SetOutputAreaWidth(pHelpImage->GetWidth() * 0.5f);
	pHelpImage->SetOutputAreaHeight(pHelpImage->GetHeight() * 0.5f);
	pHelpImage->SetVisible(false);
	m_pBunnyBombUI->SetHelpImage(pHelpImage);
	AddChild(m_pBunnyBombUI);//m_pBunnyBombUI->SetHelpImage(L"UI_Help_BunnyBomb", 0, 0);

	AddChild(m_pVorpalBladeUI->GetHelpImage());
	AddChild(m_pHobbyHorseUI->GetHelpImage());
	AddChild(m_pGunUI->GetHelpImage());
	AddChild(m_pBunnyBombUI->GetHelpImage());

	return S_OK;
}

int CUI_WeaponLock::Update_Object(const _float & _fDeltaTime)
{
	CUI_Image::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_WeaponLock::Render_Object(void)
{
	CUI_Image::Render_Object();
}

CUI_WeaponLock * CUI_WeaponLock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_WeaponLock*	pInstance = new CUI_WeaponLock(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_WeaponLock::Free(void)
{
	CUI_Image::Free();
}

void CUI_WeaponLock::SetPlayer(CAliceW * _pAlice)
{
	if (!_pAlice)
		return;

	m_pAliceW = _pAlice;
	m_pToothUI->SetPlayer(_pAlice);
	m_pVorpalBladeUI->SetPlayer(_pAlice);
	m_pHobbyHorseUI->SetPlayer(_pAlice);
	m_pGunUI->SetPlayer(_pAlice);
	m_pBunnyBombUI->SetPlayer(_pAlice);
}

