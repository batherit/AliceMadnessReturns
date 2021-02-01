#ifndef DataMgr_h__
#define DataMgr_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
//class CGameObject;
class CAliceW;
class CShip;
class CUI_InGame;
class CDataMgr : public CBase
{
	DECLARE_SINGLETON(CDataMgr)
private:
	explicit CDataMgr(void);
	virtual ~CDataMgr(void);

public:
	void SetValidData(_bool _bIsValidData) { m_bIsValidData = _bIsValidData; }
	_bool IsValidData() const { return m_bIsValidData; }

	void SaveAliceWData(CAliceW* _pAliceW);
	void SaveShipData(CShip* _pShip);
	void SaveInGameUIData(CUI_InGame* _pInGameUI);
	_int GetToothNum() const { return m_iToothNum; }
	_int GetWeaponType() const { return m_iWeaponType; }
	_float GetCurHP() const { return m_fCurHP; }
	_float GetMaxHP() const { return m_fMaxHP; }
	_bool IsVorpalBladeLocked() const { return m_bIsVorpalBladeLocked; }
	_bool IsHobbyHorseLocked() const { return m_bIsHobbyHorseLocked; }
	_bool IsGunLocked() const { return m_bIsGunLocked; }
	_bool IsBunnyBombLocked() const { return m_bIsBunnyBombLocked; }

private:
	virtual void Free(void);

private:
	_int m_iWeaponType = 0;
	_bool m_bIsValidData = false;

	_int m_iToothNum = 0;
	_float m_fCurHP = 64.f;
	_float m_fMaxHP = 64.f;

	_bool m_bIsVorpalBladeLocked = false;
	_bool m_bIsHobbyHorseLocked = true;
	_bool m_bIsGunLocked = true;
	_bool m_bIsBunnyBombLocked = true;
};

END
#endif // DataMgr_h__
