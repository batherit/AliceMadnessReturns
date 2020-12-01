#ifndef Gizmo_h__
#define Gizmo_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CGizmo : public Engine::CGameObject
{
#define TRIANGLE_NUM	2
#define TRI_VERTEX_NUM	3
#define AXIS_NUM		3

private: // 생성자, 소멸자
	explicit  CGizmo(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CGizmo(const  CGizmo& rhs);
	virtual ~CGizmo(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CGizmo*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void	Free(void);

public:
	void SetObject(Engine::CGameObject* _pGameObject);

	void ActivateGizmo(const _bool& _bIsGizmoActived) { m_bIsGizmoActivated = _bIsGizmoActived; }

	_bool IsGizmoActivated() const { return m_bIsGizmoActivated; }
	PLANE::E_TYPE GetPlaneTypeByRay(Engine::PICKINGRAYINFO& _rRayInfo);
	void DragObject();


private:
	Engine::CGameObject* m_pGameObject = nullptr;

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

	_bool m_bIsGizmoActivated = false;
	_bool m_bIsPicking = false;
	PLANE::E_TYPE m_ePlaneType = PLANE::TYPE_END;

	// 이동자를 렌더링 하기 위한 렌더러
	Engine::CRenderer* m_pRenderer = nullptr;
};

END

#endif // !TerrainMap_h__