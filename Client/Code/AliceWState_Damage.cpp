#include "pch.h"
#include "AliceWState_Damage.h"
#include "AliceWState_Death.h"
#include "AliceWState_Idle.h"
#include "AliceWState_SuperJump.h"
//#include "MadCapAState_Run.h"
//#include "MadCapAState_Attack.h"
//#include "MadCapAState_Jump.h"
//#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_Damage::CAliceWState_Damage(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Damage::~CAliceWState_Damage()
{
}

void CAliceWState_Damage::OnLoaded(void)
{
	if (Engine::GetRandomBoolean()) {
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Damg_Lft_Lt);
	}
	else {
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Damg_Rgt_Lt);
	}
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);
	m_rOwner.GetAttribute()->SetDamaged(false);
}

int CAliceWState_Damage::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsSuperJumpOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_SuperJump(m_rOwner));
		m_rOwner.SetSuperJumped(false);
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Damage(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
	}

	return 0;
}

void CAliceWState_Damage::OnExited(void)
{
	//m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CAliceWState_Damage::Free(void)
{
}
