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
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceGiant_Walk);
	m_pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	//m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::GiantAliceW_WP1_Walk);
}

int CGiantAliceWState_Walk::Update(const _float& _fDeltaTime)
{
	_vec3 vDir;
	// Walk => Death, Jump, Idle, Attack
	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED * 0.7f);
		// 플레이어 방향 전환이 이루어졌다면 회전시키기
		_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vDir);
		_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vDir) * 0.25f;
		m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Idle(m_rOwner));
	}

	m_pCameraMgr->GetCamera()->Shake(2.f, 0.08f * (1.f - Engine::GetWeightByValue(D3DXVec3Length(&m_rOwner.GetTransform()->GetPos()), 2.f, 20.f)), 10);

	return 0;
}

void CGiantAliceWState_Walk::OnExited(void)
{
}

void CGiantAliceWState_Walk::Free(void)
{
}