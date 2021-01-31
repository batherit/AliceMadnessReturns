#ifndef UI_ShipProgress_h__
#define UI_ShipProgress_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CAttribute;
class CUI_ShipProgress : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_ShipProgress(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_ShipProgress(const  CUI_ShipProgress& rhs);
	virtual ~CUI_ShipProgress(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_ShipProgress*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void UpdateProgress(_float _fProgress);

private:
	CUI_Image* m_pProgress = nullptr;
	CUI_Image* m_pShip = nullptr;
};

END

#endif // !LogoObject_h__