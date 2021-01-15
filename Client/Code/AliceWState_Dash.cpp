#include "pch.h"
#include "AliceWState_Dash.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Bomb.h"
#include "AliceWState_Attack_Blade.h"
#include "AliceWState_Attack_Horse.h"
#include "AliceWState_GunMode.h"
#include "AliceWState_Jump.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Death.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Damage.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_Dash::CAliceWState_Dash(CAliceW & _rOwner, const _vec3& _vDir)
	:
	CState(_rOwner),
	m_vDir(_vDir)
{
}

CAliceWState_Dash::~CAliceWState_Dash()
{
}

void CAliceWState_Dash::OnLoaded(void)
{
	m_rOwner.SetVisible(false);
	m_rOwner.GetPhysics()->SetGravity(0.f);
	m_rOwner.GetPhysics()->SetVelocity(m_vDir * ALICE_RUN_SPEED * 2.f);
	m_fElapsedTime = 0.f;
	m_rOwner.SetDashing(true);
}

int CAliceWState_Dash::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	_float fT = 0.f;
	if ((m_fElapsedTime += _fDeltaTime) < DASH_TIME * 0.5f) {
		fT = Engine::GetWeightByValue(m_fElapsedTime, 0.f, DASH_TIME * 0.5f);
		m_rOwner.GetPhysics()->SetSpeed(Engine::GetValueByWeight(fT, ALICE_RUN_SPEED * 2.f, ALICE_RUN_SPEED * 3.f));
	}
	else if ((m_fElapsedTime += _fDeltaTime) < DASH_TIME) {
		fT = Engine::GetWeightByValue(m_fElapsedTime, DASH_TIME * 0.5f, DASH_TIME);
		m_rOwner.GetPhysics()->SetSpeed(Engine::GetValueByWeight(fT, ALICE_RUN_SPEED * 3.f, ALICE_RUN_SPEED* 2.f));
	}
	else if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
		return 0;
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
		return 0;
	}

	return 0;
}

void CAliceWState_Dash::OnExited(void)
{
	m_rOwner.SetVisible(true);
	m_rOwner.SetDashing(false);
	m_rOwner.GetPhysics()->SetGravity(9.8f * 3.f);
}

void CAliceWState_Dash::Free(void)
{
}