#include "pch.h"
#include "BossState_Attack_Mixer.h"
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


CBossState_Attack_Mixer::CBossState_Attack_Mixer(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Attack_Mixer::~CBossState_Attack_Mixer()
{
}

void CBossState_Attack_Mixer::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Run_Attack);
	m_rOwner.GetAttackCollider()->SetActivated(true);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBossState_Attack_Mixer::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	if ((m_fElapsedTime += _fDeltaTime) >= 3.f) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
		return 0;
	}
	else if(!m_bIsStopping){
		// 플레이어를 향해 회전시킨다.
		_vec3 vToPlayer = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
		_float fToPlayerLength = D3DXVec3Length(&vToPlayer);

		// xz에 투영된 방향 구해서 속도 적용하기.
		vToPlayer.y = 0.f;
		fToPlayerLength = D3DXVec3Length(&vToPlayer);

		if (fToPlayerLength <= 0.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			return 0;
		}

		vToPlayer /= fToPlayerLength;	// normalize

		_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		_vec3 vToTargetDirXZ = _vec3(vToPlayer.x, 0.f, vToPlayer.z);
		_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
		_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
		m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.8f, vRotAxis);

		vLook = m_rOwner.GetTransform()->GetLook();
		if (D3DXVec3Dot(&vLook, &vToPlayer) <= 0.f || fToPlayerLength < 1.f) {
			//m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			m_bIsStopping = true;
			m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
			return 0;
		}

		// 플레이어를 향해 점점 빠르게 다가온다.
		_float fT = Engine::GetWeightByValue(m_fElapsedTime, 0.f, 3.f);
		m_rOwner.GetPhysics()->SetVelocity(vLook * Engine::GetValueByWeight(fT, BOSS_RUN_SPEED, BOSS_RUN_SPEED * 2.f));
	}

	return 0;
}

void CBossState_Attack_Mixer::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBossState_Attack_Mixer::Free(void)
{
}
