#include "pch.h"
#include "BolterFlyState_Damage.h"
#include "BolterFlyState_Death.h"
#include "BolterFlyState_Alert.h"
#include "BolterFlyState_Idle.h"
//#include "BolterFlyState_Run.h"
//#include "BolterFlyState_Attack.h"
//#include "BolterFlyState_Jump.h"
//#include "BolterFlyState_Death.h"
#include "StateMgr.h"
#include "BolterFly.h"
#include "Map.h"
#include "Attribute.h"


CBolterFlyState_Damage::CBolterFlyState_Damage(CBolterFly & _rOwner, _bool _bIsBigAttack)
	:
	CState(_rOwner),
	m_bIsBigAttack(_bIsBigAttack)
{
}

CBolterFlyState_Damage::~CBolterFlyState_Damage()
{
}

void CBolterFlyState_Damage::OnLoaded(void)
{
	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	_vec3 vToOwner = m_rOwner.GetTransform()->GetPos() - m_rOwner.GetTargetObject()->GetTransform()->GetPos();
	vToOwner.y = 0.f;
	D3DXVec3Normalize(&vToOwner, &vToOwner);

	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Bolterfly_Idle_Damaged);

	m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * BOLTERFLY_RUN_SPEED * 2.f);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);

	m_rOwner.GetAttribute()->SetDamaged(false);
}

int CBolterFlyState_Damage::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Damage(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBolterFlyState_Idle(m_rOwner));
	}

	return 0;
}

void CBolterFlyState_Damage::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CBolterFlyState_Damage::Free(void)
{
}
