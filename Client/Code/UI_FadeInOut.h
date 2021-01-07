#pragma once
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CUI_FadeInOut :
	public CUI_Image
{
private:
	explicit CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_FadeInOut(const CUI_FadeInOut& rhs);
	virtual ~CUI_FadeInOut();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& _fDeltaTime) override;
	virtual void Render_Object(void) override;

	static CUI_FadeInOut* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);

public:
	void StartFadeInOut(_float _fKeepTime, _bool _bIsFadeIn);
	_bool IsFadeIn() const { return m_bIsFadeIn; }
	_float GetProgress() const { return (m_fMaxKeepTime - m_fKeepTime) / m_fMaxKeepTime; }

public:
	//_bool m_bIsEffectStarting = false;
	_bool m_bIsFadeIn = true;
	_float m_fKeepTime = 0.f;
	_float m_fMaxKeepTime = 0.f;
};

END
