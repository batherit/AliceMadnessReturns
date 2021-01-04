#include "pch.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Death.h"
#include "AliceWState_Slide.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"


CAliceWState_SuperJump::CAliceWState_SuperJump(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_SuperJump::~CAliceWState_SuperJump()
{
}

void CAliceWState_SuperJump::OnLoaded(void)
{
	m_rOwner.GetPhysics()->SetVelocityY(ALICE_JUMP_SPEED * 2.f);
	m_rOwner.GetPhysics()->SetGravity(9.8f * 1.5f);
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_Pad_Jump_Raise_C);
	m_rOwner.SetLanded(false);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.6f);
}

int CAliceWState_SuperJump::Update(const _float& _fDeltaTime)
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

	if (m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
		return 0;
	}

	return 0;
}

void CAliceWState_SuperJump::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_rOwner.GetPhysics()->SetGravity(9.8f * 3.f);
}

void CAliceWState_SuperJump::Free(void)
{
}
