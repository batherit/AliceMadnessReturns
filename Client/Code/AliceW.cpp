#include "pch.h"
#include "AliceW.h"
#include "Map.h"
#include "StateMgr.h"
#include "AliceWState_Idle.h"

CAliceW::CAliceW(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CAliceW::CAliceW(const CAliceW & rhs)
	:
	CGameObject(rhs)
{
}

CAliceW::~CAliceW(void)
{
}

HRESULT CAliceW::Ready_Object(void)
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
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED, ALICE_RUN_SPEED);
	m_pPhysics->SetResistanceCoefficientXZ(0.95f);
	m_pPhysics->SetGravity(9.8f * 3.f);

	m_pStateMgr = new CStateMgr<CAliceW>(*this);
	m_pStateMgr->SetNextState(new CAliceWState_Idle(*this));
		
	return S_OK;
}

int CAliceW::Update_Object(const _float & _fDeltaTime)
{
	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}

	if (!m_pStateMgr->ConfirmValidState())
		return 1;

	// TEST : 테스트용이므로 삭제 요망
	if (!IsDead() && Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		SetDead(true);
	}

	if(1 == CGameObject::Update_Object(_fDeltaTime))
		return 1;
	m_pStateMgr->Update(_fDeltaTime);

	_vec3 vCurrentPos = GetTransform()->GetPos();
	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);
	_vec3 vSettedPos = vTargetPos;
	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
	if (IsFalling(_fDeltaTime)) {
		// 추락 중이라면, 적절한 셀을 찾는다.
		_int iCellIndex = pNaviMesh->GetNaviIndexByPos(vCurrentPos, vTargetPos);
		if (-1 != iCellIndex) {
			m_bIsLanded = true;
			pNaviMesh->Set_NaviIndex(iCellIndex);
			GetPhysics()->SetVelocityY(0.f);
		}
	}
	else if(IsLanded()){
		if (!pNaviMesh->GetCurCell()->IsCollided(vCurrentPos, vTargetPos))
			m_bIsLanded = false;
		else {
			// 점프 중 상태도 아닌 착지 상태라면,
			GetPhysics()->SetVelocityY(0.f);
			vSettedPos = m_pMap->GetNaviMesh()->Move_OnNaviMesh(&GetTransform()->GetPos(), &(vTargetPos+_vec3(0.f, -1.0f, 0.f)));
		}
	}
	
	GetTransform()->SetPos(vSettedPos);

	m_pMesh->Play_Animation(_fDeltaTime);

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return 0;
	}
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CAliceW::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CAliceW::LoadColliders(const _tchar* _pFileName)
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

void CAliceW::OnCollision(Engine::CollisionInfo _tCollisionInfo)
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

//void CAliceW::AttachItemToBone(Engine::CGameObject * _pItemObject, const _tchar * _pBoneName)
//{
//}
//
//void CAliceW::DetachFromBone()
//{
//}

CAliceW * CAliceW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAliceW*	pInstance = new CAliceW(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CAliceW::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

_bool CAliceW::IsAttackOn(const _float & _fDeltaTime)
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB))
		return true;
	return false;
}

_bool CAliceW::IsJumpOn(const _float & _fDeltaTime)
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_SPACE))
		return true;
	return false;
}

_bool CAliceW::IsFloatingOn(const _float & _fDeltaTime)
{
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_SPACE))
		return true;
	return false;
}

_bool CAliceW::IsFalling(const _float & _fDeltaTime)
{
	if (GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded)
		return true;
	return false;
}

_bool CAliceW::IsRunOn(const _float &, _vec3 * _pDir)
{
	_matrix matView;
	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
	vCamRight.y = 0.f;
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	_vec3 vDir = _vec3(0.f, 0.f, 0.f);

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
		vDir -= vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
		vDir += vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
		vDir += vCamLook;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
		vDir -= vCamLook;
	}

	_float fLengthSq = D3DXVec3LengthSq(&vDir);
	if (fLengthSq > 0.f) {
		// 플레이어 방향 전환이 이루어졌다면 회전시키기
		_vec3 vRotAxis = Engine::GetRotationAxis(GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(GetTransform()->GetLook(), vDir) * 0.25f;
		GetTransform()->RotateByAxis(vRotAngle, vRotAxis);

		if (_pDir)
			*_pDir = vDir;
		return true;
	}

	return false;
}

//_bool CAliceW::ProcessMoveXZ(const _float& _fDeltaTime)
//{
//	_matrix matView;
//	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, nullptr, &matView);
//
//	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
//	vCamRight.y = 0.f;
//	D3DXVec3Normalize(&vCamRight, &vCamRight);
//	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
//	vCamLook.y = 0.f;
//	D3DXVec3Normalize(&vCamLook, &vCamLook);
//	_vec3 vDir = _vec3(0.f, 0.f, 0.f);
//
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
//		vDir -= vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
//		vDir += vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
//		vDir += vCamLook;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
//		vDir -= vCamLook;
//	}
//
//	_float fLengthSq = D3DXVec3LengthSq(&vDir);
//	if (fLengthSq > 0.f) {
//		// 플레이어 방향 전환이 이루어졌다면 회전시키기
//		_vec3 vRotAxis = Engine::GetRotationAxis(GetTransform()->GetLook(), vDir);
//		_float vRotAngle = Engine::GetRotationAngle(GetTransform()->GetLook(), vDir) * 0.25f;
//		GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
//		return true;
//	}
//
//	return false;
//}


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