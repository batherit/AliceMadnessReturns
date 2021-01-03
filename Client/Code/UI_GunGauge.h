#ifndef UI_GunGauge_h__
#define UI_GunGauge_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_GunGauge : public Engine::CGameObject
{
#define OFFSET_X 30
#define OFFSET_Y 30
private: // 생성자, 소멸자
	explicit  CUI_GunGauge(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_GunGauge(const  CUI_GunGauge& rhs);
	virtual ~CUI_GunGauge(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_GunGauge*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);
	void Init();
	void SetOverload(_bool _bIsOverloaded);
	_bool IsOverloaded() const { return m_bIsOverloaded; }

	void IncreaseGauge(_float _fAmount) { 
		if (m_bIsOverloaded) 
			return; 
		Engine::Clamp(&(m_fCurGauge += _fAmount), 0.f, 1.f);
		if (m_fCurGauge >= 1.f)
			SetOverload(true);
	}

	void DecreaseGauge(_float _fAmount) {
		if (m_bIsOverloaded)
			return;
		Engine::Clamp(&(m_fCurGauge -= _fAmount), 0.f, 1.f);
	}

private:
	Engine::CGameObject* m_pPlayer = nullptr;
	_float m_fCurGauge = 0.f;
	CUI_Image* m_pGunGauge = nullptr;
	CUI_Image* m_pGunCover = nullptr;
	CUI_Image* m_pGunOverload = nullptr;
	_bool m_bIsOverloaded = false;
	_float m_fElapsedTime = 0.f;
};

END

#endif // !LogoObject_h__