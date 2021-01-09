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
	SetStickLen(4.5f);

	
}

CCameraController_Player::~CCameraController_Player(void)
{

}

void CCameraController_Player::ControlCamera(const _float& _fDeltaTime, Engine::CCamera * _pCamera, const _float& _fShiftFactor)
{
	if (!m_pPlayer)
		return;

	// ī�޶� ��Ʈ�ѷ��� �÷��̾� ��ġ�� �����.
	_vec3 vPlayerPos = m_pPlayer->GetTransform()->GetPos();
	_float fRatio = 1.f;// m_pPlayer->GetTransform()->GetScale().x;
	GetTransform()->SetPos(vPlayerPos + _vec3(0.f, 1.f * fRatio, 0.f));
	SetStickLen(4.5f * fRatio);

	// ī�޶� ��ƽ�� ������ �����Ѵ�.
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
	

	// ���콺 �Ƚ�
	//POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };
	//ClientToScreen(g_hWnd, &ptMouse);
	//SetCursorPos(ptMouse.x, ptMouse.y);

	// ī�޶� ��ġ�� �����Ѵ�.
	TranslateCameraToStickEnd(_pCamera, _fShiftFactor);

	RotateCameraToTargetPoint(_pCamera, vPlayerPos + _vec3(0.f, 1.f * fRatio, 0.f), _fShiftFactor);
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