#include "pch.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack_Blade.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Death.h"
#include "AliceWState_Damage.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


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
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CAliceWState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	_vec3 vSettedPos = m_rOwner.GetTransform()->GetPos();
	// Idle => Death, Jump, Run, Attack
	if (m_rOwner.IsJumpOn(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner));
	}
	else if (m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
	}
	else if (m_rOwner.IsAttackOn(_fDeltaTime))  {
		switch (m_rOwner.GetWeaponType()) {
		case CAliceW::TYPE_BLADE:
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack_Blade(m_rOwner));
			break;
		}
	}
	else if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
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
