#include "pch.h"
#include "BolterFlyState_Idle.h"
#include "BolterFlyState_Death.h"
#include "BolterFlyState_Alert.h"
#include "BolterFlyState_Damage.h"
#include "BolterFlyState_Attack.h"
#include "StateMgr.h"
#include "BolterFly.h"
#include "Map.h"
#include "Attribute.h"


CBolterFlyState_Idle::CBolterFlyState_Idle(CBolterFly & _rOwner)
	:
	CState(_rOwner)
{
}

CBolterFlyState_Idle::~CBolterFlyState_Idle()
{
}

void CBolterFlyState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_bIsAttackIdle = false;

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBolterFlyState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);


	if (fLength > BOLTERFLY_COGNITIVE_RANGE)
		return 0;


	if (fLength <= BOLTERFLY_ATTACK_RANGE) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Alert(m_rOwner));
		return 0;
	}

	m_rOwner.GetPhysics()->SetVelocity(vToTargetDir / fLength * BOLTERFLY_RUN_SPEED);

	// 타겟 위치를 향해 회전합니다.
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
	D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
	_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
	m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);


	//if (m_bIsAttackIdle) {
		//if (fLength > BOLTERFLY_COGNITIVE_RANGE) {
		//	// 인지 범위 밖에 있다면 일반 Idle상태를 유지합니다.
		//	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
		//	m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vLook.x, vLook.z) * BOLTERFLY_RUN_SPEED);
		//	//m_bIsAttackIdle = false;
		////	m_fElapsedTime = 0.f;
		//}
		//else if ((m_fElapsedTime += _fDeltaTime) >= 2.f) {
		//	// TODO : 인지 후 1초가 지났다면 경계 태세로 전환합니다.
		//	m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Alert(m_rOwner));
		//}
		//else {
		//	// 타겟 위치를 향해 회전합니다.
		//	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		//	_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
		//	D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
		//	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
		//	_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
		//	m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
		//}
	//}
	//else {
	//	if (fLength <= BOLTERFLY_COGNITIVE_RANGE) {
	//		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk_2);
	//		m_bIsAttackIdle = true;
	//		m_fElapsedTime = 0.f;
	//	}
		// 인지 범위 밖에 있다면 아무것도 하지 않는다.
	//}

	return 0;
}

void CBolterFlyState_Idle::OnExited(void)
{
}

void CBolterFlyState_Idle::Free(void)
{
}
