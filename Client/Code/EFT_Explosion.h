#ifndef EFT_Explosion_h__
#define EFT_Explosion_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CEFT_Explosion : public Engine::CGameObject
{
#define EXPLOSION_TIME 1.f
private: // 생성자, 소멸자
	explicit  CEFT_Explosion(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_Explosion(const  CEFT_Explosion& rhs);
	virtual ~CEFT_Explosion(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_Explosion*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetExplosionInfo(const _vec3& _vPos);

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;

	_float m_fElapsedTime = 0.f;
};

END

#endif // !LogoObject_h__