#include "pch.h"
#include "CameraController_Target.h"
#include "Camera.h"
#include "AliceW.h"

USING(Client)

CCameraController_Target::CCameraController_Target(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCameraController(pGraphicDev)
{
	_vec3 vDir = _vec3(0.f, 0.8f, -1.f);
	D3DXVec3Normalize(&vDir, &vDir);

	SetStickDir(vDir);
	SetStickLen(6.2f);


}

CCameraController_Target::~CCameraController_Target(void)
{

}

void CCameraController_Target::ControlCamera(const _float& _fDeltaTime, Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// 카메라 컨트롤러를 플레이어 위치에 맞춘다.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	GetTransform()->SetPos(vPlayerPos);

	// 카메라 스틱의 방향을 조정한다.
	CAliceW* pAliceW = dynamic_cast<CAliceW*>(m_pPlayer);
	if (pAliceW && pAliceW->GetTargetObject()) {
		_vec3 vToPlayer = vPlayerPos - pAliceW->GetTargetObject()->GetTransform()->GetPos();
		vToPlayer.y = 0.f;
		_float fLength = D3DXVec3Length(&vToPlayer);
		if (fLength == 0.f)
			return;

		vToPlayer /= fLength;	// Normalize;

		//GetTransform()->SetPos((pAliceW->GetTargetObject()->GetTransform()->GetPos() + vPlayerPos) * 0.5f);

		//_vec3 vStickDirXZ = m_vStickDir;
		//vStickDirXZ.y = 0.f;
		//D3DXVec3Normalize(&vStickDirXZ, &vStickDirXZ);
		//_vec3 vRotAxis = Engine::GetRotationAxis(vStickDirXZ, vToPlayer);
		//_float fRotAngle = Engine::GetRotationAngle(vStickDirXZ, vToPlayer);
		//_matrix matRot;
		//D3DXMatrixRotationAxis(&matRot, &vRotAxis, fRotAngle * 0.2f);
		//D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);

		m_fStickLen = Engine::GetValueByWeight(Engine::Clamp(D3DXVec3Dot(&WORLD_Y_AXIS, &m_vStickDir), 0.f, 1.f), 3.0f, 9.f);
		m_fStickLen *= Engine::GetValueByWeight(Engine::GetWeightByValue(Engine::Clamp(fLength, 1.5f, 10.f), 1.f, 10.f), 0.8f, 1.5f);

		if (Engine::CDirectInputMgr::GetInstance()->IsMouseFixed()) {
			_vec3 vDeltaMouseDegree = Engine::CDirectInputMgr::GetInstance()->GetDeltaMouseDegree();
			_float fRotAngleByY = vDeltaMouseDegree.x * 0.005f;
			_float fRotAngleByRight = vDeltaMouseDegree.y * 0.005f;
			
			_matrix matRot;
			D3DXMatrixRotationAxis(&matRot, &WORLD_Y_AXIS, fRotAngleByY);
			D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);
			D3DXMatrixRotationAxis(&matRot, &GetRightAxis(), fRotAngleByRight);
			D3DXVec3TransformNormal(&m_vStickDir, &m_vStickDir, &matRot);
		}

		/*POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);*/

		// 카메라 위치를 조정한다.
		TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

		RotateCameraToTargetPoint(_pCamera, (pAliceW->GetTargetObject()->GetTransform()->GetPos() + vPlayerPos) * 0.5f + _vec3(0.f, 1.5f, 0.f), _fShiftFactor);
	}
}

CCameraController_Target * CCameraController_Target::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraController_Target*	pInstance = new CCameraController_Target(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCameraController_Target::Free(void)
{
	CCameraController::Free();
}