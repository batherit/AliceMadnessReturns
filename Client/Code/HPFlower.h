#ifndef HPFlower_h__
#define HPFlower_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CHPFlower : public Engine::CGameObject
{
#define HP_RECOVERY_SPEED 8.f
private: // 생성자, 소멸자
	explicit  CHPFlower(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CHPFlower(const  CHPFlower& rhs);
	virtual ~CHPFlower(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CHPFlower*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }

private:
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	CAliceW* m_pAlice = nullptr;
	_bool m_bIsEating = false;
};

END

#endif // !LogoObject_h__