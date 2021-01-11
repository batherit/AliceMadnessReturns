#include "pch.h"
#include "AliceLState_Run.h"
#include "AliceLState_Idle.h"
#include "StateMgr.h"
#include "AliceL.h"
#include "Map.h"
#include "Attribute.h"


CAliceLState_Run::CAliceLState_Run(CAliceL & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceLState_Run::~CAliceLState_Run()
{
}

void CAliceLState_Run::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceI_Run);
	//m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceL_WP1_Run);
}

int CAliceLState_Run::Update(const _float& _fDeltaTime)
{
	_vec3 vDir;
	// Run => Death, Jump, Idle, Attack
	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
		// 플레이어 방향 전환이 이루어졌다면 회전시키기
		_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vDir) * 0.25f;
		m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceLState_Idle(m_rOwner));
	}

	return 0;
}

void CAliceLState_Run::OnExited(void)
{
}

void CAliceLState_Run::Free(void)
{
}