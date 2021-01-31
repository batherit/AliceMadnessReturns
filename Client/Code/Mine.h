#ifndef Mine_h__
#define Mine_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CMine : public Engine::CGameObject
{

private: // 생성자, 소멸자
	explicit  CMine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMine(const  CMine& rhs);
	virtual ~CMine(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CMine*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void Bang();

private:
	Engine::CColliderObject* m_pCollider = nullptr;
	Engine::CColliderObject* m_pDetect = nullptr;
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;

	CAttribute* m_pAttribute = nullptr;

	_bool m_bIsBang = false;
	_float m_fElapsedTime = 0.f;
};

END

#endif // !LogoObject_h__
