#include "pch.h"
#include "AliceWState_Death.h"
#include "AliceWState_Idle.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Attribute.h"
#include "UI_InGame.h"
#include "UI_FadeInOut.h"
#include "Map.h"
#include "Butterfly.h"

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

		CButterfly* pButterfly = nullptr;
		Engine::CLayer* pLayer = Engine::GetLayer(L"Environment");
		for (_int i = 0; i <40; ++i) {
			pButterfly = CButterfly::Create(m_rOwner.GetGraphicDev());
			pButterfly->SetButterflyInfo(m_rOwner.GetTransform()->GetPos() + _vec3(Engine::GetNumberBetweenMinMax(-0.25f, 0.25f), 1.f + Engine::GetNumberBetweenMinMax(-0.25f, 0.25f), Engine::GetNumberBetweenMinMax(-0.25f, 0.25f)),
				Engine::GetRandomVector(), Engine::GetNumberBetweenMinMax(3.5f,4.0f ), Engine::GetNumberBetweenMinMax(3.f, 4.f), Engine::GetNumberBetweenMinMax(2.f, 2.1f), Engine::GetNumberBetweenMinMax(1.f, 3.f));
			pLayer->Add_GameObject(L"Effect", pButterfly);
		}
	}
	else {
		// 페이드인아웃 실행
		if (!m_pInGameUI->GetFadeInOut()->IsFadeIn() && !m_pInGameUI->GetFadeInOut()->IsProcessing()) {
			if ((m_fElapsedTime += _fDeltaTime) >= 2.f) {
				// 2초후 리스폰
				m_pInGameUI->GetFadeInOut()->StartFadeInOut(2.f, true);
				m_rOwner.GetAttribute()->SetHP(m_rOwner.GetAttribute()->GetMaxHP());
				m_rOwner.GetTransform()->SetPos(dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin())->GetCurSpawnPoint());
				m_rOwner.SetLanded(false);
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
	m_rOwner.GetAttribute()->SetDamaged(false);
}

void CAliceWState_Death::Free(void)
{
}
