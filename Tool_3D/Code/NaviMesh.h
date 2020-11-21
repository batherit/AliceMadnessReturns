#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CNaviMesh : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMesh(const  CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3);
	void PopTriangleVertices(_int _iTriangleIndex);

private:
	Engine::CRenderer* m_pRenderer = nullptr;
	Engine::CManualCol* m_pManualCol = nullptr;
};

END

#endif // !TerrainMap_h__