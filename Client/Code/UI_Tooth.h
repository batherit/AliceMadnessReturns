#ifndef UI_Tooth_h__
#define UI_Tooth_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CAliceW;
class CUI_Tooth : public CUI_Image
{
#define TOOTH_OFFSET_X 40
#define TOOTH_OFFSET_Y 120

private: // 생성자, 소멸자
	explicit  CUI_Tooth(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_Tooth(const  CUI_Tooth& rhs);
	virtual ~CUI_Tooth(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_Tooth*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(CAliceW* _pAlice) { m_pAliceW = _pAlice; }

private:
	CAliceW* m_pAliceW = nullptr;
};

END

#endif // !LogoObject_h__