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
	SetStickLen(1.4f);


}

CCameraController_Gun::~CCameraController_Gun(void)
{

}

void CCameraController_Gun::ControlCamera(const _float& _fDeltaTime, Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// ī�޶� ��Ʈ�ѷ��� �÷��̾� ��ġ�� �����.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	_float fRatio = m_pPlayer->GetTransform()->GetScale().x;

	SetStickLen(1.4f * fRatio);

	CAliceW* pAlice = dynamic_cast<CAliceW*>(m_pPlayer);
	_float fOffsetY = 0.f;
	if (pAlice) {
		m_fRadian += D3DXToRadian(360.f * 3.f) * _fDeltaTime;
		if (m_fRadian >= 2.f * D3DX_PI)
			m_fRadian -= 2.f * D3DX_PI;

		if (pAlice->IsMoving(_fDeltaTime)) {
			fOffsetY = Engine::GetValueByWeight(Engine::GetWeightByCos(m_fRadian), -0.02f, 0.02f);
		}
	}

	_vec3 vCameraPos = vPlayerPos + m_pPlayer->GetTransform()->GetRight() * 0.4f * fRatio + m_pPlayer->GetTransform()->GetUp() * (1.5f + fOffsetY) * fRatio;
	GetTransform()->SetPos(vCameraPos);

	// ī�޶� ��ƽ�� ������ �����Ѵ�.
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

	// ī�޶� ��ġ�� �����Ѵ�.
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