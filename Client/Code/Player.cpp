#include "pch.h"
#include "Player.h"
#include "Map.h"

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

	//m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());

	// Clone Mesh
	//pComponent = m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"AliceW"));
	//pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Alice"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), pComponent);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	// MeshRenderer
	pComponent = m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	m_pTransform->SetScale(_vec3(1.f, 1.f, 1.f));
	m_pMesh->Set_AnimationSet(0);
	//m_pMesh->Set_AnimationSet(0);
	
	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(20.f, 20.f);

	// Renderer
	//pComponent = m_pRenderer = AddComponent<Engine::CRenderer>();

	// Collider
	//pComponent = m_pCollider = AddComponent<Engine::CSphereCollider>();
	//m_pCollider->SetSphereColliderInfo(2.f);

	// MeshCollider
	//pComponent = m_pCollider = dynamic_cast<Engine::CMeshCollider*>(Engine::GetOriProto(L"Collider_Stone"));
	//pComponent = m_pCollider = dynamic_cast<Engine::CMeshCollider*>(Engine::GetOriProto(L"Collider_Player"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);
	//Engine::Safe_AddRef(pComponent);
		
	return S_OK;
}

int CPlayer::Update_Object(const _float & _fDeltaTime)
{
	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}

	CGameObject::Update_Object(_fDeltaTime);

	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
	vCamRight.y = 0.f;
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	_vec3 vDir = _vec3(0.f, 0.f, 0.f);

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_LEFT)) {
		vDir -= vCamRight;
	}
	
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_RIGHT)) {
		vDir += vCamRight;
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_UP)) {
		vDir += vCamLook;
	}

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_DOWN)) {
		vDir -= vCamLook;
	}

	m_pPhysics->SetDirection(vDir);
	/*if(vDir.x == 0.f && vDir.y == 0.f && vDir.z == 0.f)
		m_pMesh->Set_AnimationSet(57);
	else
		m_pMesh->Set_AnimationSet(54);*/
	
	m_pTransform->SetPos(m_pMap->GetNaviMesh()->Move_OnNaviMesh(&m_pTransform->GetPos(), &(vDir * _fDeltaTime * m_pPhysics->GetSpeed())));

	//m_pMesh->Set_AnimationSet(54);
	m_pMesh->Play_Animation(_fDeltaTime);

	//m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRenderer->Update(_fDeltaTime);



	return 0;
}

void CPlayer::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	m_pRenderer->Render();
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

//void CPlayer::AttachItemToBone(Engine::CGameObject * _pItemObject, const _tchar * _pBoneName)
//{
//}
//
//void CPlayer::DetachFromBone()
//{
//}

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


// 쿼터니언 회전을 통해 카메라 축과 일치시키기
/*_matrix matView, matViewRot, matRotSlerp;
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
}*/