#ifndef Map_h__
#define Map_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CTerrain;
class CStaticObject;
class CMap : public Engine::CGameObject
{
private: // ������, �Ҹ���
	explicit  CMap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CMap(const  CMap& rhs);
	virtual ~CMap(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CMap*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);

public:
	void LoadMap(Engine::CLayer* pLayer, const _tchar* _pTerrainFilePath, const _tchar* _pNaviFilePath, const _tchar* _pObjectsFilePath);
	void LoadTerrain(const _tchar* _pFilePath);
	void LoadNaviMesh(const _tchar* _pFilePath);
	void LoadStaticObjects(Engine::CLayer* pLayer, const _tchar* pFilePath);

	Engine::CNaviMesh* GetNaviMesh() const { return m_pNaviMesh; }

private:
	Engine::CRenderer* m_pRenderer = nullptr;
	Engine::CNaviMesh* m_pNaviMesh = nullptr;
	CTerrain* m_pTerrain = nullptr;
	vector<CStaticObject*> m_vecStaticObjects;
};

END

#endif // !MapMap_h__
