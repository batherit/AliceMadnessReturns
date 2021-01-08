#ifndef UI_BunnyBomb_h__
#define UI_BunnyBomb_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Image;
class CUI_BunnyBomb : public Engine::CGameObject
{
#define OFFSET_X 30
#define OFFSET_Y 30
private: // 생성자, 소멸자
	explicit  CUI_BunnyBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_BunnyBomb(const  CUI_BunnyBomb& rhs);
	virtual ~CUI_BunnyBomb(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_BunnyBomb*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetProgress(_float _fProgress);

private:
	CUI_Image* m_pBunnyBomb = nullptr;
	CUI_Image* m_pArrow = nullptr;
	_float m_fProgress = 0.f;
};

END

#endif // !LogoObject_h__