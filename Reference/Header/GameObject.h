#ifndef GameObject_h__
#define	GameObject_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)
class CComponent;
class CTransform;
class CColliderObject;
class CColliderObject_Sphere;
class CColliderObject_AABB;
class CColliderObject_OBB;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);	// 일반 생성자
	explicit CGameObject(const CGameObject& rhs);			// 복사 생성자
	virtual ~CGameObject();

public:
	//CComponent* Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CTransform* GetTransform() const { return m_pTransform; }

public:
	virtual HRESULT Ready_Object(void) = 0;
	virtual _int Update_Object(const _float& _fDeltaTime);
	virtual void Render_Object(void) {};
	virtual void OnCollision(CollisionInfo _tCollisionInfo) {};

public:
	LPDIRECT3DDEVICE9 GetGraphicDev() const { return m_pGraphicDev; }

	void SetValid(_bool _bIsValid) { m_bIsValid = _bIsValid; }
	_bool IsValid() const { return m_bIsValid; }
	void SetActivated(_bool _bIsActivated) { m_bIsActivated = _bIsActivated; }
	_bool IsActivated() const { return m_bIsActivated; }

	CComponent* GetComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	CComponent*	AddComponent(const COMPONENTID eComponentID, const _tchar* pComponentTag);
	template <class T>
	T* GetComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		return static_cast<T*>(GetComponent(id, tag));
	}
	template <class T>
	T* AddComponent() {
		const _tchar* tag = T::GetComponentTag();
		const COMPONENTID id = T::GetComponentID();
		return static_cast<T*>(AddComponent(id, tag));
	}
	
	const _matrix* GetBoneMatrixPointer(const char* _pBoneName);
	void SetParent(CGameObject* _pParent, const char* _pBoneName = nullptr);
	_bool AddChild(CGameObject* _pChild, const char* _pBoneName = nullptr);
	_bool AddCollider(CColliderObject* _pCollider, const char* _pBoneName = nullptr);
	_bool IsChildExist(CGameObject* _pChild);
	_bool IsColliderExist(CColliderObject* _pCollider);
	void ReleaseParent();
	void ReleaseChild(CGameObject* _pChild);
	void ClearColliders();
	//void ReleaseCollider(CColliderObject* _pCollider);

	CGameObject* GetParent() const { return m_pParent; }
	vector<CGameObject*>& GetChildList() { return m_vecChildList; }
	//vector<CColliderObject*>& GetColliderList() { return m_vecColliderList; }
	vector<pair<string, vector<CColliderObject*>>>& GetColliderList() { return m_vecColliders; }
	CColliderObject* GetColliderFromTag(const _tchar* _pColliderTag);
	CColliderObject_Sphere* GetCullingSphere() const { return m_pCullingSphere; }

	// 파일 입출력 관련
	virtual _bool SaveInfo(FILE* _fpOut)	{ return true; }
	virtual _bool LoadInfo(FILE* _fpIn)		{ return true; }
	virtual _bool SaveInfo(HANDLE& _hfOut)	{ return true; }
	virtual _bool LoadInfo(HANDLE& _hfIn)	{ return true; }
	_bool SaveCollidersInfo(HANDLE& _hfOut);
	_bool LoadCollidersInfo(HANDLE& _hfIn);

	virtual void Free(void);

protected:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	_bool m_bIsValid = true;
	_bool m_bIsActivated = true;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CGameObject* m_pParent = nullptr;
	vector<CGameObject*> m_vecChildList;
	vector<pair<string, vector<CColliderObject*>>> m_vecColliders;
	CColliderObject_Sphere* m_pCullingSphere = nullptr;		// 충돌체로부터 추출함.
	//vector<CColliderObject*> m_vecColliderList;
	map<const _tchar*, CComponent*>			m_mapComponent[ID_END];
	CTransform* m_pTransform = nullptr;
};

END

#endif


/*
template<typename T>
void AddComponent() {
	Engine::COMPONENTID eComponentID = T::GetComponentID();
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(T::GetComponentTag()));
	if (iter != m_mapComponent[eComponentID].end()) return;
	m_mapComponent[eComponentID].emplace(T::GetComponentTag(), T::Create());
}

template<typename T>
T* GetComponent() {
	Engine::COMPONENTID eComponentID = T::GetComponentID();
	auto iter = find_if(m_mapComponent[eComponentID].begin(), m_mapComponent[eComponentID].end(), CTag_Finder(T::GetComponentTag()));
	if (iter == m_mapComponent[eComponentID].end()) return nullptr;
	return static_cast<T*>(m_mapComponent[eComponentID][T::GetComponentTag()]);
}
*/