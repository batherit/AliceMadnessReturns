#include "pch.h"
#include "GiantAliceWState_Idle.h"
#include "GiantAliceWState_Walk.h"
#include "GiantAliceWState_Attack.h"
#include "StateMgr.h"
#include "GiantAliceW.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"


CGiantAliceWState_Idle::CGiantAliceWState_Idle(CGiantAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CGiantAliceWState_Idle::~CGiantAliceWState_Idle()
{
}

void CGiantAliceWState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceGiant_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CGiantAliceWState_Idle::Update(const _float& _fDeltaTime)
{
	_vec3 vDir;

	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Walk(m_rOwner));
	}
	else if (m_rOwner.IsAttackOn(0.f)) {
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Attack(m_rOwner));
	}

	return 0;
}

void CGiantAliceWState_Idle::OnExited(void)
{
}

void CGiantAliceWState_Idle::Free(void)
{
}
