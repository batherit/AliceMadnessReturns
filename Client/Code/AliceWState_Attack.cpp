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
		// 공격 모션이 진행되고 있는 상태에서,,,
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// 공격이 종료되었으면, 종료 모션 애니메이션을 돌린다.
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
			// 입력 가능 상태(공격 애니메이션이 70퍼센트 진행됐을때)가 됐을때,
			// 공격에 대한 입력을 살피고 새로운 공격 입력이 들어왔으면 다음 공격을 실행한다.
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
			// else 어떤 입력도 받지 못했다면 아무것도 하지 않는다.
		}
	}
	else if(m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()){
		_vec3 vDir;
		// 공격 종료 모션이 끝나고 이후 움직임에 따라 상태를 변경한다.
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
