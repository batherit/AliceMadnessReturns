#ifndef Ship_h__
#define Ship_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAttribute;
class CUI_HPBar;
class CShip : public Engine::CGameObject
{

private: // 생성자, 소멸자
	explicit  CShip(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CShip(const  CShip& rhs);
	virtual ~CShip(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CShip*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetHPBar(CUI_HPBar* _pHPBar);
	void Bang();
	void Attacked();
	_bool IsAttacked() const { return m_bIsAttacked; }

private:
	_bool m_bIsAttacked = false;
	_float m_fRecoveryTime = 0.f;
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CAttribute* m_pAttribute = nullptr;

	CUI_HPBar* m_pHPBar = nullptr;
};

END

#endif // !LogoObject_h__
