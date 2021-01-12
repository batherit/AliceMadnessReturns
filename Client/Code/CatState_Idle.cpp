#include "pch.h"
#include "CatState_Idle.h"
#include "CatState_Run.h"
#include "StateMgr.h"
#include "Cat.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"


CCatState_Idle::CCatState_Idle(CCat & _rOwner)
	:
	CState(_rOwner)
{
}

CCatState_Idle::~CCatState_Idle()
{
}

void CCatState_Idle::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Cat_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CCatState_Idle::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsGoOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CCatState_Run(m_rOwner));
	}

	return 0;
}

void CCatState_Idle::OnExited(void)
{
}

void CCatState_Idle::Free(void)
{
}
