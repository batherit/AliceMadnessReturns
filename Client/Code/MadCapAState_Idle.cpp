#include "pch.h"
#include "MadCapAState_Idle.h"
#include "MadCapAState_Death.h"
#include "MadCapAState_Alert.h"
#include "MadCapAState_Damage.h"
//#include "MadCapAState_Run.h"
//#include "MadCapAState_Attack.h"
//#include "MadCapAState_Jump.h"
//#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "MadCapA.h"
#include "Map.h"
#include "Attribute.h"


CMadCapAState_Idle::CMadCapAState_Idle(CMadCapA & _rOwner)
	:
	CState(_rOwner)
{
}

CMadCapAState_Idle::~CMadCapAState_Idle()
{
}

void CMadCapAState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_bIsAttackIdle = false;
	
	if(!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CMadCapAState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
	_float fLength = D3DXVec3Length(&vToTargetDir);
	
	if (m_bIsAttackIdle) {
		if (fLength > MADCAPA_COGNITIVE_RANGE) {
			// ���� ���� �ۿ� �ִٸ� �Ϲ� Idle���¸� �����մϴ�.
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Idle);
			m_bIsAttackIdle = false;
			m_fElapsedTime = 0.f;
		}
		else if((m_fElapsedTime += _fDeltaTime) >= 2.f){
			// TODO : ���� �� 1�ʰ� �����ٸ� ��� �¼��� ��ȯ�մϴ�.
			m_rOwner.GetStateMgr()->SetNextState(new CMadCapAState_Alert(m_rOwner));
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
		if (fLength <= MADCAPA_COGNITIVE_RANGE) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Attack_Idle);
			m_bIsAttackIdle = true;
			m_fElapsedTime = 0.f;
		}
		// ���� ���� �ۿ� �ִٸ� �ƹ��͵� ���� �ʴ´�.
	}

	return 0;
}

void CMadCapAState_Idle::OnExited(void)
{
}

void CMadCapAState_Idle::Free(void)
{
}
