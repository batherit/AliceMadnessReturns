#ifndef ToothShip_h__
#define ToothShip_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CShip;
class CToothShip : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CToothShip(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CToothShip(const  CToothShip& rhs);
	virtual ~CToothShip(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CToothShip*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CStaticMesh* GetDynamicMesh() const { return m_pMesh; }

public:
	void SetPopTooth(const _vec3& _vPos);

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CColliderObject* m_pCollider = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CShip* m_pShip = nullptr;
	_bool m_bIsPoped = false;
};

END

#endif // !LogoObject_h__