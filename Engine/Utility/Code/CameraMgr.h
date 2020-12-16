#pragma once

#include "Engine_Define.h"
#include "Base.h"

#include "Camera.h"

BEGIN(Engine)
class CCameraController;
class ENGINE_DLL CCameraMgr final : public CGameObject
{
private:
	explicit CCameraMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraMgr();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& _fDeltaTime) override;

public:
	void SetCamera(CCamera* _pCamera) { m_pCamera = _pCamera; }	// 씬 실행시 카메라를 초기 한 번 세팅함.
	CCamera* GetCamera() const { return m_pCamera; }
	void ChangeCameraController(_int _iCameraControllerIndex, _float _fShiftFactor = 1.f);
	CCameraController* GetCameraController(_int _iCameraControllerIndex) const;
	void AddCameraController(CCameraController* _pCameraController) { m_vecCameraControllers.emplace_back(_pCameraController); }

public:
	static CCameraMgr* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free(void);

private:
	CCamera* m_pCamera = nullptr;
	vector<CCameraController*> m_vecCameraControllers;
	_int m_iCameraControllerIndex = -1;
	_float m_fShiftFactor = 1.f;
};

END