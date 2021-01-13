#include "pch.h"
#include "MiniGruntState_Alert.h"
#include "MiniGruntState_Death.h"
#include "MiniGruntState_Idle.h"
#include "MiniGruntState_Attack.h"
#include "MiniGruntState_Damage.h"
#include "Attribute.h"
#include "StateMgr.h"
#include "MiniGrunt.h"
#include "AliceW.h"


CMiniGruntState_Alert::CMiniGruntState_Alert(CMiniGrunt & _rOwner)
	:
	CState(_rOwner)
{
}

CMiniGruntState_Alert::~CMiniGruntState_Alert()
{
}

void CMiniGruntState_Alert::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_3_F);
	m_eStateType = TYPE_PURSUE;

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CMiniGruntState_Alert::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);

	if (fLength > MINIGRUNT_COGNITIVE_RANGE * 2) {
		// 인지범위 밖 * 2 초과 위치에 타겟이 존재한다면 Idle상태로 전환한다.
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Idle(m_rOwner));
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

	switch (m_eStateType)
	{
	case TYPE_PURSUE:
		if (fLength > MINIGRUNT_ATTACK_RANGE) {
			// 플레이어를 향해 달린다.

			m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vLook.x, vLook.z) * MINIGRUNT_RUN_SPEED);
		}
		else {
			m_eStateType = TYPE_ALERT;
			if (m_bIsRightMoving) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Right);
				
			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Left);
			}
			m_fTickTime = 0.f;
			m_fElapsedTime = 0.f;
		}
		break;
	case TYPE_ALERT:
		if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Attack(m_rOwner));
		}
		else if (fLength > MINIGRUNT_ATTACK_RANGE + 1.f) {
			// 추격 상태로 전환한다.
			m_eStateType = TYPE_PURSUE;
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_3_F);
		}
		else if (fLength < MINIGRUNT_ATTACK_RANGE - 1.f) {
			m_eStateType = TYPE_EVADE;
		}
		else {
			if ((m_fTickTime += _fDeltaTime) >= 1.5f) {
				m_bIsRightMoving = Engine::GetRandomBoolean();
				if (m_bIsRightMoving) {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Right);
				}
				else {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Left);
				}
				m_fTickTime = 0.f;
			}

			if (m_bIsRightMoving) {
				m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vRight.x, vRight.z) * MINIGRUNT_RUN_SPEED * 0.5f);
			}
			else {
				m_rOwner.GetPhysics()->SetVelocityXZ(-_vec2(vRight.x, vRight.z) * MINIGRUNT_RUN_SPEED * 0.5f);
			}
		}
		break;
	case TYPE_EVADE: {
		if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Attack(m_rOwner));
		}
		else if (fLength > MINIGRUNT_ATTACK_RANGE) {
			m_eStateType = TYPE_ALERT;
			if (m_bIsRightMoving) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Right);
			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Left);
			}
			m_fTickTime = 0.f;
		}
		else {
			// 새로운 속도를 갱신한다.
			CAliceW* pPlayer = dynamic_cast<CAliceW*>(m_rOwner.GetTargetObject());
			if (!pPlayer) {
				// 잘못된 타겟이 세팅되었다면, Idle상태로 전환한다.
				m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Idle(m_rOwner));
				return 0;
			}

			_vec3 vRevTargetVelXZ = -pPlayer->GetPhysics()->GetVelocity();
			vRevTargetVelXZ.y = 0.f;
			D3DXVec3Normalize(&vRevTargetVelXZ, &vRevTargetVelXZ);
			_vec3 vNewVelXZ = *D3DXVec3Normalize(&vRevTargetVelXZ, &(vRevTargetVelXZ - vToTargetDirXZ)) * MINIGRUNT_RUN_SPEED * 0.5f;
			m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vNewVelXZ.x, vNewVelXZ.z));

			E_DIR eDir = GetVelDirXZType(_vec2(vNewVelXZ.x, vNewVelXZ.z));

			if (m_eDir != eDir) {
				m_eDir = eDir;
				switch (m_eDir)
				{
				case DIR_FORWARD:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_F);
					break;
				case DIR_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Right);
					break;
				case DIR_BACK:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_B);
					break;
				case DIR_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Strafe_Left);
					break;
				default:
					break;
				}
			}
		}

	}
					 break;
	default:
		break;
	}

	return 0;
}

void CMiniGruntState_Alert::OnExited(void)
{
}

void CMiniGruntState_Alert::Free(void)
{
}

CMiniGruntState_Alert::E_DIR CMiniGruntState_Alert::GetVelDirXZType(const _vec2 & _vVelDirXZ)
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
