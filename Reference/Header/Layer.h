#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
	typedef list<CGameObject*>	LayerList;
#define OBJECTLIST_TCHAR	L"ObjectList"

private:
	explicit CLayer(void);
	virtual ~CLayer();

public:
	HRESULT		Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	HRESULT		Add_GameObject(CGameObject* _pGameObject);

	LayerList&	GetLayerList(const _tchar* _pObjTag);
	LayerList&	GetLayerList();

	HRESULT		Ready_Layer(void);
	_int		Update_Layer(const _float& fTimeDelta);

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

private:
	void CollectInvalidObjects();

private:
	map<const _tchar*, LayerList>		m_mapObjectList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);
};

END
#endif // Layer_h__
