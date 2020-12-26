#ifndef AliceW_h__
#define AliceW_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
template<typename T> class CStateMgr;
class CAliceW : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CAliceW(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CAliceW(const  CAliceW& rhs);
	virtual ~CAliceW(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CAliceW*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CAliceW>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

	void SetDead(const _bool& _bIsDead) { m_bIsDead = _bIsDead; }
	void SetLanded(const _bool& _bIsLanded) { m_bIsLanded = _bIsLanded; }

	_bool IsAttackOn(const _float& _fDeltaTime);
	_bool IsJumpOn(const _float& _fDeltaTime);
	_bool IsSliding(const _float& _fDeltaTime);
	_bool IsFloatingOn(const _float& _fDeltaTime);
	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	//_bool ProcessMoveXZ(const _float& _fDeltaTime);
	//_bool ProcessMoveY(const _float& _fDeltaTime);
	_bool IsDead() const { return m_bIsDead; }
	_bool IsLanded() const { return m_bIsLanded; }
	CMap* GetMap() const { return m_pMap; }

private:
	CMap* m_pMap = nullptr;

	CStateMgr<CAliceW>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;

	_bool m_bIsDead = false;
	_bool m_bIsLanded = false;
};

END

#endif // !LogoObject_h__
