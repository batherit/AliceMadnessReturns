#include "pch.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"


CAliceWState_Slide::CAliceWState_Slide(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Slide::~CAliceWState_Slide()
{
}

void CAliceWState_Slide::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceL_Slide);
}

int CAliceWState_Slide::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	_vec3 vSettedPos = m_rOwner.GetTransform()->GetPos();
	if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
	}
	// Slide => Death, Jump, Idle, Attack
	if (!m_rOwner.IsSliding(_fDeltaTime) && m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
	}

	return 0;
}

void CAliceWState_Slide::OnExited(void)
{
}

void CAliceWState_Slide::Free(void)
{
}
