#ifndef UI_InGame_h__
#define UI_InGame_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_GunGauge;
class CUI_HPGauge;		// PlayerHP
class CUI_HPBar;		// BossHP
class CUI_Tooth;
class CUI_Targeting;
class CUI_TargetingMode;
class CUI_FadeInOut;
class CUI_BunnyBomb;
class CUI_WeaponLock;
class CUI_Cursor;
class CUI_BloodScreen;
class CUI_PurpleScreen;
class CUI_InGame : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_InGame(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_InGame(const  CUI_InGame& rhs);
	virtual ~CUI_InGame(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_InGame*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);
	CUI_Image* GetAimImage() const { return m_pAimImage; }
	CUI_GunGauge* GetGunGauge() const { return m_pGunGauge; }
	CUI_HPGauge* GetHPGauge() const { return m_pHPGauge; }
	CUI_HPBar* GetHPBar() const { return m_pHPBar; }
	CUI_Tooth* GetTooth() const { return m_pTooth; }
	CUI_Targeting* GetTargeting() const { return m_pTargeting; }
	CUI_TargetingMode* GetTargetingMode() const { return m_pTargetingMode; }
	CUI_FadeInOut* GetFadeInOut() const { return m_pFadeInOut; }
	CUI_BloodScreen* GetBloodScreen() const { return m_pBloodScreen; }
	CUI_PurpleScreen* GetPurpleScreen() const { return m_pPurpleScreen; }
	CUI_BunnyBomb* GetBunnyBomb() const { return m_pBunnyBomb; }
	CUI_WeaponLock* GetWeaponLock() const { return m_pWeaponLock; }
	CUI_Cursor* GetCursor() const { return m_pCursor; }

private:
	Engine::CGameObject* m_pPlayer = nullptr;
	CUI_Image* m_pAimImage = nullptr;
	CUI_GunGauge* m_pGunGauge = nullptr;
	CUI_HPGauge* m_pHPGauge = nullptr;
	CUI_HPBar* m_pHPBar = nullptr;
	CUI_Tooth* m_pTooth = nullptr;
	CUI_Targeting* m_pTargeting = nullptr;
	CUI_TargetingMode* m_pTargetingMode = nullptr;
	CUI_FadeInOut* m_pFadeInOut = nullptr;
	CUI_BloodScreen* m_pBloodScreen = nullptr;
	CUI_PurpleScreen* m_pPurpleScreen = nullptr;
	CUI_BunnyBomb* m_pBunnyBomb = nullptr;
	CUI_WeaponLock* m_pWeaponLock = nullptr;
	CUI_Image* m_pHelpImage = nullptr;
	CUI_Cursor* m_pCursor = nullptr;
};

END

#endif // !LogoObject_h__