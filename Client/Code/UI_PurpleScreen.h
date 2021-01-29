#pragma once
#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CUI_PurpleScreen :
	public CUI_Image
{
private:
	explicit CUI_PurpleScreen(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_PurpleScreen(const CUI_PurpleScreen& rhs);
	virtual ~CUI_PurpleScreen();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& _fDeltaTime) override;
	virtual void Render_Object(void) override;

	static CUI_PurpleScreen* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);

public:
	void On();
	void Off();
};

END