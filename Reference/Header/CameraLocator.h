#ifndef CameraLocator_h__
#define CameraLocator_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCameraLocator : public CGameObject
{
protected:
	explicit CCameraLocator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraLocator(void);

public:
	virtual HRESULT			Ready_Object(void) { return S_OK; }
	virtual _int			Update_Object(const _float& fTimeDelta) { return 0; }

	void SetStickLen(_float _fStickLen) { m_fStickLen = _fStickLen; }
	void SetStickDir(const _vec3& _vStickDir) { m_vStickDir = _vStickDir; }
	void SetCameraPos(const _vec3& _vCameraPosW);

	_float GetStickLen() const { return m_fStickLen; }
	_vec3 GetStickDir() const { return m_vStickDir; }
	_vec3 GetCameraPos() const;


protected:
	virtual void Free(void);

protected:
	_float m_fStickLen = 10.f;
	_vec3 m_vStickDir = _vec3(0.f, 0.f, -1.f);
};


END
#endif // CameraLocator_h__
