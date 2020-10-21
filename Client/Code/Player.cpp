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
	AddComponent<Engine::CMoveComponent>();
	m_pMoveComponent = GetComponent<Engine::CMoveComponent>();
	m_pMoveComponent->SetMaxSpeed(20.f);
	m_pMoveComponent->SetSpeed(20.f);
	m_vTargetPos = m_pMoveComponent->GetPos();

	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_UP", VK_UP);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_DOWN", VK_DOWN);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_LEFT", VK_LEFT);
	Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_RIGHT", VK_RIGHT);
		
	return S_OK;
}

int CPlayer::Update_Object(const _float & _fDeltaTime)
{
	/*_vec3 vDir{ 0.f, 0.f, 0.f };
	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_UP")) {
		vDir += m_pMoveComponent->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_DOWN")) {
		vDir -= m_pMoveComponent->GetLook();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_LEFT")) {
		vDir -= m_pMoveComponent->GetRight();
	}

	if (Engine::CKeyMgr::GetInstance()->IsKeyPressing(L"KEY_RIGHT")) {
		vDir += m_pMoveComponent->GetRight();
	}*/

	_vec3 vDir = m_vTargetPos - m_pMoveComponent->GetPos();
	if (D3DXVec3LengthSq(&vDir) > 3.f) {
		D3DXVec3Normalize(&vDir, &vDir);
		m_pMoveComponent->SetToXYZ(vDir);
		m_pMoveComponent->MoveByDelta(_fDeltaTime);
	}

	

	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pMoveComponent->GetObjectMatrix());

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
