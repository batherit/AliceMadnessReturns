#ifndef UI_TargetingMode_h__
#define UI_TargetingMode_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_TargetingMode : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_TargetingMode(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_TargetingMode(const  CUI_TargetingMode& rhs);
	virtual ~CUI_TargetingMode(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_TargetingMode*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);

private:
	Engine::CGameObject* m_pPlayer = nullptr;

	CUI_Image* m_pImageLT = nullptr;
	CUI_Image* m_pImageMT = nullptr;
	CUI_Image* m_pImageRT = nullptr;

	CUI_Image* m_pImageLB = nullptr;
	CUI_Image* m_pImageMB = nullptr;
	CUI_Image* m_pImageRB = nullptr;
};

END

#endif // !LogoObject_h__