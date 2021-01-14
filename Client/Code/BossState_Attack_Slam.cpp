#include "pch.h"
#include "BossState_Attack_Slam.h"
#include "BossState_Idle.h"
#include "BossState_Run.h"
#include "BossState_Death.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"


CBossState_Attack_Slam::CBossState_Attack_Slam(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Attack_Slam::~CBossState_Attack_Slam()
{
}

void CBossState_Attack_Slam::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_AttackSlam);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.96f);
	m_rOwner.GetAttackCollider()->SetActivated(true);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBossState_Attack_Slam::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
		if (Engine::GetRandomBoolean()) {
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Run(m_rOwner));
		}
		return 0;
	}

	return 0;
}

void CBossState_Attack_Slam::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBossState_Attack_Slam::Free(void)
{
}
