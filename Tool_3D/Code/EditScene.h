#ifndef EditScene_h__
#define EditScene_h__

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
class CEditScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CEditScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEditScene(const CEditScene& rhs);
	virtual ~CEditScene(void);

public:
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEditScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

public:
	CTerrain* GetTerrain() const;
	CNaviMesh* GetNaviMesh() const;
	CNaviMeshVtxMover* GetNaviMeshVtxMover() const;
	CGizmo* GetGizmo() const;

	//Engine::CInputProcessor* GetInputProcessor() const;
	Engine::CGameObject* GetPickedObject() const;
	//Engine::CInputProcessorMgr* GetInputProcessorMgr() const { return m_pInputProcessorMgr; }

	
	_bool AddStaticObject(const _tchar* _pMeshTag);
	_bool AddDynamicObject(const _tchar* _pMeshTag, const _vec3& _vPos = _vec3(0.f, 0.f, 0.f));
	_bool AddCustomObject(const _tchar* _pMeshTag);
	CStaticObject* GetStaticObject(_int _iObjectIndex);
	CStaticObject* GetStaticObject(const _tchar* _pMeshTag);
	CDynamicObject* GetDynamicObject(const _tchar* _pMeshTag);
	Engine::CGameObject* GetCustomObject(_int _iObjectIndex);
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
	_bool IsValidStaticObjectIndex(_int _iObjectIndex);
	_bool IsValidCustomObjectIndex(_int _iObjectIndex);
	Engine::MESHTYPE GetCustomObjectMeshType(const _tchar* _pMeshTag);

private:
	//Engine::CInputProcessorMgr* m_pInputProcessorMgr = nullptr;
	vector<CStaticObject*> m_vecStaticObjects;
	vector<CDynamicObject*> m_vecDynamicObjects;
	vector<Engine::CGameObject*> m_vecCustomObjects;
	map<_tchar*, Engine::MESHTYPE> m_mapCustomObjectType;
};

END
#endif // MainApp_h__