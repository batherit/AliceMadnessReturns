#include "pch.h"
#include "BigGuyBState_Idle.h"
#include "BigGuyBState_Walk.h"
#include "BigGuyBState_Talk.h"
#include "StateMgr.h"
#include "BigGuyB.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Route.h"


CBigGuyBState_Idle::CBigGuyBState_Idle(CBigGuyB & _rOwner)
	:
	CState(_rOwner)
{
}

CBigGuyBState_Idle::~CBigGuyBState_Idle()
{
}

void CBigGuyBState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Idle_a);
	m_fKeepTime = Engine::GetNumberBetweenMinMax(2.f, 5.f);
	m_rOwner.GetPhysics()->SetSpeed(0.f);

	if (m_rOwner.GetRoute()->IsNextPositionExisted())
		m_bIsNextGo = true;
	else
		m_bIsNextGo = false;
}

int CBigGuyBState_Idle::Update(const _float& _fDeltaTime)
{
	// 대화 상태라면,,
	if (m_rOwner.IsTalking()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Talk(m_rOwner));
		return 0;
	}

	if ((m_fKeepTime -= _fDeltaTime) <= 0.f) {
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Walk(m_rOwner));
		return 0;
	}

	// Route 갱신
	CRoute* pRoute = m_rOwner.GetRoute();
	if (m_bIsNextGo) {
		if (pRoute->IsNextPositionExisted())
			pRoute->GetNextPosition(_fDeltaTime, ALICE_RUN_SPEED);
		else
			m_bIsNextGo = false;
	}
	else {
		if (pRoute->IsPrevPositionExisted())
			pRoute->GetPrevPosition(_fDeltaTime, ALICE_RUN_SPEED);
		else
			m_bIsNextGo = true;
	}

	return 0;
}

void CBigGuyBState_Idle::OnExited(void)
{
}

void CBigGuyBState_Idle::Free(void)
{
}
