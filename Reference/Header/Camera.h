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
	void Shake(_float _fShakeKeepTime, _float _fSakeRadius, _int _iShakeNum);

protected:
	_matrix			m_matView, m_matProj;
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFovY, m_fAspect, m_fNear, m_fFar;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void);

private:
	_bool m_bIsShaking = false;
	_float m_fShakeKeepTime = 0.f;
	_float m_fShakeTickTime = 0.f;
	_float m_fShakeElapsedTime = 0.f;
	_float m_fShakeRadius = 0.f;
	_vec3 m_vShakeOffset = _vec3(0.f, 0.f, 0.f);
	_int m_iShakeNum = 0;
};


END
#endif // Camera_h__
