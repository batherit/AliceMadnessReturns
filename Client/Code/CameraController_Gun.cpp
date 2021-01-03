#include "pch.h"
#include "CameraController_Gun.h"
#include "Camera.h"
#include "AliceW.h"

USING(Client)

CCameraController_Gun::CCameraController_Gun(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 0.f, -1.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(1.3f);


}

CCameraController_Gun::~CCameraController_Gun(void)
{

}

void CCameraController_Gun::ControlCamera(Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// 카메라 컨트롤러를 플레이어 위치에 맞춘다.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	_vec3 vCameraPos = vPlayerPos + m_pPlayer->GetTransform()->GetRight() * 0.3f + m_pPlayer->GetTransform()->GetUp() * 1.4f;
	GetTransform()->SetPos(vCameraPos);

	// 카메라 스틱의 방향을 조정한다.
	_vec3 vDeltaMouseDegree = Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree();
	_float fRotAngleByY = vDeltaMouseDegree.x * 0.005f;
	_float fRotAngleByRight = vDeltaMouseDegree.y * 0.005f;

	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &WORLD_Y_AXIS, fRotAngleByY);
	D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);
	D3DXMatrixRotationAxis(&matRot, &GetRightAxis(), fRotAngleByRight);
	D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);

	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

	m_pPlayer->GetTransform()->ResetRightUpLook(&(vPlayerPos - m_vStickDir), &WORLD_Y_AXIS);

	// 카메라 위치를 조정한다.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, _pCamera->GetTransform()->GetPos() - m_vStickDir, _fShiftFactor);
}

CCameraController_Gun * CCameraController_Gun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Gun*	pInstance = new CCameraController_Gun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Gun::Free(void)
{
	CCameraController::Free();
}