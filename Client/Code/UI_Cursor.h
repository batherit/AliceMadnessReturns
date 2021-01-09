#ifndef UI_Cursor_h__
#define UI_Cursor_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CUI_Cursor : public CUI_Image
{
private: // 생성자, 소멸자
	explicit  CUI_Cursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_Cursor(const  CUI_Cursor& rhs);
	virtual ~CUI_Cursor(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_Cursor*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	CUI_Image* m_pCursorImage = nullptr;
	LONG m_iOffsetX = 0;
	LONG m_iOffsetY = 0;
};

END

#endif // !LogoObject_h__