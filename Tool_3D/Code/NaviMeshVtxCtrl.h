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

private: // ������, �Ҹ���
	explicit  CNaviMeshVtxCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CNaviMeshVtxCtrl(const  CNaviMeshVtxCtrl& rhs);
	virtual ~CNaviMeshVtxCtrl(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CNaviMeshVtxCtrl*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetActive(_bool _bIsActivated) { m_bIsActivated = _bIsActivated; }
	void SetVertexInfo(CNaviMesh* _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex);
	PLANE::E_TYPE GetPlaneTypeByRay(Engine::PICKINGRAYINFO& _rRayInfo);
	void DragVertex();

	_bool IsActive() const { return m_bIsActivated; }

private:
	const _vec3 m_vAxisVertices[AXIS_NUM][2] =
	{
		{ _vec3(0.f, 0.f, 0.f), _vec3(1.f, 0.f, 0.f) },		// X��
		{ _vec3(0.f, 0.f, 0.f), _vec3(0.f, 1.f, 0.f) },		// Y��
		{ _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, -1.f) }		// -Z��
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
	_bool m_bIsPicking = false;
	CNaviMesh* m_pNaviMesh = nullptr;
	_int m_iTriangleIndex = -1;
	_int m_iVertexIndex = -1;

	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !TerrainMap_h__