#include "pch.h"
#include "BossState_Walk.h"
#include "BossState_Run.h"
#include "BossState_Idle.h"
#include "BossState_Death.h"
#include "BossState_Attack_SideCut.h"
#include "BossState_Attack_JumpDown.h"
//#include "BossState_Attack_Slam.h"
#include "BossState_Attack_IronHand.h"
#include "BossState_Attack_Mixer.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"
#include "AliceW.h"


CBossState_Walk::CBossState_Walk(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Walk::~CBossState_Walk()
{
}

void CBossState_Walk::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Strafe_Fwd);
	m_rOwner.GetPhysics()->SetSpeed(BOSS_RUN_SPEED * 0.5f);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	m_fWanderingTime = Engine::GetNumberBetweenMinMax(3.f, 4.f);
}

int CBossState_Walk::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}


	// 플레이어를 향해 달린다.
	_vec3 vToPlayer = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fToPlayerLength = D3DXVec3Length(&vToPlayer);

	if (fToPlayerLength <= BOSS_ATTACK_RANGE) {
		switch (Engine::GetNumberBetweenMinMax(0, 2)) {
		case 0:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_SideCut(m_rOwner));
			break;
		case 1:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_IronHand(m_rOwner));
			break;
		case 2:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_JumpDown(m_rOwner));
			break;
		}
		
		return 0;
	}
	else if ((m_fWanderingTime -= _fDeltaTime) <= 0.f && fToPlayerLength > BOSS_ATTACK_RANGE) {
		// 배회 시간이 다되었는데 아직 공격 범위로 들어가지 못한 경우 => 달리거나 점프 공격을 시도한다.
		switch (Engine::GetNumberBetweenMinMax(0, 3)) {
		case 0:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Run(m_rOwner));
			break;
		case 1:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_JumpDown(m_rOwner));
			break;
		case 2:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_IronHand(m_rOwner));
			break;
		case 3:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_Mixer(m_rOwner));
			break;
		}
		
		return 0;
	}
	else if (m_fWanderingTime <= 0.f && fToPlayerLength <= BOSS_ATTACK_RANGE) {
		// 배회 시간이 다되었고 공격범위에 플레이어가 있으면, 바로 공격한다.
		switch(Engine::GetNumberBetweenMinMax(0, 2)) {
		case 0:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_SideCut(m_rOwner));
			break;
		case 1:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_JumpDown(m_rOwner));
			break;
		case 2:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_IronHand(m_rOwner));
			break;
		}
		return 0;
	}
	//else if (m_fWanderingTime > 0.f && fToPlayerLength <= BOSS_ATTACK_RANGE) {
	//	// TODO : EVADE를 시전합니다.
	//	// 새로운 속도를 갱신한다.
	//	CAliceW* pPlayer = dynamic_cast<CAliceW*>(m_rOwner.GetTargetObject());
	//	if (!pPlayer) {
	//		// 잘못된 타겟이 세팅되었다면, Idle상태로 전환한다.
	//		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
	//		return 0;
	//	}

	//	_vec3 vRevTargetVelXZ = -pPlayer->GetPhysics()->GetVelocity();
	//	vRevTargetVelXZ.y = 0.f;
	//	D3DXVec3Normalize(&vRevTargetVelXZ, &vRevTargetVelXZ);
	//	_vec3 vNewVelXZ = *D3DXVec3Normalize(&vRevTargetVelXZ, &(vRevTargetVelXZ - vToPlayer)) * BOSS_RUN_SPEED * 0.3f;
	//	m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vNewVelXZ.x, vNewVelXZ.z));

	//	E_DIR eDir = GetVelDirXZType(_vec2(vNewVelXZ.x, vNewVelXZ.z));

	//	if (m_eDir != eDir) {
	//		m_eDir = eDir;
	//		switch (m_eDir)
	//		{
	//		case DIR_FORWARD:
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Strafe_Fwd);
	//			break;
	//		case DIR_RIGHT:
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Strafe_Bk);
	//			break;
	//		case DIR_BACK:
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Strafe_Bk);
	//			break;
	//		case DIR_LEFT:
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Strafe_Bk);
	//			break;
	//		default:
	//			break;
	//		}
	//	}

	//}

	// xz에 투영된 방향 구해서 속도 적용하기.
	vToPlayer.y = 0.f;
	fToPlayerLength = D3DXVec3Length(&vToPlayer);

	if (fToPlayerLength <= 0.f)
		return 0;

	vToPlayer /= fToPlayerLength;	// normalize

	// 플레이어를 향해 회전시킨다.
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	_vec3 vToTargetDirXZ = _vec3(vToPlayer.x, 0.f, vToPlayer.z);
	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
	_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
	m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.6f, vRotAxis);

	// 플레이어를 향해 걷는다.
	m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * BOSS_RUN_SPEED * 0.3f);


	return 0;
}

void CBossState_Walk::OnExited(void)
{
}

void CBossState_Walk::Free(void)
{
}

CBossState_Walk::E_DIR CBossState_Walk::GetVelDirXZType(const _vec2 & _vVelDirXZ)
{
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	_vec3 vVel = m_rOwner.GetPhysics()->GetVelocity();
	_float fLength = 0.f;
	vVel.y = 0.f;
	fLength = D3DXVec3Length(&vVel);

	if (fLength <= 0.f)
		return DIR_END;

	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vVel);
	_float fRotDegree = D3DXToDegree(Engine::GetRotationAngle(vLook, vVel));

	if (vRotAxis.y == 0.f) {
		return DIR_END;
	}
	else if (vRotAxis.y < 0.f) {
		fRotDegree = 360.f - fRotDegree;
	}

	if (45.f < fRotDegree && fRotDegree <= 135.f) {
		return DIR_RIGHT;
	}
	else if (135.f < fRotDegree && fRotDegree <= 225.f) {
		return DIR_BACK;
	}
	else if (225.f < fRotDegree && fRotDegree <= 315.f) {
		return DIR_LEFT;
	}

	return DIR_FORWARD;
}
