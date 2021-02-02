#include "pch.h"
#include "MiniGruntState_Death.h"
#include "StateMgr.h"
#include "MiniGrunt.h"


CMiniGruntState_Death::CMiniGruntState_Death(CMiniGrunt & _rOwner)
	:
	CState(_rOwner)
{
}

CMiniGruntState_Death::~CMiniGruntState_Death()
{
}

void CMiniGruntState_Death::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Grunt_Min_die_3);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	PlayDieSound();
}

int CMiniGruntState_Death::Update(const _float& _fDeltaTime)
{
	if (!m_rOwner.GetDynamicMesh()->IsAnimStopping() && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.8f) {
		m_rOwner.GetDynamicMesh()->SetAnimationStop(true);
	}
	else if ((m_fElapsedTime += _fDeltaTime) >= 3.f) {
		m_rOwner.SetValid(false);
	}

	return 0;
}

void CMiniGruntState_Death::OnExited(void)
{
}

void CMiniGruntState_Death::Free(void)
{
}

void CMiniGruntState_Death::PlayDieSound()
{
	switch (Engine::GetNumberBetweenMinMax(0, 4)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Die0.ogg", CSoundMgr::MONSTER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Die1.ogg", CSoundMgr::MONSTER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Die2.ogg", CSoundMgr::MONSTER);
		break;
	case 3:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Die3.ogg", CSoundMgr::MONSTER);
		break;
	case 4:
		CSoundMgr::Get_Instance()->PlaySound(L"MiniGrunt_Die4.ogg", CSoundMgr::MONSTER);
		break;
	}
}
