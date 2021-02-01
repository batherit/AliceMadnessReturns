#include "pch.h"
#include "BolterFlyState_Attack.h"
#include "BolterFlyState_Death.h"
#include "BolterFlyState_Idle.h"
#include "BolterFlyState_Damage.h"
//#include "BolterFlyState_Run.h"
//#include "BolterFlyState_Attack.h"
//#include "BolterFlyState_Jump.h"
//#include "BolterFlyState_Death.h"
#include "StateMgr.h"
#include "BolterFly.h"
#include "Map.h"
#include "Attribute.h"


CBolterFlyState_Attack::CBolterFlyState_Attack(CBolterFly & _rOwner)
	:
	CState(_rOwner)
{
}

CBolterFlyState_Attack::~CBolterFlyState_Attack()
{
}

void CBolterFlyState_Attack::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Sprint);

	if (m_rOwner.GetTargetObject()) {
		m_vTargetPos = m_rOwner.GetTargetObject()->GetTransform()->GetPos();
		m_vTargetPos.y += 1.f;
		_vec3 vToPlayer = m_vTargetPos - m_rOwner.GetTransform()->GetPos();
		_float vToPlayerLength = D3DXVec3Length(&vToPlayer);
		if(vToPlayerLength > 0.f)
			m_rOwner.GetPhysics()->SetDirection(vToPlayer / vToPlayerLength);
			
		m_rOwner.GetPhysics()->SetSpeed(BOLTERFLY_RUN_SPEED * 7.5f);
		switch (Engine::GetNumberBetweenMinMax(0, 2)) {
		case 0:
			CSoundMgr::Get_Instance()->PlaySound(L"Bolterfly_Attack0.ogg", CSoundMgr::MONSTER);
			break;
		case 1:
			CSoundMgr::Get_Instance()->PlaySound(L"Bolterfly_Attack1.ogg", CSoundMgr::MONSTER);
			break;
		case 2:
			CSoundMgr::Get_Instance()->PlaySound(L"Bolterfly_Attack2.ogg", CSoundMgr::MONSTER);
			break;
		}
	}
	
	//m_rOwner.GetPhysics()->SetDirection(m_rOwner.GetTransform()->GetLook());
	//m_rOwner.GetPhysics()->SetSpeed(BolterFly_RUN_SPEED * 3.f);
	//m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_bIsAttackEnd = false;

	m_rOwner.GetAttackCollider()->SetActivated(true);
}

int CBolterFlyState_Attack::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Death(m_rOwner));
		return 0;
	}

	//if (m_rOwner.GetAttribute()->IsDamaged()) {
	//	// 공격 도중에는 데미지 모션으로 전환하지 않는다.
	//	m_rOwner.GetAttribute()->SetDamaged(false);
	//}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Damage(m_rOwner));
		return 0;
	}

	//if (m_bIsAttackEnd) {
	//	//if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
	//		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
	//		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	//		m_rOwner.GetPhysics()->SetSpeed(0.f);
	//	//}
	//}
	if (m_bIsAttackEnd) {
		//if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
			m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
			m_rOwner.GetPhysics()->SetSpeed(0.f);
			return 0;
		//}
	}
	else if ((m_fElapsedTime += _fDeltaTime) >= 0.5f) {
		m_bIsAttackEnd = true;
		m_rOwner.GetAttackCollider()->SetActivated(false);
	}
	else if(m_rOwner.GetTransform()->GetPos().y < m_vTargetPos.y - 0.5f) {
		m_rOwner.GetTransform()->SetPosY(m_vTargetPos.y - 0.5f);
	}
	//		
	//	
	//	
	//	//m_rOwner.GetPhysics()->SetSpeed(BolterFly_RUN_SPEED * 3.f);
	//}

	return 0;
}

void CBolterFlyState_Attack::OnExited(void)
{
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBolterFlyState_Attack::Free(void)
{
}
