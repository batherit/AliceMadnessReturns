#include "pch.h"
#include "StaticCamera.h"


CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CCamera(pGraphicDev)
{
}

CStaticCamera::CStaticCamera(const CStaticCamera & rhs)
	:
	CCamera(rhs)
{
}

CStaticCamera::~CStaticCamera(void)
{
}


HRESULT CStaticCamera::Ready_Object(void)
{
	AddComponent<Engine::CMoveComponent>();
	m_pMoveComponent = GetComponent<Engine::CMoveComponent>();
	m_pMoveComponent->SetXYZ(0.f, 30.f, -30.f);
	m_pMoveComponent->RotateByRight(D3DXToRadian(45.f));

	SetProjectionMatrix(D3DXToRadian(45.f), (_float)WINCX / WINCY, 0.1f, 1000.f);

	return S_OK;
}

int CStaticCamera::Update_Object(const _float & fTimeDelta)
{
	// �� �����̽� ��ȯ ��� ���� �Լ�(��, ī�޶� ���� ����� �� ����� ������ִ� �Լ�)
	m_vEye = m_pMoveComponent->GetPos();		// ī�޶� ��ġ
	m_vAt = m_vEye + m_pMoveComponent->GetLook();	// ī�޶� ��ġ���� �ٷ� �Ʒ��� ����.
	m_vUp = m_pMoveComponent->GetUp();						// ī�޶� Up��

	return Engine::CCamera::Update_Object(fTimeDelta);
}

CStaticCamera * CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaticCamera*	pInstance = new CStaticCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CStaticCamera::Free(void)
{
	CCamera::Free();
}
