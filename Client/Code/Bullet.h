#ifndef Bullet_h__
#define Bullet_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "StaticObject.h"

BEGIN(Client)

class CBullet : public CStaticObject
{
private: // 생성자, 소멸자
	explicit  CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CBullet(const  CBullet& rhs);
	virtual ~CBullet(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	virtual void OnCollision(Engine::CollisionInfo) override;

public:
	static CBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetBulletInfo(_vec3 _vPos, _vec3 _vDir, _float _fSpeed = 60.f, _float _fDeathLength = 200.f);

private:
	Engine::CPhysics* m_pPhysics = nullptr;
	_float m_fDeathLength = 100.f;
};

END

#endif // !LogoObject_h__
