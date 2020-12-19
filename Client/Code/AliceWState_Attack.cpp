#include "pch.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"


CAliceWState_Attack::CAliceWState_Attack(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Attack::~CAliceWState_Attack()
{
}

void CAliceWState_Attack::OnLoaded(void)
{
	m_bIsAttacking = true;
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_1_A);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	++m_iAttackNum;
}

int CAliceWState_Attack::Update(const _float& _fDeltaTime)
{
	// Attack => Death, Jump, Idle, Attack
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_bIsAttacking) {
		// ���� ����� ����ǰ� �ִ� ���¿���,,,
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// ������ ����Ǿ�����, ���� ��� �ִϸ��̼��� ������.
			switch (m_iAttackNum)
			{
			case 1:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_1_B);
				break;
			case 2:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_2_B);
				break;
			case 3:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_5_B);
				break;
			}
			m_bIsAttacking = false;
		}
		else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.35f) {
			// �Է� ���� ����(���� �ִϸ��̼��� 70�ۼ�Ʈ ���������)�� ������,
			// ���ݿ� ���� �Է��� ���ǰ� ���ο� ���� �Է��� �������� ���� ������ �����Ѵ�.
			if (m_rOwner.IsAttackOn(_fDeltaTime)) {
				if (m_iAttackNum < 3) {
					switch (m_iAttackNum)
					{
					case 1:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_2_A);
						break;
					case 2:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_WP1_Mele_Attack_5_A);
						break;
					}
					++m_iAttackNum;
				}
			}
			// else � �Էµ� ���� ���ߴٸ� �ƹ��͵� ���� �ʴ´�.
		}
	}
	else if(m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()){
		_vec3 vDir;
		// ���� ���� ����� ������ ���� �����ӿ� ���� ���¸� �����Ѵ�.
		if (m_rOwner.IsAttackOn(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack(m_rOwner));
		}
		else if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
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

void CAliceWState_Attack::OnExited(void)
{
}

void CAliceWState_Attack::Free(void)
{
}
