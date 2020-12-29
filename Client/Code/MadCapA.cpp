#include "pch.h"
#include "MadCapA.h"
#include "Map.h"
#include "StateMgr.h"
#include "MadCapAState_Idle.h"
#include "StaticObject.h"

CMadCapA::CMadCapA(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMadCapA::CMadCapA(const CMadCapA & rhs)
	:
	CGameObject(rhs)
{
}

CMadCapA::~CMadCapA(void)
{
}

HRESULT CMadCapA::Ready_Object(void)
{
	Engine::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"MadCapA"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), pComponent);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"MadCapA.col");

	// MeshRenderer
	pComponent = m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	//m_pTransform->SetScale(_vec3(0.01f, 0.01f, 0.01f));

	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	//m_pPhysics->SetDirection(_vec3(0.f, 0.f, -1.f));
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED, ALICE_RUN_SPEED);
	//m_pPhysics->SetResistanceCoefficientXZ(0.95f);
	m_pPhysics->SetGravity(9.8f * 3.f);

	m_pStateMgr = new CStateMgr<CMadCapA>(*this);
	m_pStateMgr->SetNextState(new CMadCapAState_Idle(*this));


	CStaticObject* pStaticObject = CStaticObject::Create(m_pGraphicDev);
	pStaticObject->SetRenderInfo(L"Fork");
	//pStaticObject->GetTransform()->Rotate(D3DXToRadian(45.f), D3DXToRadian(90.f), D3DXToRadian(160.f));
	pStaticObject->GetTransform()->Translate(0.f, -0.5f, 0.f);
	AddChild(pStaticObject, "Bip01-Prop1");

	return S_OK;
}

int CMadCapA::Update_Object(const _float & _fDeltaTime)
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

	if (!IsActivated())
		return 1;

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return 0;
	}
	m_pRenderer->Update(_fDeltaTime);

	CGameObject::Update_Object(_fDeltaTime);
	m_pStateMgr->Update(_fDeltaTime);

	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
	_vec3 vCurrentPos = GetTransform()->GetPos();
	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	vTargetPos = pNaviMesh->GetSlidedPos(vTargetPos);

	_vec3 vSettedPos = vTargetPos;

	if (IsFalling(_fDeltaTime)) {
		// 추락 중이라면, 적절한 셀을 찾는다.
		_int iCellIndex = pNaviMesh->GetNaviIndexByPos(vCurrentPos, vTargetPos + _vec3(0.f, -1.0f, 0.f));
		if (-1 != iCellIndex && pNaviMesh->GetCell(iCellIndex)->GetTagIndex() != Engine::CCell::TYPE_SLIDE_EXIT) {
			m_bIsLanded = true;
			m_iCellIndex = iCellIndex;
			//pNaviMesh->Set_NaviIndex(iCellIndex);
			GetPhysics()->SetVelocityY(0.f);
		}
	}
	else if (IsLanded()) {
		GetPhysics()->SetVelocityY(0.f);

		vSettedPos = m_pMap->GetNaviMesh()->Move_OnNaviMesh(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f)), GetPhysics());

		if (vSettedPos == vTargetPos + _vec3(0.f, -1.0f, 0.f)) {
			vSettedPos += _vec3(0.f, 1.f, 0.f);
			m_bIsLanded = false;
		}
	}
	// 이동 확정
	GetTransform()->SetPos(vSettedPos);

	m_pMesh->Play_Animation(_fDeltaTime);
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW())) {
		return 0;
	}
	

	return 0;
}

void CMadCapA::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CMadCapA::LoadColliders(const _tchar* _pFileName)
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

void CMadCapA::OnCollision(Engine::CollisionInfo _tCollisionInfo)
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

CMadCapA * CMadCapA::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMadCapA*	pInstance = new CMadCapA(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMadCapA::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

_bool CMadCapA::IsFalling(const _float & _fDeltaTime)
{
	if (GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded)
		return true;
	return false;
}