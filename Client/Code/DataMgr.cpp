#include "pch.h"
#include "DataMgr.h"
#include "AliceW.h"
#include "Ship.h"
#include "UI_InGame.h"
#include "Attribute.h"
#include "UI_WeaponLock.h"
#include "UI_LockedWeapon.h"

USING(Client)
IMPLEMENT_SINGLETON(CDataMgr)

CDataMgr::CDataMgr(void)
{

}

CDataMgr::~CDataMgr(void)
{
	Free();
}

void CDataMgr::SaveAliceWData(CAliceW * _pAliceW)
{
	m_iWeaponType = _pAliceW->GetWeaponType();
	m_iToothNum = _pAliceW->GetToothNum();
	CAttribute* pAttribute = _pAliceW->GetComponent<CAttribute>();
	m_fCurHP = pAttribute->GetHP();
	m_fMaxHP = pAttribute->GetMaxHP();
}

void CDataMgr::SaveShipData(CShip* _pShip) {
	m_iToothNum = _pShip->GetToothNum();
}

void CDataMgr::SaveInGameUIData(CUI_InGame * _pInGameUI)
{
	m_bIsVorpalBladeLocked = _pInGameUI->GetWeaponLock()->GetVorpalBladeUI()->IsLocked();
	m_bIsHobbyHorseLocked = _pInGameUI->GetWeaponLock()->GetHobbyHorseUI()->IsLocked();
	m_bIsGunLocked = _pInGameUI->GetWeaponLock()->GetGunUI()->IsLocked();
	m_bIsBunnyBombLocked = _pInGameUI->GetWeaponLock()->GetBunnyBombUI()->IsLocked();
}

void CDataMgr::Free(void)
{
}