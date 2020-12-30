#include "pch.h"
#include "MadCapAState_Death.h"
#include "StateMgr.h"
#include "MadCapA.h"


CMadCapAState_Death::CMadCapAState_Death(CMadCapA & _rOwner)
	:
	CState(_rOwner)
{
}

CMadCapAState_Death::~CMadCapAState_Death()
{
}

void CMadCapAState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Attack_Death01);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CMadCapAState_Death::Update(const _float& _fDeltaTime)
{
	if (!m_rOwner.GetDynamicMesh()->IsAnimStopping() && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.8f) {
		m_rOwner.GetDynamicMesh()->SetAnimationStop(true);
	}
	else if ((m_fElapsedTime += _fDeltaTime) >= 3.f) {
		m_rOwner.SetValid(false);
	}

	return 0;
}

void CMadCapAState_Death::OnExited(void)
{
}

void CMadCapAState_Death::Free(void)
{
}
