#ifndef UI_Aim_h__
#define UI_Aim_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_Aim : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_Aim(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_Aim(const  CUI_Aim& rhs);
	virtual ~CUI_Aim(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_Aim*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);

private:
	CUI_Image* m_pAimImage = nullptr;
};

END

#endif // !LogoObject_h__