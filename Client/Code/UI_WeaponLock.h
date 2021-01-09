#ifndef UI_WeaponLock_h__
#define UI_WeaponLock_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CAliceW;
class CUI_Tooth;
class CUI_LockedWeapon;
class CUI_WeaponLock : public CUI_Image
{
private: // 생성자, 소멸자
	explicit  CUI_WeaponLock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_WeaponLock(const  CUI_WeaponLock& rhs);
	virtual ~CUI_WeaponLock(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_WeaponLock*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(CAliceW* _pAlice);
	CUI_LockedWeapon* GetVorpalBladeUI() const { return m_pVorpalBladeUI; }
	CUI_LockedWeapon* GetHobbyHorseUI() const { return m_pHobbyHorseUI; }
	CUI_LockedWeapon* GetGunUI() const { return m_pGunUI; }
	CUI_LockedWeapon* GetBunnyBombUI() const { return m_pBunnyBombUI; }

private:
	CAliceW* m_pAliceW = nullptr;
	CUI_Tooth* m_pToothUI = nullptr;		// 플레이어가 소지하고 있는 이빨의 수

	CUI_LockedWeapon* m_pVorpalBladeUI = nullptr;
	CUI_LockedWeapon* m_pHobbyHorseUI = nullptr;
	CUI_LockedWeapon* m_pGunUI = nullptr;
	CUI_LockedWeapon* m_pBunnyBombUI = nullptr;
};

END

#endif // !LogoObject_h__