#pragma once
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CUI_BloodScreen :
	public CUI_Image
{
private:
	explicit CUI_BloodScreen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_BloodScreen(const CUI_BloodScreen& rhs);
	virtual ~CUI_BloodScreen();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& _fDeltaTime) override;
	virtual void Render_Object(void) override;

	static CUI_BloodScreen* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);

public:
	void StartBloodScreen();

public:
	//_bool m_bIsEffectStarting = false;
	/*_bool m_bIsFadeIn = true;
	_float m_fKeepTime = 0.f;
	_float m_fMaxKeepTime = 1.f;*/
	_float m_fElapsedTime = 0.f;
	_bool m_bIsFadeIn = false;
	_bool m_bIsProcessing = false;
};

END
