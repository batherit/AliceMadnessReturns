#include "pch.h"
#include "MadCapAState_Damage.h"
#include "MadCapAState_Death.h"
#include "MadCapAState_Alert.h"
#include "MadCapAState_Idle.h"
//#include "MadCapAState_Run.h"
//#include "MadCapAState_Attack.h"
//#include "MadCapAState_Jump.h"
//#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "MadCapA.h"
#include "Map.h"


CMadCapAState_Damage::CMadCapAState_Damage(CMadCapA & _rOwner, _bool _bIsBigAttack)
	:
	CState(_rOwner),
	m_bIsBigAttack(_bIsBigAttack)
{
}

CMadCapAState_Damage::~CMadCapAState_Damage()
{
}

void CMadCapAState_Damage::OnLoaded(void)
{
	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	_vec3 vToOwner = m_rOwner.GetTransform()->GetPos() - m_rOwner.GetTargetObject()->GetTransform()->GetPos();
	vToOwner.y = 0.f;
	D3DXVec3Normalize(&vToOwner, &vToOwner);

	if (m_bIsBigAttack) {
		if (Engine::GetRandomBoolean()) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Damaged_Light01);
		}
		else {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Damaged_Light02);
		}
		//m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * MADCAPA_RUN_SPEED * 2.f);
		//m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);
	}
	else {
		if (Engine::GetRandomBoolean()) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Damaged_Heave01);
		}
		else {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Damaged_Heave02);
		}
		//m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * MADCAPA_RUN_SPEED * 3.f);
		//m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);
	}
}

int CMadCapAState_Damage::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Idle(m_rOwner));
	}

	return 0;
}

void CMadCapAState_Damage::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CMadCapAState_Damage::Free(void)
{
}
