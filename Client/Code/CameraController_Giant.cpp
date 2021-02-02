#include "pch.h"
#include "CameraController_Giant.h"
#include "Camera.h"
#include "AliceW.h"

USING(Client)

CCameraController_Giant::CCameraController_Giant(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 0.f, -1.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(1.4f);


}

CCameraController_Giant::~CCameraController_Giant(void)
{

}

void CCameraController_Giant::ControlCamera(const _float& _fDeltaTime, Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pTargetObject || !m_pBenchmarkObject)
		return;

	// 카메라 컨트롤러를 기준점 오브젝트 위치에 맞춘다.
	//GetTransform()->SetPos(m_pTargetObject->GetTransform()->GetPos() + _vec3(0.f, 2.f, 0.f));

	// 카메라 스틱의 길이/방향을 조정한다.
	//_vec3 vCameraPos = m_pBenchmarkObject->GetTransform()->GetPos() 
	//	+ m_pBenchmarkObject->GetTransform()->GetRight() * 0.4f + m_pBenchmarkObject->GetTransform()->GetUp() * (1.5f + 3.f);
	GetTransform()->SetPos(m_pBenchmarkObject->GetTransform()->GetPos() + _vec3(0.f, 0.15f, 0.f));
	m_vStickDir = -m_pBenchmarkObject->GetTransform()->GetLook();
	_matrix matRot;
	D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, D3DXMatrixRotationAxis(&matRot, &WORLD_Y_AXIS, -D3DX_PI * 0.15f));
	m_fStickLen = D3DXVec3Length(&(m_pTargetObject->GetTransform()->GetPos() - m_pBenchmarkObject->GetTransform()->GetPos())) * 0.4f;
	/*if (m_fStickLen < 6.f) {
		m_fStickLen = 6.f;
	}*/

	// 카메라 위치를 조정한다.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, m_pTargetObject->GetTransform()->GetPos(), _fShiftFactor);
}

CCameraController_Giant * CCameraController_Giant::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Giant*	pInstance = new CCameraController_Giant(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Giant::Free(void)
{
	CCameraController::Free();
}