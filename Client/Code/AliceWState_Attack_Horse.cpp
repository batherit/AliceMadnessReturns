#include "pch.h"
#include "AliceWState_Attack_Horse.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Death.h"
#include "AliceWState_Damage.h"
#include "AliceWState_Dash.h"
#include "AliceWState_SuperJump.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_Attack_Horse::CAliceWState_Attack_Horse(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Attack_Horse::~CAliceWState_Attack_Horse()
{
}

void CAliceWState_Attack_Horse::OnLoaded(void)
{
	m_bIsAttacking = true;
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_1_A);
	m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 2.f);
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.85f);
	++m_iAttackNum;

	//m_pWeaponCollider = m_rOwner.GetWeapon()->GetColliderFromTag(L"PlayerAttack");
	//m_pWeaponCollider->SetActivated(true);
	m_rOwner.GetAttackCollider()->SetActivated(false);
	m_bIsSwing = false;
	//CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing0.ogg", CSoundMgr::PLAYER);
}

int CAliceWState_Attack_Horse::Update(const _float& _fDeltaTime)
{
	// Attack_Horse => Death, Jump, Idle, Attack_Horse
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsSuperJumpOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_SuperJump(m_rOwner));
		m_rOwner.SetSuperJumped(false);
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Damage(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsDashOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, m_rOwner.GetTransform()->GetLook()));
		return 0;
	}

	_vec3 vDir;
	if (m_bIsAttacking) {
		// ���� ����� ����ǰ� �ִ� ���¿���,,,
		if (m_iAttackNum == 2) {
			_float fProgress = m_rOwner.GetDynamicMesh()->GetAnimationProgress();
			if (fProgress <= 0.2f) {
				if (!m_bIsSwing) {
					m_rOwner.GetAttackCollider()->SetActivated(true);
					PlaySwingSound();
					m_bIsSwing = true;
				}
			}
			else if (fProgress <= 0.4f) {
				if (m_bIsSwing) {
					m_rOwner.GetAttackCollider()->SetActivated(false);
					m_bIsSwing = false;
				}
			}
			else if (fProgress <= 0.8f) {
				if (!m_bIsSwing) {
					m_rOwner.GetAttackCollider()->SetActivated(true);
					PlaySwingSound();
					m_bIsSwing = true;
				}
			}
			else {
				if (m_bIsSwing) {
					m_rOwner.GetAttackCollider()->SetActivated(false);
					m_bIsSwing = false;
				}
			}
		}
		else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= m_fEndTime[m_iAttackNum]) {
			if (m_bIsSwing) {
				m_rOwner.GetAttackCollider()->SetActivated(false);
				m_bIsSwing = false;
			}
		}
		else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= m_fStartTime[m_iAttackNum]) {
			if (!m_bIsSwing) {
				m_rOwner.GetAttackCollider()->SetActivated(true);
				PlaySwingSound();
				m_bIsSwing = true;
			}
		}
		

		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// ������ ����Ǿ�����, ���� ��� �ִϸ��̼��� ������.
			switch (m_iAttackNum)
			{
			case 1:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_1_B);
				break;
			case 2:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_2_B);
				break;
			case 3:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_3_B);
				break;
			case 4:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_4_B);
				break;
			}
			m_bIsAttacking = false;
			if (m_bIsSwing) {
				m_rOwner.GetAttackCollider()->SetActivated(false);
				m_bIsSwing = false;
			}
		}
		else if (m_iAttackNum < 4 && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= m_fEntryTime[m_iAttackNum]) {
			// �Է� ���� ����(���� �ִϸ��̼��� 70�ۼ�Ʈ ���������)�� ������,
			// ���ݿ� ���� �Է��� ���ǰ� ���ο� ���� �Է��� �������� ���� ������ �����Ѵ�.
			if (m_rOwner.IsAttackOn(_fDeltaTime)) {
				switch (m_iAttackNum)
				{
				case 1:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_2_A);
					m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 1.f);
					m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
					//CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing0.ogg", CSoundMgr::PLAYER);
					break;
				case 2:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_3_A);
					m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 1.f);
					m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
					//CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing1.ogg", CSoundMgr::PLAYER);
					break;
				case 3:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP2_Mele_Attack_4_A);
					m_rOwner.GetPhysics()->SetVelocity(m_rOwner.GetTransform()->GetLook() * ALICE_RUN_SPEED * 1.f);
					m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.9f);
					//CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing2.ogg", CSoundMgr::PLAYER);
					break;
				}
				++m_iAttackNum;
				if (m_bIsSwing) {
					m_rOwner.GetAttackCollider()->SetActivated(false);
					m_bIsSwing = false;
				}
			}
			else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.6f && m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
				_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
				m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
			}
			// else � �Էµ� ���� ���ߴٸ� �ƹ��͵� ���� �ʴ´�.
		}
	}
	else if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		// ���� ���� ����� ������ ���� �����ӿ� ���� ���¸� �����Ѵ�.
		/*if (m_rOwner.IsAttackOn(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Attack_Horse(m_rOwner));
		}
		else */
		if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
		}
	}

	return 0;
}

void CAliceWState_Attack_Horse::OnExited(void)
{
	//m_pWeaponCollider->SetActivated(false);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CAliceWState_Attack_Horse::Free(void)
{
}

void CAliceWState_Attack_Horse::PlaySwingSound()
{
	switch (Engine::GetNumberBetweenMinMax(0, 2)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing0.ogg", CSoundMgr::PLAYER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing1.ogg", CSoundMgr::PLAYER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Swing2.ogg", CSoundMgr::PLAYER);
		break;
	}
}
