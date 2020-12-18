#include "pch.h"
#include "CameraController_Player.h"
#include "Camera.h"

USING(Client)

CCameraController_Player::CCameraController_Player(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 1.f, -1.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(3.5f);

	
}

CCameraController_Player::~CCameraController_Player(void)
{

}

void CCameraController_Player::ControlCamera(Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// 카메라 컨트롤러를 플레이어 위치에 맞춘다.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	GetTransform()->SetPos(vPlayerPos + _vec3(0.f, 1.f, 0.f));

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

	// 카메라 위치를 조정한다.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, vPlayerPos + _vec3(0.f, 1.f, 0.f), _fShiftFactor);
}

CCameraController_Player * CCameraController_Player::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Player*	pInstance = new CCameraController_Player(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Player::Free(void)
{
	CCameraController::Free();
}