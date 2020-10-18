#include "pch.h"
#include "CLogoObject.h"


CLogoObject::CLogoObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CLogoObject::CLogoObject(const CLogoObject & rhs)
	:
	CGameObject(rhs)
{
}

CLogoObject::~CLogoObject(void)
{
}


HRESULT CLogoObject::Ready_Object(void)
{
	AddComponent<Engine::CMoveComponent>();

	Engine::CMoveComponent* pMoveComponent = nullptr;
	pMoveComponent = GetComponent<Engine::CMoveComponent>();
	pMoveComponent->Scaling(1.f, 0.f, 0.f);
	return S_OK;
}

int CLogoObject::Update_Object(const _float & fTimeDelta)
{
	Engine::CMoveComponent* pMoveComponent = nullptr;
	pMoveComponent = GetComponent<Engine::CMoveComponent>();

	if (!pMoveComponent) return 1;

	_matrix matRot;
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_Q)) {
		pMoveComponent->RotateByRight(5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		pMoveComponent->RotateByRight(-5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		pMoveComponent->RotateByUp(5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		pMoveComponent->RotateByUp(-5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_E)) {
		pMoveComponent->RotateByLook(5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		pMoveComponent->RotateByLook(-5.f * fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_UP)) {
		pMoveComponent->SetToXYZ(pMoveComponent->GetUp());
		pMoveComponent->SetSpeed(10.f);
		pMoveComponent->MoveByDelta(fTimeDelta);
	}
	if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_DOWN)) {
		pMoveComponent->SetToXYZ(pMoveComponent->GetUp());
		pMoveComponent->SetSpeed(-10.f);
		pMoveComponent->MoveByDelta(fTimeDelta);
	}

	return 0;
}

void CLogoObject::Render_Object(void)
{
	Engine::CMoveComponent* pMoveComponent = nullptr;
	pMoveComponent = GetComponent<Engine::CMoveComponent>();

	if (!pMoveComponent) return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &pMoveComponent->GetWorldMatrix());

	Engine::Render_Texture(Engine::RESOURCE_STATIC, L"Texture_Logo", 0);
	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_RcTex");
}

CLogoObject * CLogoObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoObject*	pInstance = new CLogoObject(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CLogoObject::Free(void)
{
	CGameObject::Free();
}
