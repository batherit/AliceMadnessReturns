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
	m_vTargetPos = m_pTransform->GetPos();

	Engine::CComponent* pComponent = nullptr;
	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransform = AddComponent<Engine::CTransform>();
	
	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(20.f, 20.f);

	// Renderer
	pComponent = m_pRenderer = AddComponent<Engine::CRenderer>();

	// Collider
	pComponent = m_pCollider = AddComponent<Engine::CSphereCollider>();
	m_pCollider->SetSphereColliderInfo(2.f);


		
	return S_OK;
}

int CPlayer::Update_Object(const _float & _fDeltaTime)
{
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
		m_pPhysics->SetDirection(vDir);
		m_pPhysics->MoveByDelta(_fDeltaTime);
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	m_pMesh->Render_Meshes();
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
