#include "pch.h"
#include "BigGuyAState_Idle.h"
#include "BigGuyAState_Walk.h"
#include "BigGuyAState_Talk.h"
#include "StateMgr.h"
#include "BigGuyA.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Route.h"


CBigGuyAState_Idle::CBigGuyAState_Idle(CBigGuyA & _rOwner)
	:
	CState(_rOwner)
{
}

CBigGuyAState_Idle::~CBigGuyAState_Idle()
{
}

void CBigGuyAState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Idle_a);
	m_fKeepTime = Engine::GetNumberBetweenMinMax(2.f, 5.f);
	m_rOwner.GetPhysics()->SetSpeed(0.f);

	if (m_rOwner.GetRoute()->IsNextPositionExisted())
		m_bIsNextGo = true;
	else
		m_bIsNextGo = false;
}

int CBigGuyAState_Idle::Update(const _float& _fDeltaTime)
{
	// 대화 상태라면,,
	if (m_rOwner.IsTalking()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyAState_Talk(m_rOwner));
		return 0;
	}

	if ((m_fKeepTime -= _fDeltaTime) <= 0.f) {
		m_rOwner.GetStateMgr()->SetNextState(new CBigGuyAState_Walk(m_rOwner));
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

void CBigGuyAState_Idle::OnExited(void)
{
}

void CBigGuyAState_Idle::Free(void)
{
}
