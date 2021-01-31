#ifndef UI_ToothShip_h__
#define UI_ToothShip_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "UI_Image.h"

BEGIN(Client)
class CShip;
class CUI_ToothShip : public CUI_Image
{
#define TOOTHSHIP_OFFSET_X 40
#define TOOTHSHIP_OFFSET_Y 40

private: // 생성자, 소멸자
	explicit  CUI_ToothShip(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_ToothShip(const  CUI_ToothShip& rhs);
	virtual ~CUI_ToothShip(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_ToothShip*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetShip(CShip* _pShip) { m_pShip = _pShip; }

private:
	CShip* m_pShip = nullptr;
};

END

#endif // !LogoObject_h__