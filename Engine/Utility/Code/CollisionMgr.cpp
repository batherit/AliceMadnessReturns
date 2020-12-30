#include "CollisionMgr.h"
#include "ColliderObject.h"
#include "ColliderObject_Sphere.h"
#include "ColliderObject_AABB.h"
#include "ColliderObject_OBB.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollisionMgr)

Engine::CCollisionMgr::CCollisionMgr(void)
{

}

Engine::CCollisionMgr::~CCollisionMgr(void)
{
	Free();
}

void Engine::CCollisionMgr::Free(void)
{
	ClearGameObjectList();
}

_bool CCollisionMgr::AddGameObject(CGameObject * _pGameObject)
{
	if (!_pGameObject)
		return false;		// 오브젝트가 nullptr이라면 넣지 않아요.

	auto iter = find(m_ObjectList.begin(), m_ObjectList.end(), _pGameObject);
	if (iter != m_ObjectList.end())	// 이미 존재한다면 넣지 않아요.
		return false;

	if (_pGameObject->GetOptimizedColliderList().empty())
		return false;		// 갖고있는 콜라이더가 없다면 넣지 않아요.

	m_ObjectList.emplace_back(_pGameObject);
	return true;
}

void CCollisionMgr::ProcessCollision()
{
	// 충돌 체크를 진행한다.
	CollisionInfo tCollisionInfo;

	for (auto& rObj1 : m_ObjectList) {
		for (auto& rObj2 : m_ObjectList) {
			try {
				if (rObj1 == rObj2 || !rObj1->IsValid() || !rObj2->IsValid())
					continue;		// 같은 오브젝트끼리는 충돌 검출을 하지 않아요.

				if (IsCollided(rObj1->GetCullingSphere(), rObj2->GetCullingSphere())) {
					for (auto& rCollider1 : rObj1->GetOptimizedColliderList()) {
						for (auto& rCollider2 : rObj2->GetOptimizedColliderList()) {
							if (rCollider1 == rObj1->GetCullingSphere() || rCollider2 == rObj2->GetCullingSphere())
								continue;
							
							if (IsCollided(rCollider1, rCollider2)) {
								tCollisionInfo.pCollidedObject = rObj2;
								tCollisionInfo.pCollidedCollider = rCollider2;
								rObj1->OnCollision(tCollisionInfo);
								tCollisionInfo.pCollidedObject = rObj1;
								tCollisionInfo.pCollidedCollider = rCollider1;
								rObj2->OnCollision(tCollisionInfo);
							}
							if (!rObj1->IsValid() || !rObj2->IsValid())
								throw false;
						}
					}
				}
			}
			catch (_bool) {
				continue;
			}
		}
	}

	// 충돌 처리 이후 유효하지 않게된 객체는 제거한다.
	m_ObjectList.remove_if([](CGameObject* _pObj) {
		return !_pObj->IsValid();
	});
}

void CCollisionMgr::ClearGameObjectList()
{
	m_ObjectList.clear();
}

_bool CCollisionMgr::IsCollided(CColliderObject * _pCollider1, CColliderObject * _pCollider2)
{
	if (!_pCollider1 || !_pCollider2)
		return false;

	switch (_pCollider1->GetColliderType()) {
	case TYPE_SPHERE:
		switch (_pCollider2->GetColliderType()) {
		case TYPE_SPHERE:
			return IsCollided(static_cast<CColliderObject_Sphere*>(_pCollider1),
				static_cast<CColliderObject_Sphere*>(_pCollider2));
		case TYPE_AABB:
			return false;
		case TYPE_OBB:
			return false;
		}
		break;
	case TYPE_AABB:
		switch (_pCollider2->GetColliderType()) {
		case TYPE_SPHERE:
			return false;
		case TYPE_AABB:
			return IsCollided(static_cast<CColliderObject_AABB*>(_pCollider1),
				static_cast<CColliderObject_AABB*>(_pCollider2));
		case TYPE_OBB:
			return IsCollided(static_cast<CColliderObject_AABB*>(_pCollider1),
				static_cast<CColliderObject_OBB*>(_pCollider2));
		}
		break;
	case TYPE_OBB:
		switch (_pCollider2->GetColliderType()) {
		case TYPE_SPHERE:
			return false;
		case TYPE_AABB:
			return IsCollided(static_cast<CColliderObject_OBB*>(_pCollider1),
				static_cast<CColliderObject_AABB*>(_pCollider2));
		case TYPE_OBB:
			return IsCollided(static_cast<CColliderObject_OBB*>(_pCollider1),
				static_cast<CColliderObject_OBB*>(_pCollider2));
		}
	default:
		return false;
	}

	return false;
}

_bool CCollisionMgr::IsCollided(CColliderObject_Sphere * _pSphere1, CColliderObject_Sphere * _pSphere2)
{
	if (!_pSphere1 || !_pSphere2)
		return false;

	_vec3 vPos1, vPos2;
	_float fRadius1, fRadius2;

	_pSphere1->GetCollisionInfo(vPos1, fRadius1);
	_pSphere2->GetCollisionInfo(vPos2, fRadius2);

	return IsSpheresCollided(vPos1, fRadius1, vPos2, fRadius2);
}

_bool CCollisionMgr::IsCollided(CColliderObject_Sphere * _pSphere, CColliderObject_AABB * _pAABB)
{
	return false;
}

_bool CCollisionMgr::IsCollided(CColliderObject_Sphere * _pSphere, CColliderObject_OBB * _pOBB)
{
	return false;
}


_bool CCollisionMgr::IsCollided(CColliderObject_AABB * _pAABB1, CColliderObject_AABB * _pAABB2)
{
	if (!_pAABB1 || !_pAABB2)
		return false;

	_vec3 vMin1, vMax1, vMin2, vMax2;
	_pAABB1->GetCollisionInfo(vMin1, vMax1);
	_pAABB2->GetCollisionInfo(vMin2, vMax2);

	return IsAABBCollided(vMin1, vMax1, vMin2, vMax2);
}

_bool CCollisionMgr::IsCollided(CColliderObject_AABB * _pAABB, CColliderObject_Sphere * _pSphere)
{
	return false;
}

_bool CCollisionMgr::IsCollided(CColliderObject_AABB * _pAABB, CColliderObject_OBB * _pOBB)
{
	if (!_pAABB || !_pOBB)
		return false;

	// OBB형식의 충돌체크를 진행한다.
	return Engine::IsOBBCollided(_pAABB->GetBoxInfo(), _pOBB->GetBoxInfo());
}

_bool CCollisionMgr::IsCollided(CColliderObject_OBB * _pOBB1, CColliderObject_OBB * _pOBB2)
{
	if (!_pOBB1 || !_pOBB2)
		return false;

	// OBB 충돌 체크를 진행한다.
	return Engine::IsOBBCollided(_pOBB1->GetBoxInfo(), _pOBB2->GetBoxInfo());
}

_bool CCollisionMgr::IsCollided(CColliderObject_OBB * _pOBB, CColliderObject_Sphere * _pSphere)
{
	return false;
}

_bool CCollisionMgr::IsCollided(CColliderObject_OBB * _pOBB, CColliderObject_AABB * _pAABB)
{
	return IsCollided(_pAABB, _pOBB);
}