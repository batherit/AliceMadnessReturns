#include "pch.h"
#include "GiantAliceWState_Scale.h"
#include "GiantAliceWState_Idle.h"
#include "StateMgr.h"
#include "GiantAliceW.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"


CGiantAliceWState_Scale::CGiantAliceWState_Scale(CGiantAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CGiantAliceWState_Scale::~CGiantAliceWState_Scale()
{
}

void CGiantAliceWState_Scale::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceGiant_Idle_Long01);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CGiantAliceWState_Scale::Update(const _float& _fDeltaTime)
{
	_float fProgress = m_rOwner.GetDynamicMesh()->GetAnimationProgress();
	if (fProgress >= 0.99f) {
		m_rOwner.GetTransform()->SetScaleXYZ(4.f, 4.f, 4.f);
		m_rOwner.GetStateMgr()->SetNextState(new CGiantAliceWState_Idle(m_rOwner));
		return 1;
	}
	else {
		_float fScale = Engine::GetValueByWeight(fProgress, 1.f, 4.f);
		m_rOwner.GetTransform()->SetScaleXYZ(fScale, fScale, fScale);
	}


	return 0;
}

void CGiantAliceWState_Scale::OnExited(void)
{
}

void CGiantAliceWState_Scale::Free(void)
{
}
