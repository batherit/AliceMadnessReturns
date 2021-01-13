#include "pch.h"
#include "MiniGruntState_Idle.h"
#include "MiniGruntState_Death.h"
#include "MiniGruntState_Alert.h"
#include "MiniGruntState_Damage.h"
#include "MiniGruntState_Attack.h"
#include "StateMgr.h"
#include "MiniGrunt.h"
#include "Map.h"
#include "Attribute.h"


CMiniGruntState_Idle::CMiniGruntState_Idle(CMiniGrunt & _rOwner)
	:
	CState(_rOwner)
{
}

CMiniGruntState_Idle::~CMiniGruntState_Idle()
{
}

void CMiniGruntState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_PreSpawn);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_bIsAttackIdle = false;

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CMiniGruntState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);

	if (m_bIsAttackIdle) {
		if (fLength > MINIGRUNT_COGNITIVE_RANGE) {
			// ���� ���� �ۿ� �ִٸ� �Ϲ� Idle���¸� �����մϴ�.
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_PreSpawn);
			m_bIsAttackIdle = false;
			m_fElapsedTime = 0.f;
		}
		else if ((m_fElapsedTime += _fDeltaTime) >= 2.f) {
			// TODO : ���� �� 1�ʰ� �����ٸ� ��� �¼��� ��ȯ�մϴ�.
			m_rOwner.GetStateMgr()->SetNextState(new CMiniGruntState_Alert(m_rOwner));
		}
		else {
			// Ÿ�� ��ġ�� ���� ȸ���մϴ�.
			_vec3 vLook = m_rOwner.GetTransform()->GetLook();
			_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
			D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);
			_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
			_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
			m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
		}
	}
	else {
		if (fLength <= MINIGRUNT_COGNITIVE_RANGE) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_PreSpawn);
			m_bIsAttackIdle = true;
			m_fElapsedTime = 0.f;
		}
		// ���� ���� �ۿ� �ִٸ� �ƹ��͵� ���� �ʴ´�.
	}

	return 0;
}

void CMiniGruntState_Idle::OnExited(void)
{
}

void CMiniGruntState_Idle::Free(void)
{
}
