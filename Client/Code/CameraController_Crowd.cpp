#include "pch.h"
#include "CameraController_Crowd.h"
#include "Camera.h"

USING(Client)

CCameraController_Crowd::CCameraController_Crowd(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 1.f, 0.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(1.f);
}

CCameraController_Crowd::~CCameraController_Crowd(void)
{

}

void CCameraController_Crowd::ControlCamera(Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pTarget)
		return;

	// 카메라 컨트롤러를 플레이어 위치에 맞춘다.
	_vec3 vTargetPos = m_pTarget->GetTransform()->GetPos();
	//GetTransform()->SetPos(vPlayerPos + _vec3(0.f, 1.f, 0.f));

	// 카메라 위치를 조정한다.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, vTargetPos + _vec3(0.f, 1.f, 0.f), _fShiftFactor);
}

CCameraController_Crowd * CCameraController_Crowd::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Crowd*	pInstance = new CCameraController_Crowd(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Crowd::Free(void)
{
	CCameraController::Free();
}