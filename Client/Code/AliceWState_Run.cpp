#include "pch.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"


CAliceWState_Run::CAliceWState_Run(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Run::~CAliceWState_Run()
{
}

void CAliceWState_Run::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_Run);
}

int CAliceWState_Run::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	_vec3 vSettedPos = m_rOwner.GetTransform()->GetPos();
	// Run => Death, Jump, Idle, Attack
	if (m_rOwner.IsJumpOn(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner));
	}
	else if (m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
	}
	else if (m_rOwner.IsAttackOn(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack(m_rOwner));
	}
	else if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
	}

	return 0;
}

void CAliceWState_Run::OnExited(void)
{
}

void CAliceWState_Run::Free(void)
{
}
