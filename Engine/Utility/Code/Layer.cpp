#include "Layer.h"
#include "ColliderObject.h"
#include "ColliderObject_Sphere.h"
#include "ColliderObject_AABB.h"
#include "ColliderObject_OBB.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer()
{

}

//CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
//{
//	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
//
//	if (iter == m_mapObject.end())
//		return nullptr;
//
//	return iter->second->Get_Component(pComponentTag, eID);
//}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	//m_mapObject.emplace(pObjTag, pGameObject);

	m_mapObjectList[pObjTag].emplace_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * _pGameObject)
{
	if (nullptr == _pGameObject)
		return E_FAIL;

	m_mapObjectList[OBJECTLIST_TCHAR].emplace_back(_pGameObject);

	return S_OK;
}

CLayer::LayerList& CLayer::GetLayerList(const _tchar * _pObjTag)
{
	return m_mapObjectList[_pObjTag];
}

CLayer::LayerList& CLayer::GetLayerList()
{
	return m_mapObjectList[OBJECTLIST_TCHAR];
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

Engine::_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	// 무효화 객체 수거(렌더 이후에 최종적으로 nullptr이 될 수 있기 때문에, 앞쪽에 배치함.)
	CollectInvalidObjects();

	_int iExit = 0;

	for (auto& iter : m_mapObjectList)
	{
		for (auto& rObj : iter.second) {
			iExit = rObj->Update_Object(fTimeDelta);

			if (iExit & 0x80000000)
			{
				MSG_BOX("GameObject Problem");
				return iExit;
			}
		}
	}

	// 충돌 체크를 진행한다.
	CollisionInfo tCollisionInfo;
	for (auto& iter1 : m_mapObjectList) {
		for (auto& rObj1 : iter1.second) {
			for (auto& iter2 : m_mapObjectList) {
				for (auto& rObj2 : iter2.second) {
					if (rObj1 == rObj2)
						continue;

					if (IsCollided(rObj1->GetCullingSphere(), rObj2->GetCullingSphere())) {
						for (auto& rPair1 : rObj1->GetColliderList()) {
							for (auto& rCollider1 : rPair1.second) {
								for (auto& rPair2 : rObj2->GetColliderList()) {
									for (auto& rCollider2 : rPair2.second) {
										if (IsCollided(rCollider1, rCollider2)) {
											tCollisionInfo.pCollidedObject = rObj2;
											tCollisionInfo.pCollidedCollider = rCollider2;
											rObj1->OnCollision(tCollisionInfo);
											tCollisionInfo.pCollidedObject = rObj1;
											tCollisionInfo.pCollidedCollider = rCollider1;
											rObj2->OnCollision(tCollisionInfo);
										}
									}
								}
							}
							
						}
					}
				}
			}
		}
	}

	return iExit;
}

_bool CLayer::IsCollided(CColliderObject * _pCollider1, CColliderObject * _pCollider2)
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

_bool CLayer::IsCollided(CColliderObject_Sphere * _pSphere1, CColliderObject_Sphere * _pSphere2)
{
	if (!_pSphere1 || !_pSphere2)
		return false;

	_vec3 vPos1, vPos2;
	_float fRadius1, fRadius2;

	_pSphere1->GetCollisionInfo(vPos1, fRadius1);
	_pSphere2->GetCollisionInfo(vPos2, fRadius2);

	return IsSpheresCollided(vPos1, fRadius1, vPos2, fRadius2);
}

_bool CLayer::IsCollided(CColliderObject_Sphere * _pSphere, CColliderObject_AABB * _pAABB)
{
	return false;
}

_bool CLayer::IsCollided(CColliderObject_Sphere * _pSphere, CColliderObject_OBB * _pOBB)
{
	return false;
}


_bool CLayer::IsCollided(CColliderObject_AABB * _pAABB1, CColliderObject_AABB * _pAABB2)
{
	if (!_pAABB1 || !_pAABB2)
		return false;
		
	_vec3 vMin1, vMax1, vMin2, vMax2;
	_pAABB1->GetCollisionInfo(vMin1, vMax1);
	_pAABB2->GetCollisionInfo(vMin2, vMax2);

	return IsAABBCollided(vMin1, vMax1, vMin2, vMax2);
}

_bool CLayer::IsCollided(CColliderObject_AABB * _pAABB, CColliderObject_Sphere * _pSphere)
{
	return false;
}

_bool CLayer::IsCollided(CColliderObject_AABB * _pAABB, CColliderObject_OBB * _pOBB)
{
	if (!_pAABB || !_pOBB)
		return false;

	// OBB형식의 충돌체크를 진행한다.
	return Engine::IsOBBCollided(_pAABB->GetBoxInfo(), _pOBB->GetBoxInfo());
}

_bool CLayer::IsCollided(CColliderObject_OBB * _pOBB1, CColliderObject_OBB * _pOBB2)
{
	if (!_pOBB1 || !_pOBB2)
		return false;
	
	// OBB 충돌 체크를 진행한다.
	return Engine::IsOBBCollided(_pOBB1->GetBoxInfo(), _pOBB2->GetBoxInfo());
}

_bool CLayer::IsCollided(CColliderObject_OBB * _pOBB, CColliderObject_Sphere * _pSphere)
{
	return false;
}

_bool CLayer::IsCollided(CColliderObject_OBB * _pOBB, CColliderObject_AABB * _pAABB)
{
	return IsCollided(_pAABB, _pOBB);
}

void CLayer::CollectInvalidObjects()
{
	for (auto& iter : m_mapObjectList)
	{
		for (auto& rObj : iter.second) {
			// 무효화 객체 해제
			if (!rObj->IsValid()) {
				Safe_Release(rObj);
			}
		}

		iter.second.remove_if([](auto& rObj)
		{
			return rObj == nullptr;
		});
	}
}

Engine::CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& iter : m_mapObjectList) {
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
	}
	m_mapObjectList.clear();
}

