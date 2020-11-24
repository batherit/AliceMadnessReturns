#ifndef Frustum_h__
#define Frustum_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFrustum : public CBase
{
private:
	explicit CFrustum(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFrustum(const CFrustum& rhs);
	virtual ~CFrustum(void);

public:
	LPDIRECT3DDEVICE9		Get_GraphicDev(void) { return m_pGraphicDev; }

public:
	HRESULT				Ready_Frustum(void);
	_bool				Is_InFrustumForObject(const _vec3* pWorldPos, const _float& fRadius);
	_bool				Is_InFrustum(const _vec3* pPos);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_vec3					m_vPoint[8];
	D3DXPLANE				m_Plane[6];

public:
	static CFrustum*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void			Free(void);

};

END

#endif // Frustum_h__
