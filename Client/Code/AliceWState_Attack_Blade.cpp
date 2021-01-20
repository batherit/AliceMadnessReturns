#include "pch.h"
#include "AliceWState_Attack_Blade.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Death.h"
#include "AliceWState_Damage.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Dash.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_Attack_Blade::CAliceWState_Attack_Blade(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Attack_Blade::~CAliceWState_Attack_Blade()
{
}

void CAliceWState_Attack_Blade::OnLoaded(void)
{
	m_bIsAttacking = true;
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_1_A);
	m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 2.f);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.85f);
	++m_iAttackNum;

	//m_pWeaponCollider = m_rOwner.GetWeapon()->GetColliderFromTag(L"PlayerAttack");
	//m_pWeaponCollider->SetActivated(true);
	m_rOwner.GetAttackCollider()->SetActivated(true);
}

int CAliceWState_Attack_Blade::Update(const _float& _fDeltaTime)
{
	// Attack_Blade => Death, Jump, Idle, Attack_Blade
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
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, m_rOwner.GetTransform()->GetLook()));
		return 0;
	}

	_vec3 vDir;
	if (m_bIsAttacking) {
		// ���� ����� ����ǰ� �ִ� ���¿���,,,

		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// ������ ����Ǿ�����, ���� ��� �ִϸ��̼��� ������.
			switch (m_iAttackNum)
			{
			case 1:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_1_B);
				break;
			case 2:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_2_B);
				break;
			case 3:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_3_B);
				break;
			case 4:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_4_B);
				break;
			}
			m_bIsAttacking = false;
		}
		else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= m_fEntryTime[m_iAttackNum]) {
			// �Է� ���� ����(���� �ִϸ��̼��� 70�ۼ�Ʈ ���������)�� ������,
			// ���ݿ� ���� �Է��� ���ǰ� ���ο� ���� �Է��� �������� ���� ������ �����Ѵ�.
			if (m_rOwner.IsAttackOn(_fDeltaTime)) {
				if (m_iAttackNum < 4) {
					switch (m_iAttackNum)
					{
					case 1:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_2_A);
						break;
					case 2:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_3_A);
						m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 2.f);
						m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
						break;
					case 3:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP1_Mele_Attack_4_A);
						m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 3.f);
						m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
						break;
					}
					++m_iAttackNum;
				}
			}
			else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.6f && m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
				_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
				m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
			}
			// else � �Էµ� ���� ���ߴٸ� �ƹ��͵� ���� �ʴ´�.
		}
	}
	else if(m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()){
		// ���� ���� ����� ������ ���� �����ӿ� ���� ���¸� �����Ѵ�.
		/*if (m_rOwner.IsAttackOn(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack_Blade(m_rOwner));
		}
		else */
		if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CAliceWState_Attack_Blade::OnExited(void)
{
	//m_pWeaponCollider->SetActivated(false);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CAliceWState_Attack_Blade::Free(void)
{
}
