#ifndef NaviMeshVtxCtrl_h__
#define NaviMeshVtxCtrl_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CNaviMesh;
class CNaviMeshVtxCtrl : public Engine::CGameObject
{
#define TRIANGLE_NUM	2
#define TRI_VERTEX_NUM	3
#define AXIS_NUM		3

private: // 생성자, 소멸자
	explicit  CNaviMeshVtxCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMeshVtxCtrl(const  CNaviMeshVtxCtrl& rhs);
	virtual ~CNaviMeshVtxCtrl(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CNaviMeshVtxCtrl*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void ProcessPickInput();
	
	void SetNaviMesh(CNaviMesh* _pNaviMesh);
	void SetVertexInfo(CNaviMesh* _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex);
	void SetActive(_bool _bIsActivated) { m_bIsActivated = _bIsActivated; }
	void SetGrouping(_bool _bIsGrouping);
	void SetGroupRange(_float _fGroupRange) { m_fGroupRange = _fGroupRange; }
	void SetPickMode(NAVIMESH_TAB::E_PICKMODE _ePickMode);
	void SetNaviMagnet(_bool _bIsNaviMagnet) { m_bIsNaviMagnet = _bIsNaviMagnet; }
	
	_vec3 GetVertexPos() const;
	_bool IsActive() const { return m_bIsActivated; }
	_bool IsGrouping() const { return m_bIsGrouping; }
	_float GetGroupRange() const { return m_fGroupRange; }
	NAVIMESH_TAB::E_PICKMODE GetPickMode() const { return m_ePickMode; }
	_bool IsNaviMagnet() const { return m_bIsNaviMagnet; }

	void ReleaseVertexInfo();
	
	PLANE::E_TYPE GetPlaneTypeByRay(Engine::PICKINGRAYINFO& _rRayInfo);
	void DragVertex();
	void FormGroup();
	void ReleaseGroup();
	void MoveGroup();

private:
	vector<pair<_int, _int>> m_vecGroupList;
	const _vec3 m_vAxisVertices[AXIS_NUM][2] =
	{
		{ _vec3(0.f, 0.f, 0.f), _vec3(1.f, 0.f, 0.f) },		// X축
		{ _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f) },		// Y축
		{ _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, -1.f) }		// -Z축
	};
	const D3DXCOLOR m_clAxisColor[AXIS_NUM] = {
		D3DXCOLOR(1.f, 0.f, 0.f, 1.f),
		D3DXCOLOR(0.f, 1.f, 0.f, 1.f),
		D3DXCOLOR(0.f, 0.f, 1.f, 1.f)
	};
	const _vec3 XYTriLocal[TRIANGLE_NUM][TRI_VERTEX_NUM] = {
		{ _vec3(0.f, 1.f, 0.f), _vec3(1.f, 1.f, 0.f), _vec3(1.f, 0.f, 0.f) },
		{ _vec3(1.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f) }
	};
	const _vec3 XZTriLocal[TRIANGLE_NUM][TRI_VERTEX_NUM] = {
		{ _vec3(0.f, 0.f, 0.f), _vec3(1.f, 0.f, 0.f), _vec3(1.f, 0.f, -1.f) },
		{ _vec3(1.f, 0.f, -1.f), _vec3(0.f, 0.f, -1.f), _vec3(0.f, 0.f, 0.f) }
		//{ _vec3(0.f, 0.f, 1.f), _vec3(1.f, 0.f, 1.f), _vec3(1.f, 0.f, 0.f) },
		//{ _vec3(1.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, -1.f) }
	};
	const _vec3 YZTriLocal[TRIANGLE_NUM][TRI_VERTEX_NUM] = {
		{ _vec3(0.f, 1.f, -1.f), _vec3(0.f, 1.f, 0.f), _vec3(0.f, 0.f, 0.f) },
		{ _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, -1.f), _vec3(0.f, 1.f, -1.f) }
	};
	PLANE::E_TYPE m_ePlaneType = PLANE::TYPE_END;
	_bool m_bIsActivated = false;
	_bool m_bIsGrouping = false;
	_bool m_bIsPicking = false;
	CNaviMesh* m_pNaviMesh = nullptr;
	_int m_iTriangleIndex = -1;
	_int m_iVertexIndex = -1;
	_float m_fGroupRange = 3.f;
	NAVIMESH_TAB::E_PICKMODE m_ePickMode = NAVIMESH_TAB::PICKMODE_NAVI;
	_bool m_bIsNaviMagnet = false;

	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !TerrainMap_h__