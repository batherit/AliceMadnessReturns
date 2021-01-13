#include "pch.h"
#include "BigGuyAState_Idle.h"
#include "BigGuyAState_Talk.h"
#include "BigGuyAState_Walk.h"
#include "StateMgr.h"
#include "BigGuyA.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Route.h"

CBigGuyAState_Talk::CBigGuyAState_Talk(CBigGuyA & _rOwner)
	:
	CState(_rOwner)
{
}

CBigGuyAState_Talk::~CBigGuyAState_Talk()
{
}

void CBigGuyAState_Talk::OnLoaded(void)
{
	m_fKeepTime = 0.f;
	m_wstrTalkText[0] = L"Oh버워치 하고 싶다..";
	m_wstrTalkText[1] = L"집에 가서 놀고 싶구만 이거,,,";
	m_wstrTalkText[2] = L"내겐 월요병같은 건 없다구~";

	if (m_rOwner.GetRoute()->IsNextPositionExisted())
		m_bIsNextGo = true;
	else
		m_bIsNextGo = false;

	_vec3 vPlayerPos = (*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin())->GetTransform()->GetPos() 
		- m_rOwner.GetTransform()->GetPos();
	D3DXVec3Normalize(&vPlayerPos, &vPlayerPos);

	_vec3 vRotAxis = Engine::GetRotationAxis(m_rOwner.GetTransform()->GetLook(), vPlayerPos);
	_float vRotAngle = Engine::GetRotationAngle(m_rOwner.GetTransform()->GetLook(), vPlayerPos);
	m_rOwner.GetTransform()->RotateByAxis(vRotAngle, vRotAxis);
}

int CBigGuyAState_Talk::Update(const _float& _fDeltaTime)
{
	m_fElapsedTime += _fDeltaTime;

	if (!m_rOwner.IsTalking()) {
		if (m_fElapsedTime >= 5.f) {
			// 이야기를 오래했기에 그냥 바로 걷는 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CBigGuyAState_Walk(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CBigGuyAState_Idle(m_rOwner));
		}
		return 0;
	}


	if (m_rOwner.IsTalking()) {
		//TODO : 몇 초 간격으로 이야기를 진행한다.
		if ((m_fKeepTime -= _fDeltaTime) <= 0.f) {
			if (Engine::GetRandomBoolean())
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Talk_a);
			else
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Talk_b);
			m_rOwner.ShoutOut(WINCX / 6.f, WINCY - WINCY / 6.f, m_wstrTalkText[rand() % 3]);
			m_fKeepTime = Engine::GetNumberBetweenMinMax(5.f, 8.f);
		}
	}

	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
		if (Engine::GetRandomBoolean())
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Idle_a);
		else
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::BigGuy_LSP_Idle_b);
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

void CBigGuyAState_Talk::OnExited(void)
{
}

void CBigGuyAState_Talk::Free(void)
{
}
