#include "pch.h"
#include "AliceWState_JumpDamage.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Death.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Dash.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"


CAliceWState_JumpDamage::CAliceWState_JumpDamage(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_JumpDamage::~CAliceWState_JumpDamage()
{
}

void CAliceWState_JumpDamage::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_Float_Reaction_FW);
	m_rOwner.SetLanded(false);
	m_pNaviMesh = m_rOwner.GetMap()->GetNaviMesh();
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.95f);
	m_rOwner.GetAttribute()->SetDamaged(false);

	switch (Engine::GetNumberBetweenMinMax(0, 3)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"Alice_Damaged0.ogg", CSoundMgr::PLAYER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"Alice_Damaged1.ogg", CSoundMgr::PLAYER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"Alice_Damaged2.ogg", CSoundMgr::PLAYER);
		break;
	case 3:
		CSoundMgr::Get_Instance()->PlaySound(L"Alice_Damaged3.ogg", CSoundMgr::PLAYER);
		break;
	}
}

int CAliceWState_JumpDamage::Update(const _float& _fDeltaTime)
{
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
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_JumpDamage(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsDashOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, m_rOwner.GetTransform()->GetLook()));
		return 0;
	}

	if (!m_bIsAnimChanging && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_Float_Idle);
		m_bIsAnimChanging = true;
	}

	_vec3 vDir;
	if (!m_rOwner.IsLanded()) {
		// TODO : 바닥에 닿았는지 여부를 따져야합니다. => AliceW 내부에서 합니다.
		if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED * 0.8f);
		}
	}
	else {
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_LandLow);
		m_bIsAnimChanging = true;
		m_eJumpStep = STEP_LAND;
	}

	// JumpDamage => Death, JumpDamage, Run, Attack
	switch (m_eJumpStep) {
	case STEP_FALL:
		break;
	case STEP_LAND:
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			m_rOwner.GetPhysics()->SetGravity(9.8f * 3.f);
			if (m_rOwner.IsSliding(_fDeltaTime)) {
				if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
					_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
					m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
				}
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Slide(m_rOwner));
			}
			else if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
				_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
				m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
			}
		}
		break;
	}

	return 0;
}

void CAliceWState_JumpDamage::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CAliceWState_JumpDamage::Free(void)
{
}
