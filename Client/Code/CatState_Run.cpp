#include "pch.h"
#include "CatState_Run.h"
#include "CatState_Idle.h"
#include "StateMgr.h"
#include "Cat.h"
#include "Map.h"
#include "Attribute.h"


CCatState_Run::CCatState_Run(CCat & _rOwner)
	:
	CState(_rOwner)
{
}

CCatState_Run::~CCatState_Run()
{
}

void CCatState_Run::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Cat_Run);
}

int CCatState_Run::Update(const _float& _fDeltaTime)
{
	_vec3 vTargetPos = m_rOwner.GetTargetPos();
	_vec3 vDir = m_rOwner.GetTargetPos() - m_rOwner.GetTransform()->GetPos();
	D3DXVec3Normalize(&vDir, &vDir);
	m_rOwner.GetPhysics()->SetVelocity(vDir * ALICE_RUN_SPEED);

	if (D3DXVec3LengthSq(&(vTargetPos - m_rOwner.GetTransform()->GetPos())) < 0.1f ||
		D3DXVec3Dot(&vDir, &(vTargetPos - m_rOwner.GetTransform()->GetPos())) <= 0.f) {
		m_rOwner.GetTransform()->SetPos(vTargetPos);
		m_rOwner.GetStateMgr()->SetNextState(new CCatState_Idle(m_rOwner));
		return 0;
	}
	else {
		_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vDir) * 0.15f;
		m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
		m_rOwner.GetTransform()->SetPos(m_rOwner.GetPhysics()->GetUpdatedPos(_fDeltaTime));
	}

	return 0;
}

void CCatState_Run::OnExited(void)
{
	m_rOwner.SetGoOn(false, _vec3(0.f, 0.f, 0.f));
}

void CCatState_Run::Free(void)
{
}