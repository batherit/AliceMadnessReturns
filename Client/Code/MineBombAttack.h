#ifndef MineBombAttack_h__
#define MineBombAttack_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "Attribute.h"

BEGIN(Client)
class CMineBombAttack : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CMineBombAttack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMineBombAttack(const  CMineBombAttack& rhs);
	virtual ~CMineBombAttack(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo);
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo);

public:
	static CMineBombAttack*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetMineBombAttackInfo(const _vec3& _vPos);

private:
	// 충돌체 세팅
	Engine::CColliderObject_Sphere* m_pAttackCollider = nullptr;
	_float m_fLifeTime = 0.5f;
};

END

#endif // !LogoObject_h__