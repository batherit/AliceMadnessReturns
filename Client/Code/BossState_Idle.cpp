#include "pch.h"
#include "BossState_Idle.h"
#include "BossState_Death.h"
#include "BossState_Walk.h"
#include "BossState_Run.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "BossState_Attack_JumpDown.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"


CBossState_Idle::CBossState_Idle(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Idle::~CBossState_Idle()
{
}

void CBossState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	m_fQuietTime = Engine::GetNumberBetweenMinMax(1.f, 2.f);

	if (D3DXVec3Length(&(m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos())) > BOSS_ATTACK_RANGE * 1.2f) {
		m_fQuietTime = 1.f;
	}

	
}

int CBossState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	//if (m_rOwner.GetAttribute()->IsDamaged()) {
	//	m_rOwner.GetStateMgr()->SetNextState(new CBossState_Damage(m_rOwner));
	//	return 0;
	//}

	/*_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);

	if (fLength <= BOSS_COGNITIVE_RANGE) {
		if ((m_fQuietTime -= _fDeltaTime) <= 0.f) {
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Walk(m_rOwner));
			return 0;
		}

		_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
		D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
		_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
		_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
		m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
	}*/

	return 0;
}

void CBossState_Idle::OnExited(void)
{
}

void CBossState_Idle::Free(void)
{
}
