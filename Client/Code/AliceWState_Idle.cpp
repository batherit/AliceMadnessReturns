#include "pch.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"


CAliceWState_Idle::CAliceWState_Idle(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Idle::~CAliceWState_Idle()
{
}

void CAliceWState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_Idle);
}

int CAliceWState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	// Idle => Death, Jump, Run, Attack
	if (m_rOwner.IsJumpOn(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner));
	}
	else if (m_rOwner.IsAttackOn(_fDeltaTime))  {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack(m_rOwner));
	}
	else if (m_rOwner.ProcessMove(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
	}

	return 0;
}

void CAliceWState_Idle::OnExited(void)
{
}

void CAliceWState_Idle::Free(void)
{
}
