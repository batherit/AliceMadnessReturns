#include "pch.h"
#include "Monster.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CMonster & rhs)
	:
	CGameObject(rhs)
{
}

CMonster::~CMonster(void)
{
}

HRESULT CMonster::Ready_Object(void)
{
	AddComponent<Engine::CTransform>();
	m_pTransform = GetComponent<Engine::CTransform>();
	m_pTransform->SetScaleXYZ(5.f, 5.f, 5.f);

	AddComponent<Engine::CRenderer>();
	m_pRenderer = GetComponent<Engine::CRenderer>();

	

	return S_OK;
}

int CMonster::Update_Object(const _float & _fDeltaTime)
{
	Engine::CTransform* pCameraTransform = m_pMainCamera->GetComponent<Engine::CTransform>();
	_vec3 vInvLook = -m_pTransform->GetLook();
	_vec3 vToCamera = pCameraTransform->GetPos() - m_pTransform->GetPos();
	vToCamera.y = 0;
	D3DXVec3Normalize(&vToCamera, &vToCamera);
	
	_float fRotAngle = Engine::GetRotationAngle(vInvLook, vToCamera);
	_vec3 vRotAxis = Engine::GetRotationAxis(vInvLook, vToCamera);
	m_pTransform->RotateByAxis(fRotAngle, vRotAxis);

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CMonster::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
}

CMonster * CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster*	pInstance = new CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free(void)
{
	CGameObject::Free();
}
