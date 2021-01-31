#ifndef CannonBall_h__
#define CannonBall_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CCannonBall : public Engine::CGameObject
{
public:
	enum E_TYPE { TYPE_PLAYER, TYPE_MONSTER, TYPE_END };

private: // 생성자, 소멸자
	explicit  CCannonBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CCannonBall(const  CCannonBall& rhs);
	virtual ~CCannonBall(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CCannonBall*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetCannonBallInfo(E_TYPE _eType, const _vec3& _vPos, const _vec3& _vVelocity);
	E_TYPE GetType() const { return m_eType; }

private:
	E_TYPE m_eType = TYPE_END;

	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;

	Engine::CColliderObject* m_pAttackCollider = nullptr;
};

END

#endif // !LogoObject_h__
