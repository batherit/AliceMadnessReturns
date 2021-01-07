#include "pch.h"
#include "AliceWState_Death.h"
#include "AliceWState_Idle.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Attribute.h"
#include "UI_InGame.h"
#include "UI_FadeInOut.h"
#include "Map.h"

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
	m_rOwner.SetSuperJumped(false);
	m_pInGameUI = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
	
}

int CAliceWState_Death::Update(const _float& _fDeltaTime)
{
	m_rOwner.GetPhysics()->SetSpeed(0.f);

	if (m_rOwner.IsVisible() && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.9f) {
		//m_rOwner.SetActivated(false);
		m_rOwner.SetVisible(false);
		m_pInGameUI->GetFadeInOut()->StartFadeInOut(2.f, false);
	}
	else {
		// 페이드인아웃 실행
		if (!m_pInGameUI->GetFadeInOut()->IsFadeIn() && m_pInGameUI->GetFadeInOut()->GetProgress() >= 0.95f) {
			if ((m_fElapsedTime += _fDeltaTime) >= 2.f) {
				// 2초후 리스폰
				m_pInGameUI->GetFadeInOut()->StartFadeInOut(2.f, true);
				m_rOwner.GetAttribute()->SetHP(m_rOwner.GetAttribute()->GetMaxHP());
				m_rOwner.GetTransform()->SetPos(dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin())->GetCurSpawnPoint());
				//m_rOwner.SetActivated(true);
				m_rOwner.SetVisible(true);
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
			}
		}
	}

	return 0;
}

void CAliceWState_Death::OnExited(void)
{
}

void CAliceWState_Death::Free(void)
{
}
