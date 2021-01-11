#include "pch.h"
#include "AliceLState_Idle.h"
#include "AliceLState_Run.h"
#include "StateMgr.h"
#include "AliceL.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"


CAliceLState_Idle::CAliceLState_Idle(CAliceL & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceLState_Idle::~CAliceLState_Idle()
{
}

void CAliceLState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceI_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CAliceLState_Idle::Update(const _float& _fDeltaTime)
{
	_vec3 vDir;

	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
		m_rOwner.GetStateMgr()->SetNextState(new CAliceLState_Run(m_rOwner));
	}

	return 0;
}

void CAliceLState_Idle::OnExited(void)
{
}

void CAliceLState_Idle::Free(void)
{
}
