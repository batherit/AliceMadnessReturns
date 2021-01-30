#include "pch.h"
#include "BossState_Death.h"
#include "StateMgr.h"
#include "Boss.h"
#include "UI_HPBar.h"
#include "Cake.h"
#include "StaticObject.h"


CBossState_Death::CBossState_Death(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Death::~CBossState_Death()
{
}

void CBossState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Giant_Laugh03);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_rOwner.GetHPBarUI()->Off();
	m_rOwner.GetAttackCollider()->SetActivated(false);
	m_rOwner.GetAttackerObject()->SetActivated(false);
	Engine::GetLayer(L"Environment")->Add_GameObject(CCake::Create(m_rOwner.GetGraphicDev()));
}

int CBossState_Death::Update(const _float& _fDeltaTime)
{
	/*if (!m_rOwner.GetDynamicMesh()->IsAnimStopping() && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.8f) {
		m_rOwner.GetDynamicMesh()->SetAnimationStop(true);
	}*/

	return 0;
}

void CBossState_Death::OnExited(void)
{
}

void CBossState_Death::Free(void)
{
}
