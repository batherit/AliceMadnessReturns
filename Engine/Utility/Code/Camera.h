#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	virtual HRESULT			Ready_Object(void);
	virtual _int			Update_Object(const _float& fTimeDelta);

	void SetProjectionMatrix(const _float& _fFOV, const _float& _fAspect, const _float& _fNearZ, const _float& _fFarZ);
	inline _matrix GetViewMatrix() const { return m_matView; }
	inline _matrix GetProjMatrix() const { return m_matProj; }

protected:
	_matrix			m_matView, m_matProj;
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFovY, m_fAspect, m_fNear, m_fFar;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void);
};


END
#endif // Camera_h__
