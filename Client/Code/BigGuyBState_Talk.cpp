#include "pch.h"
#include "BigGuyBState_Idle.h"
#include "BigGuyBState_Talk.h"
#include "BigGuyBState_Walk.h"
#include "StateMgr.h"
#include "BigGuyB.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Route.h"

CBigGuyBState_Talk::CBigGuyBState_Talk(CBigGuyB & _rOwner)
	:
	CState(_rOwner)
{
}

CBigGuyBState_Talk::~CBigGuyBState_Talk()
{
}

void CBigGuyBState_Talk::OnLoaded(void)
{
	m_fKeepTime = 0.f;
	m_wstrTalkText[0] = L"아 서둘러야겠어, 놀아야되거든,,,";
	m_wstrTalkText[1] = L"아무 생각이 없다,,,";
	m_wstrTalkText[2] = L"놀고 싶다~";

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

int CBigGuyBState_Talk::Update(const _float& _fDeltaTime)
{
	m_fElapsedTime += _fDeltaTime;

	if (!m_rOwner.IsTalking()) {
		if (m_fElapsedTime >= 5.f) {
			// 이야기를 오래했기에 그냥 바로 걷는 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Walk(m_rOwner));
		}
		else {
			m_rOwner.GetStateMgr()->SetNextState(new CBigGuyBState_Idle(m_rOwner));
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
			m_rOwner.ShoutOut(WINCX / 6.f, WINCY - WINCY / 8.f, m_wstrTalkText[rand() % 3]);
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

void CBigGuyBState_Talk::OnExited(void)
{
}

void CBigGuyBState_Talk::Free(void)
{
}
