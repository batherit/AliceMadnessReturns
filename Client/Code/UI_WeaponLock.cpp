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

	m_pVorpalBladeUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pVorpalBladeUI->SetWeaponTexture(L"UI_WeaponLock_VorpalBlade");
	m_pVorpalBladeUI->SetToothNum(0);
	m_pVorpalBladeUI->GetTransform()->SetPos(-WINCX / 5.f - WINCX / 10.f, 0.f, 0.f);
	m_pVorpalBladeUI->SetLocked(false);
	AddChild(m_pVorpalBladeUI);

	m_pHobbyHorseUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pHobbyHorseUI->SetWeaponTexture(L"UI_WeaponLock_HobbyHorse");
	m_pHobbyHorseUI->SetToothNum(20);
	m_pHobbyHorseUI->GetTransform()->SetPos(-WINCX / 10.f, 0.f, 0.f);
	AddChild(m_pHobbyHorseUI);

	m_pGunUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pGunUI->SetWeaponTexture(L"UI_WeaponLock_Gun");
	m_pGunUI->SetToothNum(20);
	m_pGunUI->GetTransform()->SetPos(WINCX / 10.f, 0.f, 0.f);
	AddChild(m_pGunUI);

	m_pBunnyBombUI = CUI_LockedWeapon::Create(m_pGraphicDev);
	m_pBunnyBombUI->SetWeaponTexture(L"UI_WeaponLock_BunnyBomb");
	m_pBunnyBombUI->SetToothNum(20);
	m_pBunnyBombUI->GetTransform()->SetPos(WINCX / 5.f + WINCX / 10.f, 0.f, 0.f);
	AddChild(m_pBunnyBombUI);

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
}

