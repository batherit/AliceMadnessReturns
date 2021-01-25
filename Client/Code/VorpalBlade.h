#ifndef VorpalBlade_h__
#define VorpalBlade_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "StaticObject.h"

BEGIN(Client)
class CAttribute;
class CVorpalBlade : public CStaticObject
{
private: // 생성자, 소멸자
	explicit  CVorpalBlade(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CVorpalBlade(const  CVorpalBlade& rhs);
	virtual ~CVorpalBlade(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CVorpalBlade*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CColliderObject* GetAttackCollider() const { return m_pAttackCollider; }
	
private:
	CAttribute* m_pAttribute = nullptr;
	Engine::CColliderObject* m_pAttackCollider = nullptr;
};

END

#endif // !LogoObject_h__
