#include "pch.h"
#include "BossState_Run.h"
#include "BossState_Idle.h"
#include "BossState_Death.h"
#include "BossState_Attack_SideCut.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"


CBossState_Run::CBossState_Run(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Run::~CBossState_Run()
{
}

void CBossState_Run::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Run_01);
	m_rOwner.GetPhysics()->SetSpeed(BOSS_RUN_SPEED);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBossState_Run::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}


	// 플레이어를 향해 달린다.
	_vec3 vToPlayer = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fToPlayerLength = D3DXVec3Length(&vToPlayer);

	if (fToPlayerLength <= BOSS_ATTACK_RANGE) {
		// 공격 범위에 들어왔다면 공격 시도 (지금은 Idle)
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_SideCut(m_rOwner));
		return 0;
	}

	// xz에 투영된 방향 구해서 속도 적용하기.
	vToPlayer.y = 0.f;
	fToPlayerLength = D3DXVec3Length(&vToPlayer);

	if (fToPlayerLength <= 0.f)
		return 0;

	vToPlayer /= fToPlayerLength;	// normalize

	// 플레이어를 향해 회전시킨다.
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	_vec3 vToTargetDirXZ = _vec3(vToPlayer.x, 0.f, vToPlayer.z);
	_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
	_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
	m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.6f, vRotAxis);

	// 플레이어를 향해 뛴다.
	m_fAccTime -= _fDeltaTime;
	if (m_fAccTime < 0.f)
		m_fAccTime = 0.f;

	m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() 
		* BOSS_RUN_SPEED * Engine::GetValueByWeight(Engine::GetWeightByValue(ACC_TIME - m_fAccTime, 0.f, ACC_TIME), 0.1, 1.f));

	return 0;
}

void CBossState_Run::OnExited(void)
{
}

void CBossState_Run::Free(void)
{
}
