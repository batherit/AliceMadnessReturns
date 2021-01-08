#ifndef UI_InGame_h__
#define UI_InGame_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_GunGauge;
class CUI_HPGauge;
class CUI_Targeting;
class CUI_FadeInOut;
class CUI_BunnyBomb;
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
	CUI_Targeting* GetTargeting() const { return m_pTargeting; }
	CUI_FadeInOut* GetFadeInOut() const { return m_pFadeInOut; }
	CUI_BunnyBomb* GetBunnyBomb() const { return m_pBunnyBomb; }

private:
	Engine::CGameObject* m_pPlayer = nullptr;
	CUI_Image* m_pAimImage = nullptr;
	CUI_GunGauge* m_pGunGauge = nullptr;
	CUI_HPGauge* m_pHPGauge = nullptr;
	CUI_Targeting* m_pTargeting = nullptr;
	CUI_FadeInOut* m_pFadeInOut = nullptr;
	CUI_BunnyBomb* m_pBunnyBomb = nullptr;
};

END

#endif // !LogoObject_h__