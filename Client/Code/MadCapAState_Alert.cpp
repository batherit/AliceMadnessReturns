#include "pch.h"
#include "MadCapAState_Alert.h"
#include "MadCapAState_Death.h"
#include "MadCapAState_Idle.h"
#include "MadCapAState_Attack.h"
#include "MadCapAState_Damage.h"
#include "Attribute.h"
#include "StateMgr.h"
#include "MadCapA.h"
#include "AliceW.h"


CMadCapAState_Alert::CMadCapAState_Alert(CMadCapA & _rOwner)
	:
	CState(_rOwner)
{
}

CMadCapAState_Alert::~CMadCapAState_Alert()
{
}

void CMadCapAState_Alert::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Run);
	m_eStateType = TYPE_PURSUE;

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CMadCapAState_Alert::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);

	if (fLength > MADCAPA_COGNITIVE_RANGE * 2) {
		// �������� �� * 2 �ʰ� ��ġ�� Ÿ���� �����Ѵٸ� Idle���·� ��ȯ�Ѵ�.
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Idle(m_rOwner));
		return 0;
	}
	
	// Ÿ�� ��ġ�� ���� ȸ���մϴ�.
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
		if (fLength > MADCAPA_ATTACK_RANGE) {
			// �÷��̾ ���� �޸���.
			
			m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vLook.x, vLook.z) * MADCAPA_RUN_SPEED);
		}
		else {
			m_eStateType = TYPE_ALERT;
			if (m_bIsRightMoving) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Right);
			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_left);
			}
			m_fTickTime = 0.f;
			m_fElapsedTime = 0.f;
		}
		break;
	case TYPE_ALERT:
		if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Attack(m_rOwner));
		}
		else if (fLength > MADCAPA_ATTACK_RANGE + 1.f) {
			// �߰� ���·� ��ȯ�Ѵ�.
			m_eStateType = TYPE_PURSUE;
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Run);
		}
		else if (fLength < MADCAPA_ATTACK_RANGE - 1.f) {
			m_eStateType = TYPE_EVADE;
		}
		else {
			if ((m_fTickTime += _fDeltaTime) >= 1.5f) {
				m_bIsRightMoving = Engine::GetRandomBoolean();
				if (m_bIsRightMoving) {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Right);
				}
				else {
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_left);
				}
				m_fTickTime = 0.f;
			}

			if (m_bIsRightMoving) {
				m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vRight.x, vRight.z) * MADCAPA_RUN_SPEED * 0.5f);
			}
			else {
				m_rOwner.GetPhysics()->SetVelocityXZ(-_vec2(vRight.x, vRight.z) * MADCAPA_RUN_SPEED * 0.5f);
			}
		}
		break;
	case TYPE_EVADE: {
		if ((m_fElapsedTime += _fDeltaTime) > 3.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Attack(m_rOwner));
		}
		else if (fLength > MADCAPA_ATTACK_RANGE) {
			m_eStateType = TYPE_ALERT;
			if (m_bIsRightMoving) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Right);
			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_left);
			}
			m_fTickTime = 0.f;
		}
		else {
			// ���ο� �ӵ��� �����Ѵ�.
			CAliceW* pPlayer = dynamic_cast<CAliceW*>(m_rOwner.GetTargetObject());
			if (!pPlayer) {
				// �߸��� Ÿ���� ���õǾ��ٸ�, Idle���·� ��ȯ�Ѵ�.
				m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Idle(m_rOwner));
				return 0;
			}

			_vec3 vRevTargetVelXZ = -pPlayer->GetPhysics()->GetVelocity();
			vRevTargetVelXZ.y = 0.f;
			D3DXVec3Normalize(&vRevTargetVelXZ, &vRevTargetVelXZ);
			_vec3 vNewVelXZ = *D3DXVec3Normalize(&vRevTargetVelXZ, &(vRevTargetVelXZ - vToTargetDirXZ)) * MADCAPA_RUN_SPEED * 0.5f;
			m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vNewVelXZ.x, vNewVelXZ.z));

			E_DIR eDir = GetVelDirXZType(_vec2(vNewVelXZ.x, vNewVelXZ.z));

			if (m_eDir != eDir) {
				m_eDir = eDir;
				switch (m_eDir)
				{
				case DIR_FORWARD:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Forward);
					break;
				case DIR_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Right);
					break;
				case DIR_BACK:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_Back);
					break;
				case DIR_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Strafe_left);
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

void CMadCapAState_Alert::OnExited(void)
{
}

void CMadCapAState_Alert::Free(void)
{
}

CMadCapAState_Alert::E_DIR CMadCapAState_Alert::GetVelDirXZType(const _vec2 & _vVelDirXZ)
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
	else if(vRotAxis.y < 0.f){
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
