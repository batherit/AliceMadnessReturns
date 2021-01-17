#include "pch.h"
#include "CameraController_Sliding.h"
#include "Camera.h"

USING(Client)

CCameraController_Sliding::CCameraController_Sliding(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 0.8f, -1.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(5.5f);


}

CCameraController_Sliding::~CCameraController_Sliding(void)
{

}

void CCameraController_Sliding::ControlCamera(const _float& _fDeltaTime, Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// ī�޶� ��Ʈ�ѷ��� �÷��̾� ��ġ�� �����.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	_float fRatio = m_pPlayer->GetTransform()->GetScale().x;
	GetTransform()->SetPos(vPlayerPos + _vec3(0.f, 1.f * fRatio, 0.f));

	// ī�޶� ��ƽ�� ������ �����Ѵ�.
	_vec3 vRevLook = -m_pPlayer->GetTransform()->GetLook();		// Look�� XZ��鿡 �����ϴٴ� ���� ������.
	_vec3 vStickDirXZ = m_vStickDir;
	vStickDirXZ.y = 0.f;
	D3DXVec3Normalize(&vStickDirXZ, &vStickDirXZ);
	_vec3 vRotAxis = Engine::GetRotationAxis(vStickDirXZ, vRevLook);
	_float fRotAngle = Engine::GetRotationAngle(vStickDirXZ, vRevLook);
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vRotAxis, fRotAngle * 0.5f);
	D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);

	// ī�޶� ��ġ�� �����Ѵ�.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, vPlayerPos + _vec3(0.f, 1.f * fRatio, 0.f), _fShiftFactor);
}

CCameraController_Sliding * CCameraController_Sliding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Sliding*	pInstance = new CCameraController_Sliding(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Sliding::Free(void)
{
	CCameraController::Free();
}