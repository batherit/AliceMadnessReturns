#ifndef Boss_h__
#define Boss_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CAttribute;
template<typename T> class CStateMgr;
class CBoss : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CBoss(const  CBoss& rhs);
	virtual ~CBoss(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CBoss*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CBoss>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	CAttribute* GetAttribute() const { return m_pAttribute; }

	Engine::CGameObject* GetTargetObject() const { return m_pTargetObject; }
	void SetTargetObject(Engine::CGameObject* _pTargetObject) { m_pTargetObject = _pTargetObject; }
	void SetLanded(const _bool& _bIsLanded) { m_bIsLanded = _bIsLanded; }

	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsDead() const;
	_bool IsLanded() const { return m_bIsLanded; }
	CMap* GetMap() const { return m_pMap; }
	_int& GetCellIndex() { return m_iCellIndex; }

	//Engine::CColliderObject* GetAttackL() const { return m_pAttackL; }
	//Engine::CColliderObject* GetAttackR() const { return m_pAttackR; }
	Engine::CColliderObject* GetAttackCollider() const { return m_pAttackCollider; }

private:
	CMap* m_pMap = nullptr;

	Engine::CColliderObject* m_pAttackCollider = nullptr;
	Engine::CGameObject* m_pTargetObject = nullptr;

	CStateMgr<CBoss>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CAttribute* m_pAttribute = nullptr;

	_int m_iCellIndex = -1;
	_bool m_bIsLanded = false;
};

END

#endif // !LogoObject_h__