#ifndef Alice_h__
#define Alice_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CAlice : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CAlice(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CAlice(const  CAlice& rhs);
	virtual ~CAlice(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CAlice*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	CMap* m_pMap = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
};

END

#endif // !LogoObject_h__
