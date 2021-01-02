#ifndef UI_Targeting_h__
#define UI_Targeting_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CUI_Targeting : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_Targeting(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_Targeting(const  CUI_Targeting& rhs);
	virtual ~CUI_Targeting(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_Targeting*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(Engine::CGameObject* _pPlayer);

private:
	Engine::CGameObject* m_pPlayer = nullptr;
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	_float m_fOffsetY = 3.f;
	_float m_fRadian = 0.f;
};

END

#endif // !LogoObject_h__