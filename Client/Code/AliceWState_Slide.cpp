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
	if (m_rOwner.IsSliding(_fDeltaTime)) {
		_vec3 vCellNormalXZ = m_rOwner.GetMap()->GetNaviMesh()->GetCurCell()->GetNormal();
		vCellNormalXZ.y = 0.f;
		vCellNormalXZ *= 20.f;
		m_rOwner.GetPhysics()->AddVelocityXZ(_vec2(vCellNormalXZ.x, vCellNormalXZ.z) * _fDeltaTime);
		if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
			m_rOwner.GetPhysics()->AddVelocityXZ(_vec2(vDir.x, vDir.z) * _fDeltaTime);
		}
	}
	// Slide => Death, Jump, Idle, Attack
	else if (m_rOwner.IsFalling(_fDeltaTime)) {
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
