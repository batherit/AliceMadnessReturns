#include "CameraMgr.h"
#include "Export_Utility.h"
#include "CameraController.h"

USING(Engine)

Engine::CCameraMgr::CCameraMgr(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

Engine::CCameraMgr::~CCameraMgr()
{
}

HRESULT CCameraMgr::Ready_Object(void)
{
	return S_OK;
}

_int CCameraMgr::Update_Object(const _float & _fDeltaTime)
{
	if (!m_pCamera)
		return 0;

	if (m_iCameraControllerIndex < 0 || m_iCameraControllerIndex >= static_cast<_int>(m_vecCameraControllers.size()))
		return 0;

	m_vecCameraControllers[m_iCameraControllerIndex]->ControlCamera(m_pCamera, m_fShiftFactor);

	return 0;
}

void CCameraMgr::ChangeCameraController(_int _iCameraControllerIndex, _float _fShiftFactor)
{
	if (_iCameraControllerIndex < 0 || _iCameraControllerIndex >= static_cast<_int>(m_vecCameraControllers.size()))
		return;

	m_fShiftFactor = Clamp(_fShiftFactor, 0.001f, 1.f);
	m_iCameraControllerIndex = _iCameraControllerIndex;
}

CCameraController * CCameraMgr::GetCameraController(_int _iCameraControllerIndex) const
{
	if (_iCameraControllerIndex < 0 || _iCameraControllerIndex >= static_cast<_int>(m_vecCameraControllers.size()))
		return nullptr;

	return m_vecCameraControllers[_iCameraControllerIndex];
}

CCameraMgr * CCameraMgr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraMgr*	pInstance = new CCameraMgr(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCameraMgr::Free(void)
{
	CGameObject::Free();
}