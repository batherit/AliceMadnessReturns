#ifndef SphereRenderer_h__
#define SphereRenderer_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CSphereRenderer : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CSphereRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CSphereRenderer(const  CSphereRenderer& rhs);
	virtual ~CSphereRenderer(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CSphereRenderer*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CRenderer* m_pRenderer = nullptr;
	_vec3 m_vTargetPos{ 0.f, 0.f, 0.f };
	_float m_fT = 0.f;
};

END

#endif // !LogoObject_h__