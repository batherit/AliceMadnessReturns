#include "pch.h"
#include "Alice.h"
#include "Map.h"

CAlice::CAlice(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CAlice::CAlice(const CAlice & rhs)
	:
	CGameObject(rhs)
{
}

CAlice::~CAlice(void)
{
}

HRESULT CAlice::Ready_Object(void)
{
	Engine::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"AliceW"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), pComponent);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"AliceW.col");

	// MeshRenderer
	pComponent = m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	m_pTransform->SetScale(_vec3(0.01f, 0.01f, 0.01f));
	
	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	//m_pPhysics->SetDirection(_vec3(0.f, 0.f, -1.f));
	m_pPhysics->SetSpeed(4.5f, 4.5f);
		
	return S_OK;
}

int CAlice::Update_Object(const _float & _fDeltaTime)
{
	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}

	if(1 == CGameObject::Update_Object(_fDeltaTime))
		return 1;

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

	if (D3DXVec3LengthSq(&vDir) > 0.f) {
		// 플레이어 회전시키기
		_vec3 vRotAxis = Engine::GetRotationAxis(GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(GetTransform()->GetLook(), vDir) * 0.25f;
		GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
		//_vec3 vRotatedDir = Engine::GetRotatedVector(vRotAxis, vRotAngle, m_pPhysics->GetDirection());
		m_pMesh->Set_AnimationSet(ALICE::AliceW_Run);
	}
	else {
		m_pMesh->Set_AnimationSet(ALICE::AliceW_Idle);
	}

	

	m_pPhysics->SetDirection(vDir);
	m_pTransform->SetPos(m_pMap->GetNaviMesh()->Move_OnNaviMesh(&m_pTransform->GetPos(), &(vDir * _fDeltaTime * m_pPhysics->GetSpeed())));
	m_pMesh->Play_Animation(_fDeltaTime);


	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return 0;
	}
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CAlice::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CAlice::LoadColliders(const _tchar* _pFileName)
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, _pFileName);

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	LoadCollidersInfo(hFile);

	CloseHandle(hFile);

	return true;
}

void CAlice::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		_int a = 10;
	}
}

//void CAlice::AttachItemToBone(Engine::CGameObject * _pItemObject, const _tchar * _pBoneName)
//{
//}
//
//void CAlice::DetachFromBone()
//{
//}

CAlice * CAlice::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAlice*	pInstance = new CAlice(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CAlice::Free(void)
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