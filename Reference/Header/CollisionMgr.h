#ifndef CollisionMgr_h__
#define CollisionMgr_h__

//#include "Engine_Define.h"
//#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
//class CGameObject;
class ENGINE_DLL CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

public:
	_bool AddGameObject(CGameObject* _pGameObject);
	void ProcessCollision();
	void ClearGameObjectList();

public:
	_bool IsCollided(CColliderObject* _pCollider1, CColliderObject* _pCollider2);

	_bool IsCollided(CColliderObject_Sphere* _pSphere1, CColliderObject_Sphere* _pSphere2);
	_bool IsCollided(CColliderObject_Sphere* _pSphere, CColliderObject_AABB* _pAABB);
	_bool IsCollided(CColliderObject_Sphere* _pSphere, CColliderObject_OBB* _pOBB);

	_bool IsCollided(CColliderObject_AABB* _pAABB1, CColliderObject_AABB* _pAABB2);
	_bool IsCollided(CColliderObject_AABB* _pAABB, CColliderObject_Sphere* _pSphere);
	_bool IsCollided(CColliderObject_AABB* _pAABB, CColliderObject_OBB* _pOBB);

	_bool IsCollided(CColliderObject_OBB* _pOBB1, CColliderObject_OBB* _pOBB2);
	_bool IsCollided(CColliderObject_OBB* _pOBB, CColliderObject_Sphere* _pSphere);
	_bool IsCollided(CColliderObject_OBB* _pOBB, CColliderObject_AABB* _pAABB);

	void SetColliderVisible(const _bool& _bIsColliderVisible);
	_bool IsColliderVisible() const { return m_bIsColliderVisible; }

	//void SetActivated(const _bool& _bIsActivated) { m_bIsActivated; }

private:
	_bool m_bIsColliderVisible = true;
	//_bool m_bIsActivated = true;
	list<CGameObject*>				m_ObjectList;

private:
	virtual void Free(void);
};

END
#endif // CollisionMgr_h__
