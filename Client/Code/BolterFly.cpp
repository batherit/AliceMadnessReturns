#include "pch.h"
#include "BolterFly.h"
#include "Map.h"
#include "StateMgr.h"
#include "BolterFlyState_Idle.h"
#include "StaticObject.h"
#include "Attribute.h"
#include "EFT_Blood.h"

CBolterFly::CBolterFly(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBolterFly::CBolterFly(const CBolterFly & rhs)
	:
	CGameObject(rhs)
{
}

CBolterFly::~CBolterFly(void)
{
}

HRESULT CBolterFly::Ready_Object(void)
{
	Engine::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"BolterFly"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), pComponent);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"BolterFly.col");

	// MeshRenderer
	pComponent = m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(BOLTERFLY_MAX_HP, BOLTERFLY_MAX_HP);

	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(BOLTERFLY_RUN_SPEED);
	//m_pPhysics->SetGravity(9.8f * 2.f);

	m_pStateMgr = new CStateMgr<CBolterFly>(*this);
	m_pStateMgr->SetNextState(new CBolterFlyState_Idle(*this));

	m_pAttackCollider = GetColliderFromTag(L"EnemyAttack");
	m_pAttackCollider->SetDamage(BOLTERFLY_DAMAGE);
	m_pAttackCollider->SetActivated(false);

	//GetWeapon()->GetColliderFromTag(L"EnemyAttack")->SetActivated(false);

	return S_OK;
}

int CBolterFly::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}
	if (!m_pStateMgr->ConfirmValidState())
		return 1;



	m_pRenderer->Update(_fDeltaTime);

	CGameObject::Update_Object(_fDeltaTime);
	m_pStateMgr->Update(_fDeltaTime);

	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
	_vec3 vCurrentPos = GetTransform()->GetPos();
	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	vTargetPos = pNaviMesh->GetSlidedPos(vTargetPos);

	_vec3 vSettedPos = vTargetPos;

	//if (IsFalling(_fDeltaTime)) {
	//	// 추락 중이라면, 적절한 셀을 찾는다.
	//	_int iCellIndex = pNaviMesh->GetNaviIndexByPos(vCurrentPos, vTargetPos + _vec3(0.f, -1.0f, 0.f));
	//	if (-1 != iCellIndex && pNaviMesh->GetCell(iCellIndex)->GetTagIndex() != Engine::CCell::TYPE_SLIDE_EXIT) {
	//		m_bIsLanded = true;
	//		m_iCellIndex = iCellIndex;
	//		//pNaviMesh->Set_NaviIndex(iCellIndex);
	//		GetPhysics()->SetVelocityY(0.f);
	//	}
	//}
	//else if (IsLanded()) {
	//	vSettedPos = m_pMap->GetNaviMesh()->Move_OnNaviMesh(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f)), GetPhysics());

	//	if (vSettedPos == vTargetPos + _vec3(0.f, -1.0f, 0.f)) {
	//		vSettedPos += _vec3(0.f, 1.f, 0.f);
	//		m_bIsLanded = false;
	//	}
	//	else {
	//		GetPhysics()->SetVelocityY(0.f);
	//	}
	//}
	// 이동 확정
	GetTransform()->SetPos(vSettedPos);

	m_pMesh->Play_Animation(_fDeltaTime);


	return 0;
}

void CBolterFly::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CBolterFly::LoadColliders(const _tchar* _pFileName)
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

void CBolterFly::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead())
		return;

	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0 &&
		lstrcmp(_tCollisionInfo.pCollidedMyCollider->GetColliderTag(), L"Monster") == 0) {
		if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
			CEFT_Blood* pEffect = nullptr;
			//	_int iBloodNum = Engine::GetNumberBetweenMinMax(10, 10);
			_float fBloodSize = 1.f;
			for (_int i = 0; i < 2; ++i) {
				fBloodSize = Engine::GetNumberBetweenMinMax(0.1f, 0.45f);
				pEffect = CEFT_Blood::Create(m_pGraphicDev);
				pEffect->SetBloodInfo(
					(_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos() + _tCollisionInfo.pCollidedMyCollider->GetTransform()->GetPos()) * 0.5f
					+ _vec3(Engine::GetNumberBetweenMinMax(-0.3f, 0.3f), Engine::GetNumberBetweenMinMax(-0.3f, 0.3f), Engine::GetNumberBetweenMinMax(-0.2f, 0.2f)),
					_vec3(fBloodSize, fBloodSize, 1.f));
				Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
			}
		}
	}
}

void CBolterFly::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead())
		return;

	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0 &&
		lstrcmp(_tCollisionInfo.pCollidedMyCollider->GetColliderTag(), L"Monster") == 0) {
		// 충돌하지 않았다면 어태커에서 제거한다.
		m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
	}
}

CBolterFly * CBolterFly::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBolterFly*	pInstance = new CBolterFly(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CBolterFly::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

//_bool CBolterFly::IsFalling(const _float & _fDeltaTime)
//{
//	if (GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded)
//		return true;
//	return false;
//}

_bool CBolterFly::IsDead() const
{
	return m_pAttribute->IsDead();
}
