#include "pch.h"
#include "GiantAliceWState_Attack.h"
#include "GiantAliceWState_Walk.h"
#include "GiantAliceWState_Idle.h"
#include "StateMgr.h"
#include "GiantAliceW.h"
#include "Map.h"
#include "Attribute.h"


CGiantAliceWState_Attack::CGiantAliceWState_Attack(CGiantAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CGiantAliceWState_Attack::~CGiantAliceWState_Attack()
{
}

void CGiantAliceWState_Attack::OnLoaded(void)
{
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceGiant_Attack03);
}

int CGiantAliceWState_Attack::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.35f) {
		if(!m_rOwner.GetAttackCollider()->IsActivated())
			m_rOwner.GetAttackCollider()->SetActivated(true);
	}

	if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Idle(m_rOwner));
	}


	return 0;
}

void CGiantAliceWState_Attack::OnExited(void)
{
	//m_pWeaponCollider->SetActivated(false);
	//m_rOwner.GetAttackCollider()->SetActivated(false);
	m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CGiantAliceWState_Attack::Free(void)
{
}
