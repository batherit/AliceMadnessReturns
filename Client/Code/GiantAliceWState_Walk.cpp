#include "pch.h"
#include "GiantAliceWState_Walk.h"
#include "GiantAliceWState_Idle.h"
#include "StateMgr.h"
#include "GiantAliceW.h"
#include "Map.h"
#include "Attribute.h"


CGiantAliceWState_Walk::CGiantAliceWState_Walk(CGiantAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CGiantAliceWState_Walk::~CGiantAliceWState_Walk()
{
}

void CGiantAliceWState_Walk::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceI_Walk);
	//m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::GiantAliceW_WP1_Walk);
}

int CGiantAliceWState_Walk::Update(const _float& _fDeltaTime)
{
	_vec3 vDir;
	// Walk => Death, Jump, Idle, Attack
	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
		// 플레이어 방향 전환이 이루어졌다면 회전시키기
		_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vDir) * 0.25f;
		m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Idle(m_rOwner));
	}

	return 0;
}

void CGiantAliceWState_Walk::OnExited(void)
{
}

void CGiantAliceWState_Walk::Free(void)
{
}