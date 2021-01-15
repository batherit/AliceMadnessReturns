#include "pch.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Bomb.h"
#include "AliceWState_Attack_Blade.h"
#include "AliceWState_Attack_Horse.h"
#include "AliceWState_GunMode.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Dash.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Death.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Damage.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_Run::CAliceWState_Run(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Run::~CAliceWState_Run()
{
}

void CAliceWState_Run::OnLoaded(void)
{
	//m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Run);
}

int CAliceWState_Run::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsSuperJumpOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_SuperJump(m_rOwner));
		m_rOwner.SetSuperJumped(false);
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Damage(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsDashOn()) {
		_vec3 vDir;
		if (m_rOwner.IsMoving(_fDeltaTime, &vDir))
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, vDir));
		else
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, m_rOwner.GetTransform()->GetLook()));
		return 0;
	}

	if (m_rOwner.IsBombOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Bomb(m_rOwner));
		return 0;
	}

	m_rOwner.IsWeaponChanging();

	_vec3 vDir;
	_float fRatio = m_rOwner.GetTransform()->GetScale().x;
	_vec3 vSettedPos = m_rOwner.GetTransform()->GetPos();
	// Run => Death, Jump, Idle, Attack
	if (m_rOwner.IsSliding(_fDeltaTime)) {
		if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED * fRatio);
		}
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Slide(m_rOwner));
	}
	else if (m_rOwner.IsJumpOn(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner));
	}
	else if (m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
	}
	else if (m_rOwner.IsAttackOn(_fDeltaTime)) {
		switch (m_rOwner.GetWeaponType()) {
		case CAliceW::TYPE_BLADE:
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack_Blade(m_rOwner));
			break;
		case CAliceW::TYPE_HORSE:
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack_Horse(m_rOwner));
			break;
		case CAliceW::TYPE_GUN:
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_GunMode(m_rOwner));
			break;
		}
	}
	else if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED * fRatio);
		if (m_rOwner.IsLockOn()) {
			_vec3 vNewVelXZ = m_rOwner.GetPhysics()->GetVelocity();
			E_DIR eDir = GetVelDirXZType(_vec2(vNewVelXZ.x, vNewVelXZ.z));

	/*		if (m_eDir != eDir) {*/
				m_eDir = eDir;
				switch (m_eDir)
				{
				case DIR_FORWARD:
					switch (m_rOwner.GetWeaponType()) {
					case CAliceW::TYPE_BLADE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Strafe_Fwd);
						break;
					case CAliceW::TYPE_HORSE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Strafe_Fwd);
						break;
					case CAliceW::TYPE_GUN:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Strafe_Fwd);
						break;
					}
					break;
				case DIR_RIGHT:
					switch (m_rOwner.GetWeaponType()) {
					case CAliceW::TYPE_BLADE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Strafe_Rgt);
						break;
					case CAliceW::TYPE_HORSE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Strafe_Rgt);
						break;
					case CAliceW::TYPE_GUN:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Strafe_Rgt);
						break;
					}
					break;
				case DIR_BACK:
					switch (m_rOwner.GetWeaponType()) {
					case CAliceW::TYPE_BLADE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Strafe_Bk);
						break;
					case CAliceW::TYPE_HORSE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Strafe_Bk);
						break;
					case CAliceW::TYPE_GUN:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Strafe_Bk);
						break;
					}
					break;
				case DIR_LEFT:
					switch (m_rOwner.GetWeaponType()) {
					case CAliceW::TYPE_BLADE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Strafe_Lft);
						break;
					case CAliceW::TYPE_HORSE:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Strafe_Lft);
						break;
					case CAliceW::TYPE_GUN:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Strafe_Lft);
						break;
					}
					break;
				default:
					break;
				}
			/*}*/
			// 몬스터 방향으로 회전
			_vec3 vToTargetDirXZ = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
			vToTargetDirXZ.y = 0.f;
			_float fLength = D3DXVec3Length(&vToTargetDirXZ);
			if (fLength > 0.f) {
				// 타겟 위치를 향해 회전합니다.
				_vec3 vLook = m_rOwner.GetTransform()->GetLook();
				vToTargetDirXZ /= fLength;
				_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
				_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
				m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
			}
		}
		else {
			// 플레이어 방향 전환이 이루어졌다면 회전시키기
			_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vDir);
			_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vDir) * 0.25f;
			m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
			switch (m_rOwner.GetWeaponType()) {
			case CAliceW::TYPE_BLADE:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Run);
				break;
			case CAliceW::TYPE_HORSE:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Run);
				break;
			case CAliceW::TYPE_GUN:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Run);
				break;
			}		
		}

		
	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
	}

	return 0;
}

void CAliceWState_Run::OnExited(void)
{
}

void CAliceWState_Run::Free(void)
{
}

CAliceWState_Run::E_DIR CAliceWState_Run::GetVelDirXZType(const _vec2 & _vVelDirXZ)
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
