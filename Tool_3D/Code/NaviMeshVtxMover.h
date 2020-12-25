#ifndef NaviMeshVtxMover_h__
#define NaviMeshVtxMover_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CNaviMesh;
class CNaviMeshVtxMover : public Engine::CGameObject
{
#define TRIANGLE_NUM	2
#define TRI_VERTEX_NUM	3
#define AXIS_NUM		3

private: // 생성자, 소멸자
	explicit  CNaviMeshVtxMover(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMeshVtxMover(const  CNaviMeshVtxMover& rhs);
	virtual ~CNaviMeshVtxMover(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CNaviMeshVtxMover*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	// 정점을 조작하려면 유효한 정점메쉬와 인덱스값을 전달하여야 합니다.
	void SelectVertex(CNaviMesh* _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex);
	void SetVertexPos(const _vec3& _vPos);
	void ActivateMoverGizmo(const _bool& _bIsMoverGizmoActived) { m_bIsMoverGizmoActivated = _bIsMoverGizmoActived; }
	
	void SetGroupRange(_float _fGroupRange) { m_fGroupRange = _fGroupRange; }
	_vec3 GetVertexPos() const;
	_float GetGroupRange() const { return m_fGroupRange; }
	_int GetTriangleIndex() const { return m_iTriangleIndex; }
	_int GetVertexIndex() const { return m_iVertexIndex; }
	_bool IsMoverGizmoActivated() const { return m_bIsMoverGizmoActivated; }

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
	
	_bool m_bIsMoverGizmoActivated = false;
	_bool m_bIsPicking = false;
	PLANE::E_TYPE m_ePlaneType = PLANE::TYPE_END;
	CNaviMesh* m_pNaviMesh = nullptr;
	_int m_iTriangleIndex = -1;
	_int m_iVertexIndex = -1;
	_float m_fGroupRange = 1.f;

	// 이동자를 렌더링 하기 위한 렌더러
	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !TerrainMap_h__