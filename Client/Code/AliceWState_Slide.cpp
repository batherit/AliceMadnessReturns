#include "pch.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"


CAliceWState_Slide::CAliceWState_Slide(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Slide::~CAliceWState_Slide()
{
}

void CAliceWState_Slide::OnLoaded(void)
{
	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	pCameraMgr->GetCameraController(1)->SetStickDir(pCameraMgr->GetCameraController(0)->GetStickDir());
	pCameraMgr->ChangeCameraController(1, 0.5f);
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceL_Slide);
}

int CAliceWState_Slide::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	_vec3 vSettedPos = m_rOwner.GetTransform()->GetPos();
	if (m_rOwner.IsSliding(_fDeltaTime)) {
		_float fMaxVelY = PHYSICS_MIN_VEL_Y * (1.f - Engine::Clamp(D3DXVec3Dot(&m_rOwner.GetMap()->GetNaviMesh()->GetCurCell()->GetNormal(), &WORLD_Y_AXIS), 0.f, 1.f));
		if (m_rOwner.GetPhysics()->GetVelocity().y < fMaxVelY) {
			m_rOwner.GetPhysics()->SetVelocityY(fMaxVelY);
		}

		_vec3 vCellNormalXZ = m_rOwner.GetMap()->GetNaviMesh()->GetCurCell()->GetNormal();
		vCellNormalXZ.y = 0.f;
		vCellNormalXZ *= 10.f;
		m_rOwner.GetPhysics()->AddVelocityXZ(_vec2(vCellNormalXZ.x, vCellNormalXZ.z) * _fDeltaTime);
		if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
			m_rOwner.GetPhysics()->AddVelocityXZ(_vec2(vDir.x, vDir.z) * 10.f * _fDeltaTime);
		}
	}
	// Slide => Death, Jump, Idle, Attack
	else if (m_rOwner.IsFalling(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
	}

	return 0;
}

void CAliceWState_Slide::OnExited(void)
{
	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	pCameraMgr->GetCameraController(0)->SetStickDir(pCameraMgr->GetCameraController(1)->GetStickDir());
	pCameraMgr->ChangeCameraController(0, 0.5f);
}

void CAliceWState_Slide::Free(void)
{
}