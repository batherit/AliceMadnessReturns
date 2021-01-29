#ifndef UI_HPBar_h__
#define UI_HPBar_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CAttribute;
class CUI_HPBar : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_HPBar(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_HPBar(const  CUI_HPBar& rhs);
	virtual ~CUI_HPBar(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_HPBar*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	_bool On(Engine::CGameObject* _pTargetObject);
	void Off();
	void UpdateHPInfo(_float _fCurHP, _float _fMaxHP);

private:
	Engine::CGameObject* m_pTargetObject = nullptr;
	CAttribute* m_pTargetAttribute = nullptr;
	CUI_Image* m_pBackground = nullptr;
	CUI_Image* m_pHPGauge = nullptr;
	_float m_fSmoothGauge = 0.f;
	LONG m_iWidth = 1;
	LONG m_iHeight = 1;
};

END

#endif // !LogoObject_h__