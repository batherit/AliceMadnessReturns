#include "pch.h"
#include "BigGuyBState_Walk.h"
#include "BigGuyBState_Talk.h"
#include "BigGuyBState_Idle.h"
#include "StateMgr.h"
#include "BigGuyB.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Route.h"


CBigGuyBState_Walk::CBigGuyBState_Walk(CBigGuyB & _rOwner)
	:
	CState(_rOwner)
{
}

CBigGuyBState_Walk::~CBigGuyBState_Walk()
{
}

void CBigGuyBState_Walk::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bigguy_LSP_Walk);
	m_vTargetPos = m_rOwner.GetRoute()->GetCurrentPosition();
	m_vDir = m_vTargetPos - m_rOwner.GetTransform()->GetPos();
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_rOwner.GetPhysics()->SetVelocity(m_vDir * ALICE_RUN_SPEED * 0.5f);
}

int CBigGuyBState_Walk::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsTalking()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Talk(m_rOwner));
		return 0;
	}

	if (D3DXVec3LengthSq(&(m_vTargetPos - m_rOwner.GetTransform()->GetPos())) < 0.1f ||
		D3DXVec3Dot(&m_vDir, &(m_vTargetPos - m_rOwner.GetTransform()->GetPos())) <= 0.f) {
		m_rOwner.GetTransform()->SetPos(m_vTargetPos);
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Idle(m_rOwner));
		return 0;
	}
	else {
		_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), m_vDir);
		_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), m_vDir) * 0.15f;
		m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
		m_rOwner.GetTransform()->SetPos(m_rOwner.GetPhysics()->GetUpdatedPos(_fDeltaTime));
	}


	return 0;
}

void CBigGuyBState_Walk::OnExited(void)
{
}

void CBigGuyBState_Walk::Free(void)
{
}
