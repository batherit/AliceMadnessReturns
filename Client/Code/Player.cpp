#include "pch.h"
#include "Player.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	:
	CGameObject(rhs)
{
}

CPlayer::~CPlayer(void)
{
}

HRESULT CPlayer::Ready_Object(void)
{
	m_pTransform->SetMaxSpeed(20.f);
	m_pTransform->SetSpeed(20.f);
	m_vTargetPos = m_pTransform->GetPos();

	m_pRenderer = AddComponent<Engine::CRenderer>();

	/*Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_UP", VK_UP);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_DOWN", VK_DOWN);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_LEFT", VK_LEFT);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_RIGHT", VK_RIGHT);*/
		
	return S_OK;
}

int CPlayer::Update_Object(const _float & _fDeltaTime)
{
	/*_vec3 vDir{ 0.f, 0.f, 0.f };
	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_UP")) {
		vDir += m_pTransform->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_DOWN")) {
		vDir -= m_pTransform->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_LEFT")) {
		vDir -= m_pTransform->GetRight();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_RIGHT")) {
		vDir += m_pTransform->GetRight();
	}*/

	// 쿼터니언 회전을 통해 카메라 축과 일치시키기
	_matrix matView, matViewRot, matRotSlerp;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	Engine::ExtractRotationMatrix(&matViewRot, &matView);
	
	_matrix matCurRot;
	Engine::ExtractRotationMatrix(&matCurRot, &m_pTransform->GetObjectMatrix());
	Engine::GetRotationMatrixSlerp(&matRotSlerp, &matCurRot, &matViewRot, 0.1f);
	m_pTransform->ResetRightUpLook(
		&(_vec3(matRotSlerp._31, matRotSlerp._32, matRotSlerp._33) + m_pTransform->GetPos()),
		&WORLD_Y_AXIS
	);

	_vec3 vDir = m_vTargetPos - m_pTransform->GetPos();
	if (D3DXVec3LengthSq(&vDir) > 0.5f) {
		D3DXVec3Normalize(&vDir, &vDir);
		m_pTransform->SetDir(vDir);
		m_pTransform->MoveByDelta(_fDeltaTime);
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}
