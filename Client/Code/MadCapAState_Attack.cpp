#include "pch.h"
#include "MadCapAState_Attack.h"
#include "MadCapAState_Death.h"
#include "MadCapAState_Idle.h"
//#include "MadCapAState_Run.h"
//#include "MadCapAState_Attack.h"
//#include "MadCapAState_Jump.h"
//#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "MadCapA.h"
#include "Map.h"


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
}

int CMadCapAState_Attack::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Death(m_rOwner));
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
	}

	return 0;
}

void CMadCapAState_Attack::OnExited(void)
{
}

void CMadCapAState_Attack::Free(void)
{
}
