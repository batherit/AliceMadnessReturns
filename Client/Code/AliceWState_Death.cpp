#include "pch.h"
#include "AliceWState_Death.h"
#include "AliceWState_Run.h"
#include "AliceWState_Jump.h"
#include "StateMgr.h"
#include "AliceW.h"


CAliceWState_Death::CAliceWState_Death(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Death::~CAliceWState_Death()
{
}

void CAliceWState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Alice_Death);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CAliceWState_Death::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.99f) {
		m_rOwner.SetActivated(false);
	}

	return 0;
}

void CAliceWState_Death::OnExited(void)
{
}

void CAliceWState_Death::Free(void)
{
}
