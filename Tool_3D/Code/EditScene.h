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
class CGizmo;
//class CNaviMeshInputProcessor;
class CEditScene : public Engine::CScene
{
private: // ������, �Ҹ���
	explicit CEditScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEditScene(const CEditScene& rhs);
	virtual ~CEditScene(void);

public:
	// CScene��(��) ���� ��ӵ�
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

	Engine::CInputProcessor* GetInputProcessor() const;

	Engine::CGameObject* GetPickedObject() const;
	Engine::CInputProcessorMgr* GetInputProcessorMgr() const { return m_pInputProcessorMgr; }

	
	_bool AddStaticObject(const _tchar* _pMeshTag);
	CStaticObject* GetStaticObject(_int _iObjectIndex);
	vector<CStaticObject*>& GetStaticObjectList() { return m_vecStaticObjects; }
	_bool DeleteStaticObject(_int _iObjectIndex);

	// �ͷ����� ���� ���� ���̺�/�ε�
	void SaveTerrain();
	void LoadTerrain();
	// �׺�޽��� ���� ���� ���̺�/�ε�
	void SaveNaviMesh();
	void LoadNaviMesh();
	// ���� ���� ���� ���̺�/�ε�
	void SaveMap();
	void LoadMap();

private:
	_bool IsValidObjectIndex(_int _iObjectIndex);

private:
	Engine::CInputProcessorMgr* m_pInputProcessorMgr = nullptr;
	vector<CStaticObject*> m_vecStaticObjects;
};

END
#endif // MainApp_h__