#ifndef Shark_h__
#define Shark_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CShark : public Engine::CGameObject
{

private: // 생성자, 소멸자
	explicit  CShark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CShark(const  CShark& rhs);
	virtual ~CShark(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CShark*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void Bang();

private:
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	CAttribute* m_pAttribute = nullptr;
	_float m_fDissolveAmount = 0.f;
	_float m_fAttackDelayTime = 0.f;
};

END

#endif // !LogoObject_h__
