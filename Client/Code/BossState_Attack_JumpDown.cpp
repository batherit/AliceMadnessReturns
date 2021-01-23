#include "pch.h"
#include "BossState_Attack_JumpDown.h"
#include "BossState_Idle.h"
#include "BossState_Run.h"
#include "BossState_Death.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"
//#include "CameraMgr.h"


CBossState_Attack_JumpDown::CBossState_Attack_JumpDown(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Attack_JumpDown::~CBossState_Attack_JumpDown()
{
}

void CBossState_Attack_JumpDown::OnLoaded(void)
{
	m_bIsJumpStarting = true;
	m_bIsTargeting = false;
	m_bIsBack = false;
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Jump_Start);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.96f);
	m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(0.f, 0.f));

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());

	m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());

	m_rOwner.GetAttackCollider()->SetActivated(true);
}

int CBossState_Attack_JumpDown::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	if (m_bIsJumpStarting) {
		if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
			// 1. ���� ���� �ִ��� �Ϸ�Ǹ� ������� ������� ��ȯ �� ������ ������ �Ѵ�.
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_AttackSlam);
			m_fHeight = m_rOwner.GetTransform()->GetPos().y;
			m_rOwner.GetPhysics()->SetGravity(9.8f * 2.f);
			m_rOwner.GetPhysics()->SetVelocityY(BOSS_JUMP_SPEED);
			m_rOwner.SetLanded(false);
			m_bIsJumpStarting = false;
		}
	}
	else {
		if (m_bIsTargeting) {
			if (m_bIsBack) {
				if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
					// 6. ���̵� ���·� ��ȯ�Ѵ�.
					m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
					return 0;
				}
			}
			else {
				//3. ������� ����� 0.6(������� ���� ����)������ ����� �����ϸ鼭 �����´�.
				_float fProgress = m_rOwner.GetDynamicMesh()->GetAnimationProgress();
				if (fProgress <= 0.6f) {
					_float fT = Engine::GetWeightByValue(fProgress, 0.5f, 0.6f);
					_vec3 vNewPos = m_vStartPos * (1.f - fT) + m_vTargetPos * fT;
					m_rOwner.GetTransform()->SetPos(m_pMap->GetNaviMesh()->MoveOnNaviMesh_Adhesion(m_rOwner.GetCellIndex(), &m_rOwner.GetTransform()->GetPos(), &vNewPos));
				}
				else {
					// 5. 1�ʰ� ������� ����� ������ ������� ���ư���.
					if ((m_fElapsedTime += _fDeltaTime) >= 0.6f) {
						m_bIsBack = true;
						m_rOwner.GetDynamicMesh()->SetAnimationStop(false);
					}
					else if(!m_bIsAttackEnd){
						// 4. 0.6�ʰ� ���������� ä�� �ִ´�.
						//m_rOwner.GetTransform()->SetPos(m_vTargetPos);
						//m_rOwner.GetPhysics()->SetVelocityY(0.f);
						//m_rOwner.GetPhysics()->SetGravity(9.8f * 2.f);
						m_rOwner.GetDynamicMesh()->SetAnimationStop(true);
						// TODO : ī�޶� ��� ����Ʈ
						Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
						pCameraMgr->GetCamera()->Shake(0.8f, 0.45f, 55);
						m_bIsAttackEnd = true;
					}
				}
			}
		}
		else if(m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.4f){
			//m_rOwner.GetPhysics()->SetGravity(0.f);
			//m_rOwner.GetPhysics()->SetVelocityY(0.f);
			// 2. ������� ����� 40% ���������, �������� ���� �غ� �۾��� �Ѵ�.
			m_vStartPos = m_rOwner.GetTransform()->GetPos();
			_vec3 vToTargetDir = m_rOwner.GetTargetObject()->GetTransform()->GetPos() - m_rOwner.GetTransform()->GetPos();
			_float fLength = D3DXVec3Length(&vToTargetDir);
			_vec3 vLook = m_rOwner.GetTransform()->GetLook();
			_vec3 vToTargetDirXZ = _vec3(vToTargetDir.x, 0.f, vToTargetDir.z);
			D3DXVec3Normalize(&vToTargetDirXZ, &vToTargetDirXZ);

			_vec3 vRotAxis = Engine::GetRotationAxis(vLook, vToTargetDirXZ);
			_float fRotAngle = Engine::GetRotationAngle(vLook, vToTargetDirXZ);
			m_rOwner.GetTransform()->RotateByAxis(fRotAngle * 0.1f, vRotAxis);
			m_vTargetPos = m_rOwner.GetTargetObject()->GetTransform()->GetPos();
			m_vTargetPos.y = m_fHeight;

		//	vSettedPos = m_pMap->GetNaviMesh()->MoveOnNaviMesh_Adhesion(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f))

			m_vTargetPos -= m_rOwner.GetTransform()->GetLook() * 4.f;
			m_bIsTargeting = true;
		}
	}

	return 0;
}

void CBossState_Attack_JumpDown::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBossState_Attack_JumpDown::Free(void)
{
}
