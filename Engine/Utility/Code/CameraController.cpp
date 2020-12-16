#include "CameraController.h"
#include "Transform.h"
#include "Camera.h"

USING(Engine)

CCameraController::CCameraController(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CCameraController::~CCameraController(void)
{

}

void CCameraController::SetStickEnd(const _vec3 & _vCameraPosW)
{
	_vec3 vToCamera = _vCameraPosW - GetTransform()->GetPos();

	m_fStickLen = D3DXVec3Length(&vToCamera);
	if (m_fStickLen > 0.f) {
		m_vStickDir = vToCamera / m_fStickLen;
	}
}

_vec3 CCameraController::GetStickEnd() const
{
	return GetTransform()->GetPos() + m_fStickLen * m_vStickDir;
}

_vec3 CCameraController::GetRightAxis() const
{
	_vec3 vRight;

	if (D3DXVec3Length(D3DXVec3Cross(&vRight, &WORLD_Y_AXIS, &m_vStickDir)) < 0.00001f) {
		return WORLD_X_AXIS;
	}

	return vRight;
}

void CCameraController::TranslateCameraToStickEnd(CCamera * _pCamera, const _float& _fShiftFactor)
{
	_vec3 vStickEnd = GetStickEnd();
	_vec3 vCameraPos = _pCamera->GetTransform()->GetPos();
	_vec3 vToStickEnd = vStickEnd - vCameraPos;
	vToStickEnd *= _fShiftFactor;
	_pCamera->GetTransform()->SetPos(vCameraPos + vToStickEnd);
}

void CCameraController::RotateCameraToTargetObject(CCamera* _pCamera, const CGameObject * _pGameObject, const _float& _fShiftFactor)
{
	if (!_pGameObject)
		return;

	RotateCameraToTargetPoint(_pCamera, _pGameObject->GetTransform()->GetPos(), _fShiftFactor);
}

void CCameraController::RotateCameraToTargetPoint(CCamera* _pCamera, const _vec3 & _vTargetPoint, const _float& _fShiftFactor)
{
	_matrix matRotCamera, matRotToTarget, matRotSlerp;

	matRotCamera = _pCamera->GetViewMatrix();
	D3DXMatrixInverse(&matRotCamera, NULL, &matRotCamera);

	// ī�޶� ���� ��ķκ��� ȸ�� ����� ������.
	Engine::ExtractRotationMatrix(&matRotCamera, &matRotCamera);

	// Ÿ�������� ȸ�� ����� ����.
	GetRotateMatrixEyeAtUp(&matRotToTarget, &_pCamera->GetTransform()->GetPos(), &_vTargetPoint, &WORLD_Y_AXIS);
	
	// Ÿ������ ���� ������ ���� ������ ���ŵ� ����� ����.
	Engine::GetRotationMatrixSlerp(&matRotSlerp, &matRotCamera, &matRotToTarget, _fShiftFactor);

	// ���ο� ȸ�� ����� ī�޶� ������.
	_pCamera->GetTransform()->ResetRightUpLook(
		&(_vec3(matRotSlerp._31, matRotSlerp._32, matRotSlerp._33) + _pCamera->GetTransform()->GetPos()),
		&WORLD_Y_AXIS
	);
}

void Engine::CCameraController::Free(void)
{
	CGameObject::Free();
}

