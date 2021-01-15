#include "pch.h"
#include "BossState_Attack_SideCut.h"
#include "BossState_Idle.h"
#include "BossState_Run.h"
#include "BossState_Death.h"
#include "BossState_Attack_JumpDown.h"
#include "BossState_Attack_IronHand.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"


CBossState_Attack_SideCut::CBossState_Attack_SideCut(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Attack_SideCut::~CBossState_Attack_SideCut()
{
}

void CBossState_Attack_SideCut::OnLoaded(void)
{
	switch (rand() % 3) {
	case 0:
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Attack01);
		break;
	case 1:
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Attack02);
		break;
	case 2:
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Attack03);
		break;
	}
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.96f);
	m_rOwner.GetAttackCollider()->SetActivated(true);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBossState_Attack_SideCut::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
		switch (Engine::GetNumberBetweenMinMax(0, 3)) {
		case 0:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			break;
		case 1:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Run(m_rOwner));
			break;
		case 2:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_JumpDown(m_rOwner));
			break;
		case 3:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_IronHand(m_rOwner));
			break;
		}
		
		return 0;
	}

	return 0;
}

void CBossState_Attack_SideCut::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBossState_Attack_SideCut::Free(void)
{
}
