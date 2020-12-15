#ifndef DefaultScene_h__
#define DefaultScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//END
BEGIN(Client)
class CTerrain;
class CNaviMesh;
class CNaviMeshVtxMover;
class CStaticObject;
class CDynamicObject;
class CGizmo;
class CStaticObject;
class CDynamicObject;
//class CNaviMeshInputProcessor;
class CColliderScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CColliderScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColliderScene(const CColliderScene& rhs);
	virtual ~CColliderScene(void);

public:
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CColliderScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

public:
	CTerrain* GetTerrain() const;
	CNaviMesh* GetNaviMesh() const;
	CNaviMeshVtxMover* GetNaviMeshVtxMover() const;
	CGizmo* GetGizmo() const;

	Engine::CGameObject* GetPickedObject() const;

	_bool AddStaticObject(const _tchar* _pMeshTag);
	_bool AddDynamicObject(const _tchar* _pMeshTag, const _vec3& _vPos = _vec3(0.f, 0.f, 0.f));
	CStaticObject* GetStaticObject(_int _iObjectIndex);
	CStaticObject* GetStaticObject(const _tchar* _pMeshTag);
	CDynamicObject* GetDynamicObject(const _tchar* _pMeshTag);
	Engine::CGameObject* GetObjectFromTag(const _tchar* _pMeshTag);

	vector<CStaticObject*>& GetStaticObjectList() { return m_vecStaticObjects; }
	vector<CDynamicObject*>& GetDynamicObjectList() { return m_vecDynamicObjects; }
	_bool DeleteStaticObject(_int _iObjectIndex);

	// 터레인탭 편집 내용 세이브/로드
	void SaveTerrain();
	void LoadTerrain();
	// 네비메쉬탭 편집 내용 세이브/로드
	void SaveNaviMesh();
	void LoadNaviMesh();
	// 맵탭 편집 내용 세이브/로드
	void SaveMap();
	void LoadMap();
	// 콜라이더탭 편집 내용 세이브/로드
	void SaveColliders(Engine::CGameObject* _pObject);
	void LoadColliders(Engine::CGameObject* _pObject);

private:
	_bool IsValidObjectIndex(_int _iObjectIndex);

private:
	vector<CStaticObject*> m_vecStaticObjects;
	vector<CDynamicObject*> m_vecDynamicObjects;
};

END
#endif // MainApp_h__