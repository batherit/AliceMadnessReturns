#include "pch.h"
#include "MiniGruntState_Damage.h"
#include "MiniGruntState_Death.h"
#include "MiniGruntState_Alert.h"
#include "MiniGruntState_Idle.h"
//#include "MiniGruntState_Run.h"
//#include "MiniGruntState_Attack.h"
//#include "MiniGruntState_Jump.h"
//#include "MiniGruntState_Death.h"
#include "StateMgr.h"
#include "MiniGrunt.h"
#include "Map.h"
#include "Attribute.h"


CMiniGruntState_Damage::CMiniGruntState_Damage(CMiniGrunt & _rOwner, _bool _bIsBigAttack)
	:
	CState(_rOwner),
	m_bIsBigAttack(_bIsBigAttack)
{
}

CMiniGruntState_Damage::~CMiniGruntState_Damage()
{
}

void CMiniGruntState_Damage::OnLoaded(void)
{
	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	_vec3 vToOwner = m_rOwner.GetTransform()->GetPos() - m_rOwner.GetTargetObject()->GetTransform()->GetPos();
	vToOwner.y = 0.f;
	D3DXVec3Normalize(&vToOwner, &vToOwner);

	if (m_bIsBigAttack) {
		_int _iIndex = rand() % 3;
		switch (rand() % 3) {
		case 0:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Mini_Damage_L);
			break;
		case 1:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Mini_Damage_L_2);
			break;
		case 2:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Mini_Damage_L_3);
			break;
		}

		m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * MINIGRUNT_RUN_SPEED * 2.f);
		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);
	}
	else {
		if (Engine::GetRandomBoolean()) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Damage_H);
		}
		else {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_Damage_H_2);
		}
		m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(vToOwner.x, vToOwner.z) * MINIGRUNT_RUN_SPEED * 3.f);
		m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.8f);
	}

	m_rOwner.GetAttribute()->SetDamaged(false);
	PlayDamagedSound();
}

int CMiniGruntState_Damage::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Damage(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Idle(m_rOwner));
	}

	return 0;
}

void CMiniGruntState_Damage::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CMiniGruntState_Damage::Free(void)
{
}

void CMiniGruntState_Damage::PlayDamagedSound()
{
	switch (Engine::GetNumberBetweenMinMax(0, 2)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Damaged0.ogg", CSoundMgr::MONSTER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Damaged1.ogg", CSoundMgr::MONSTER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Damaged2.ogg", CSoundMgr::MONSTER);
		break;
	}
}
