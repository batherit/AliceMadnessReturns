#ifndef CameraController_h__
#define CameraController_h__

#include "GameObject.h"

BEGIN(Engine)
class CCamera;
class ENGINE_DLL CCameraController abstract : public CGameObject
{
protected:
	explicit CCameraController(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraController(void);

public:
	virtual HRESULT			Ready_Object(void) { return S_OK; }
	virtual _int			Update_Object(const _float& fTimeDelta) { return 0; }

	void SetStickLen(_float _fStickLen) { m_fStickLen = _fStickLen; }
	void SetStickDir(const _vec3& _vStickDir) { m_vStickDir = _vStickDir; }
	void SetStickEnd(const _vec3& _vCameraPosW);

	_float GetStickLen() const { return m_fStickLen; }
	_vec3 GetStickDir() const { return m_vStickDir; }
	_vec3 GetStickEnd() const;
	_vec3 GetRightAxis() const;

	// 매니져에서 업데이트마다 호출됩니다.
	virtual void ControlCamera(const _float& _fDeltaTime, CCamera* _pCamera, const _float& _fShiftFactor) = 0; 

protected:
	void TranslateCameraToStickEnd(CCamera * _pCamera, const _float& _fShiftFactor);
	void RotateCameraToTargetObject(CCamera* _pCamera, const CGameObject* _pGameObject, const _float& _fShiftFactor = 1.f);
	void RotateCameraToTargetPoint(CCamera* _pCamera, const _vec3& _vTargetPoint, const _float& _fShiftFactor = 1.f);

protected:
	virtual void Free(void);

protected:
	_float m_fStickLen = 10.f;
	_vec3 m_vStickDir = _vec3(0.f, 0.f, -1.f);
};


END
#endif // CameraController_h__
