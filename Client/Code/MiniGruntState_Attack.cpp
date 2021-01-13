#include "pch.h"
#include "MiniGruntState_Attack.h"
#include "MiniGruntState_Death.h"
#include "MiniGruntState_Idle.h"
#include "MiniGruntState_Damage.h"
//#include "MiniGruntState_Run.h"
//#include "MiniGruntState_Attack.h"
//#include "MiniGruntState_Jump.h"
//#include "MiniGruntState_Death.h"
#include "StateMgr.h"
#include "MiniGrunt.h"
#include "Map.h"
#include "Attribute.h"


CMiniGruntState_Attack::CMiniGruntState_Attack(CMiniGrunt & _rOwner)
	:
	CState(_rOwner)
{
}

CMiniGruntState_Attack::~CMiniGruntState_Attack()
{
}

void CMiniGruntState_Attack::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_attack_01_A);
	m_rOwner.GetPhysics()->SetDirection(m_rOwner.GetTransform()->GetLook());
	//m_rOwner.GetPhysics()->SetSpeed(MINIGRUNT_RUN_SPEED * 3.f);
	//m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_bIsAttackEnd = false;

	m_rOwner.GetAttackR()->SetActivated(true);
}

int CMiniGruntState_Attack::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Death(m_rOwner));
		return 0;
	}

	//if (m_rOwner.GetAttribute()->IsDamaged()) {
	//	// 공격 도중에는 데미지 모션으로 전환하지 않는다.
	//	m_rOwner.GetAttribute()->SetDamaged(false);
	//}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Damage(m_rOwner));
		return 0;
	}

	//if (m_bIsAttackEnd) {
	//	//if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
	//		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Idle(m_rOwner));
	//		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	//		m_rOwner.GetPhysics()->SetSpeed(0.f);
	//	//}
	//}
	if (m_bIsAttackEnd) {
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Idle(m_rOwner));
			m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
			m_rOwner.GetPhysics()->SetSpeed(0.f);
			return 0;
		}
	}
	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
		m_bIsAttackEnd = true;
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_attack_01_B);
		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.25f);
	
		/*_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		_vec3 vRight = m_rOwner.GetTransform()->GetRight();
		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, -D3DXToRadian(60.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		m_rOwner.GetPhysics()->SetVelocity(vLook * MINIGRUNT_RUN_SPEED * 12.f);*/
		//m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.3f);
		
	}
	else {
		m_rOwner.GetPhysics()->SetSpeed(Engine::GetValueByWeight(m_rOwner.GetDynamicMesh()->GetAnimationProgress(), MINIGRUNT_RUN_SPEED * 0.5f, MINIGRUNT_RUN_SPEED * 5.f));
		/*if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() > 0.9f) {
			_vec3 v = m_rOwner.GetPhysics()->GetVelocity();
			int a = 10;
		}*/
	}
	//		
	//	
	//	
	//	//m_rOwner.GetPhysics()->SetSpeed(MINIGRUNT_RUN_SPEED * 3.f);
	//}

	return 0;
}

void CMiniGruntState_Attack::OnExited(void)
{
	m_rOwner.GetAttackR()->SetActivated(false);
}

void CMiniGruntState_Attack::Free(void)
{
}
