#include "pch.h"
#include "MadCapAState_Attack.h"
#include "MadCapAState_Death.h"
#include "MadCapAState_Idle.h"
#include "MadCapAState_Damage.h"
//#include "MadCapAState_Run.h"
//#include "MadCapAState_Attack.h"
//#include "MadCapAState_Jump.h"
//#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "MadCapA.h"
#include "Map.h"
#include "Attribute.h"


CMadCapAState_Attack::CMadCapAState_Attack(CMadCapA & _rOwner)
	:
	CState(_rOwner)
{
}

CMadCapAState_Attack::~CMadCapAState_Attack()
{
}

void CMadCapAState_Attack::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Attack_Fork_Melee02_a);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_bIsAttacking = false;

	m_rOwner.GetAttackCollider()->SetActivated(true);
	//m_pWeaponCollider = m_rOwner.GetWeapon()->GetColliderFromTag(L"EnemyAttack");
	//m_pWeaponCollider->SetActivated(false);
}

int CMadCapAState_Attack::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Death(m_rOwner));
		return 0;
	}

	//if (m_rOwner.GetAttribute()->IsDamaged()) {
	//	// 공격 도중에는 데미지 모션으로 전환하지 않는다.
	//	m_rOwner.GetAttribute()->SetDamaged(false);
	//}
	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Damage(m_rOwner));
		return 0;
	}

	if (m_bIsAttacking) {
		/*if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.3f) {
			m_rOwner.GetPhysics()->SetSpeed(0.f);
		}
*/
		if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
			m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Idle(m_rOwner));
			m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
		}
	}
	else if(m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f){
		m_bIsAttacking = true;
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Attack_Fork_Melee02_b);
		_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		_vec3 vRight = m_rOwner.GetTransform()->GetRight();
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, -D3DXToRadian(60.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_rOwner.GetPhysics()->SetVelocity(vLook * MADCAPA_RUN_SPEED * 12.f);
		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.93f);
		//m_pWeaponCollider->SetActivated(true);
		m_rOwner.GetAttackCollider()->SetActivated(true);
		PlayAttackSound();
	}

	return 0;
}

void CMadCapAState_Attack::OnExited(void)
{
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CMadCapAState_Attack::Free(void)
{
}

void CMadCapAState_Attack::PlayAttackSound()
{
	switch (Engine::GetNumberBetweenMinMax(0, 3)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Attack0.ogg", CSoundMgr::MONSTER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Attack1.ogg", CSoundMgr::MONSTER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Attack2.ogg", CSoundMgr::MONSTER);
		break;
	case 3:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Attack3.ogg", CSoundMgr::MONSTER);
		break;
	}

	switch (Engine::GetNumberBetweenMinMax(0, 2)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Swipe0.ogg", CSoundMgr::MONSTER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Swipe1.ogg", CSoundMgr::MONSTER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"MadCapA_Swipe2.ogg", CSoundMgr::MONSTER);
		break;
	}
}
