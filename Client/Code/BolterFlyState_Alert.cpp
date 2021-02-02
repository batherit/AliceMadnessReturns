#include "pch.h"
#include "BolterFlyState_Alert.h"
#include "BolterFlyState_Death.h"
#include "BolterFlyState_Idle.h"
#include "BolterFlyState_Attack.h"
#include "BolterFlyState_Damage.h"
#include "Attribute.h"
#include "StateMgr.h"
#include "BolterFly.h"
#include "AliceW.h"


CBolterFlyState_Alert::CBolterFlyState_Alert(CBolterFly & _rOwner)
	:
	CState(_rOwner)
{
}

CBolterFlyState_Alert::~CBolterFlyState_Alert()
{
}

void CBolterFlyState_Alert::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
	m_rOwner.GetPhysics()->SetVelocityY(0.f);
	m_eStateType = TYPE_PURSUE;

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());


	m_vDir = Engine::GetRotatedVector(-WORLD_X_AXIS, Engine::GetNumberBetweenMinMax(D3DXToRadian(20.f), D3DXToRadian(40.f)), m_vDir);
	m_vDir = Engine::GetRotatedVector(WORLD_Y_AXIS, Engine::GetNumberBetweenMinMax(0.f, D3DX_PI * 2.f), m_vDir);
}

int CBolterFlyState_Alert::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Damage(m_rOwner));
		return 0;
	}

	CAliceW* pAliceW = dynamic_cast<CAliceW*>(m_rOwner.GetTargetObject());
	if (pAliceW && pAliceW->IsFalling(0.f)) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
		return 0;
	}

	_vec3 vTargetPos = m_rOwner.GetTargetObject()->GetTransform()->GetPos();
	_vec3 vToTargetDir = vTargetPos - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);

	if (fLength > BOLTERFLY_COGNITIVE_RANGE) {
		// 인지범위 밖 위치에 타겟이 존재한다면 Idle상태로 전환한다.
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
		return 0;
	}

	// 타겟 위치를 향해 회전합니다.
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	_vec3 vRight;
	_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
	D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
	_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
	m_rOwner.GetTransform()->RotateByAxis(fRotAngle, vRotAxis);

	vLook = m_rOwner.GetTransform()->GetLook();
	vRight = m_rOwner.GetTransform()->GetRight();

	_vec3 vToEnd = vTargetPos + m_vDir * BOLTERFLY_ATTACK_RANGE - m_rOwner.GetTransform()->GetPos();
	_float fToEndLength = D3DXVec3Length(&vToEnd);

	//if (fToEndLength > BOLTERFLY_ATTACK_RANGE - BOLTERFLY_ATTACK_RANGE * 0.5f) {
		// 목표점을 향해 달린다.
		//if (fToEndLength > 0.f) {
		m_rOwner.GetPhysics()->SetVelocity(vToEnd / fToEndLength * BOLTERFLY_RUN_SPEED);
		//}
	//}

	switch (m_eStateType)
	{
	case TYPE_PURSUE:
		//if (fToEndLength > BOLTERFLY_ATTACK_RANGE - BOLTERFLY_ATTACK_RANGE * 0.5f) {
		//	// 플레이어를 향해 달린다.
		//	//if (fToEndLength > 0.f) {
		//		m_rOwner.GetPhysics()->SetVelocity(vToEnd / fToEndLength * BOLTERFLY_RUN_SPEED);
		//	//}
		//}
		//else {
		if(fToEndLength <= BOLTERFLY_ATTACK_RANGE - BOLTERFLY_ATTACK_RANGE * 0.5f) {
			m_eStateType = TYPE_ALERT;
			if (m_bIsRightMoving) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Rgt);

			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Lft);
			}
			m_fTickTime = 0.f;
			m_fElapsedTime = 0.f;
		}
		break;
	case TYPE_ALERT:
		if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Attack(m_rOwner));
			return 0;
		}
		else if (fLength > BOLTERFLY_ATTACK_RANGE + 4.f) {
			// 추격 상태로 전환한다.
			m_eStateType = TYPE_PURSUE;
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
		}
		/*else if (fLength < BOLTERFLY_ATTACK_RANGE - 1.f) {
			m_eStateType = TYPE_EVADE;
		}*/
		else {
			if ((m_fTickTime += _fDeltaTime) >= 1.5f) {
				m_bIsRightMoving = Engine::GetRandomBoolean();
				if (m_bIsRightMoving) {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Rgt);
				}
				else {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Lft);
				}
				m_fTickTime = 0.f;
			}

			if (m_bIsRightMoving) {
				m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vRight.x, vRight.z) * BOLTERFLY_RUN_SPEED * 0.5f);
			}
			else {
				m_rOwner.GetPhysics()->SetVelocityXZ(-_vec2(vRight.x, vRight.z) * BOLTERFLY_RUN_SPEED * 0.5f);
			}
		}
		break;
	//case TYPE_EVADE: {
	//	if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
	//		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Attack(m_rOwner));
	//	}
	//	else if (fLength > BOLTERFLY_ATTACK_RANGE) {
	//		m_eStateType = TYPE_ALERT;
	//		if (m_bIsRightMoving) {
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Rgt);
	//		}
	//		else {
	//			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Lft);
	//		}
	//		m_fTickTime = 0.f;
	//	}
	//	else {
	//		// 새로운 속도를 갱신한다.
	//		CAliceW* pPlayer = dynamic_cast<CAliceW*>(m_rOwner.GetTargetObject());
	//		if (!pPlayer) {
	//			// 잘못된 타겟이 세팅되었다면, Idle상태로 전환한다.
	//			m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
	//			return 0;
	//		}

	//		_vec3 vRevTargetVelXZ = -pPlayer->GetPhysics()->GetVelocity();
	//		vRevTargetVelXZ.y = 0.f;
	//		D3DXVec3Normalize(&vRevTargetVelXZ, &vRevTargetVelXZ);
	//		_vec3 vNewVelXZ = *D3DXVec3Normalize(&vRevTargetVelXZ, &(vRevTargetVelXZ - vToTargetDirXZ)) * BOLTERFLY_RUN_SPEED * 0.5f;
	//		m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vNewVelXZ.x, vNewVelXZ.z));

	//		E_DIR eDir = GetVelDirXZType(_vec2(vNewVelXZ.x, vNewVelXZ.z));

	//		if (m_eDir != eDir) {
	//			m_eDir = eDir;
	//			switch (m_eDir)
	//			{
	//			case DIR_FORWARD:
	//				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
	//				break;
	//			case DIR_RIGHT:
	//				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Rgt);
	//				break;
	//			case DIR_BACK:
	//				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Walk);
	//				break;
	//			case DIR_LEFT:
	//				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Strafe_Lft);
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//	}

	//}
					 break;
	default:
		break;
	}

	return 0;
}

void CBolterFlyState_Alert::OnExited(void)
{
}

void CBolterFlyState_Alert::Free(void)
{
}

CBolterFlyState_Alert::E_DIR CBolterFlyState_Alert::GetVelDirXZType(const _vec2 & _vVelDirXZ)
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
