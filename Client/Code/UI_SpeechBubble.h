#pragma once
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_SpeechBubble : public Engine::CGameObject
{
protected:
	explicit CUI_SpeechBubble(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_SpeechBubble(const CUI_SpeechBubble & rhs);
	virtual ~CUI_SpeechBubble(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& _fDeltaTime) override;
	virtual void Render_Object(void) override;

	static CUI_SpeechBubble*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetText(_float _fOffsetX, _float _fOffsetY, const wstring& _wstrText, _float _fLifeTime = 3.5f);

private:
	Engine::CRenderer* m_pRenderer = nullptr;

	wstring m_wstrText = L"";
	_vec2 m_vOutputPos;
	_float m_fLifeTime = 0.f;
	_float m_fElapsedTime = 0.f;
};
END
