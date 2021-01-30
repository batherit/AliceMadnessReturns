#include "pch.h"
#include "Boss.h"
#include "Map.h"
#include "StateMgr.h"
#include "BossState_Idle.h"
#include "StaticObject.h"
#include "Attribute.h"
#include "EFT_HobbyHorseAttack.h"
#include "EFT_SlashAttack.h"
#include "EFT_BulletAttack.h"
#include "EFT_Blood.h"
#include "UI_HPBar.h"

CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBoss::CBoss(const CBoss & rhs)
	:
	CGameObject(rhs)
{
}

CBoss::~CBoss(void)
{
}

HRESULT CBoss::Ready_Object(void)
{
	Engine::CComponent* pComponent = nullptr;

	// Mesh
	pComponent = m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Boss"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), pComponent);
	//m_pMesh->Set_AnimationSet(48);

	GetTransform()->SetScaleXYZ(1.3f, 1.3f, 1.3f);

	// Load Colliders
	LoadColliders(L"Boss.col");
	/*m_pAttackL = GetColliderFromTag(L"EnemyAttack_L");
	m_pAttackL->SetActivated(false);
	m_pAttackR = GetColliderFromTag(L"EnemyAttack_R");
	m_pAttackR->SetActivated(false);*/

	// MeshRenderer
	pComponent = m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(BOSS_MAX_HP, BOSS_MAX_HP);

	// Physics
	pComponent = m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(BOSS_RUN_SPEED);
	m_pPhysics->SetGravity(9.8f * 2.f);

	m_pStateMgr = new CStateMgr<CBoss>(*this);
	m_pStateMgr->SetNextState(new CBossState_Idle(*this));

	m_pAttackerObject = CStaticObject::Create(m_pGraphicDev);
	m_pAttackerObject->SetRenderInfo(L"Sickle");
	//pStaticObject->GetTransform()->Rotate(D3DXToRadian(45.f), D3DXToRadian(90.f), D3DXToRadian(160.f));
	m_pAttackerObject->GetTransform()->Translate(0.f, -0.5f, 0.f);
	AddChild(m_pAttackerObject, "Bip01-Prop1");

	m_pAttackCollider = m_pAttackerObject->GetColliderFromTag(L"EnemyAttack");
	m_pAttackCollider->SetDamage(BOSS_DAMAGE);
	m_pAttackCollider->SetActivated(false);

	return S_OK;
}

int CBoss::Update_Object(const _float & _fDeltaTime)
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
	//vTargetPos = pNaviMesh->GetSlidedPos(vTargetPos);

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
		vSettedPos = m_pMap->GetNaviMesh()->MoveOnNaviMesh_Adhesion(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f))/*, GetPhysics()*/);

		if (vSettedPos == vTargetPos + _vec3(0.f, -1.0f, 0.f)) {
			vSettedPos += _vec3(0.f, 1.f, 0.f);
			m_bIsLanded = false;
		}
		else {
			GetPhysics()->SetVelocityY(0.f);
		}
	}
	// 이동 확정
	GetTransform()->SetPos(vSettedPos);

	m_pMesh->Play_Animation(_fDeltaTime);

	return 0;
}

void CBoss::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	_matrix matWorld = GetTransform()->GetObjectMatrix();
	//matWorld._42 -= 1.f;
	m_pRenderer->SetWorldMatrix(matWorld);

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	// 컬링을 안하는 이유는 컬링을 하면 애니메이션 갱신이 안돼서 화면밖에 객체가 있을 경우 Progress를 갱신할 수 없게 되어
	// 상태 진행에 에러가 존재한다.
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	//m_pCollider->Render_MeshCollider(Engine::COL_TRUE, &m_pTransform->GetObjectMatrix());
}

_bool CBoss::LoadColliders(const _tchar* _pFileName)
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

void CBoss::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead())
		return;

	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0) {
			if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
				// 어태커에 등록이 성공했다는 것은 기존 어태커가 등록되지 않았음을 의미하므로 데미지가 들어간다
				//_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(/*VORPALBLADE_DAMAGE*/0);

				CEFT_Blood* pEffect = nullptr;
				//	_int iBloodNum = Engine::GetNumberBetweenMinMax(10, 10);
				_float fBloodSize = 1.f;
				for (_int i = 0; i < 4; ++i) {
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

		//if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0) {
		//	if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
		//		// 어태커에 등록이 성공했다는 것은 기존 어태커가 등록되지 않았음을 의미하므로 데미지가 들어간다
		//		m_pAttribute->Damaged(/*_tCollisionInfo.pCollidedCollider->GetDamage()*/ 0);
		//		//CEFT_HobbyHorseAttack* pEffect = CEFT_HobbyHorseAttack::Create(m_pGraphicDev);
		//		CEFT_SlashAttack* pEffect = CEFT_SlashAttack::Create(m_pGraphicDev);
		//		//CEFT_BulletAttack* pEffect = CEFT_BulletAttack::Create(m_pGraphicDev);
		//		pEffect->SetInfo((_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos() + _tCollisionInfo.pCollidedMyCollider->GetTransform()->GetPos()) * 0.5f);
		//		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
		//	}
		//}
	}
}

void CBoss::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (IsDead())
		return;

	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0) {
			// 충돌하지 않았다면 어태커에서 제거한다.
			m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
		}
	}
}

CBoss * CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss*	pInstance = new CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CBoss::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

void CBoss::SetHPBarUI(CUI_HPBar * _pHPBarUI)
{
	m_pHPBar = _pHPBarUI;
	m_pHPBar->On(this);
}

_bool CBoss::IsFalling(const _float & _fDeltaTime)
{
	if (GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded)
		return true;
	return false;
}

_bool CBoss::IsDead() const
{
	return m_pAttribute->IsDead();
}
