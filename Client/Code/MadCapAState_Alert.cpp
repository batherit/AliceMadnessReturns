#include "pch.h"
#include "MadCapAState_Alert.h"
#include "StateMgr.h"
#include "MadCapA.h"


CMadCapAState_Alert::CMadCapAState_Alert(CMadCapA & _rOwner)
	:
	CState(_rOwner)
{
}

CMadCapAState_Alert::~CMadCapAState_Alert()
{
}

void CMadCapAState_Alert::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::MadCap_Idle);
	m_rOwner.GetPhysics()->SetSpeed(0.f);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CMadCapAState_Alert::Update(const _float& _fDeltaTime)
{

	return 0;
}

void CMadCapAState_Alert::OnExited(void)
{
}

void CMadCapAState_Alert::Free(void)
{
}
