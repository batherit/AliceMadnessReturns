#include "pch.h"
#include "AliceWState_GunMode.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Death.h"
#include "AliceWState_Damage.h"
#include "AliceWState_Jump.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"
#include "UI_InGame.h"
#include "UI_Image.h"


CAliceWState_GunMode::CAliceWState_GunMode(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_GunMode::~CAliceWState_GunMode()
{
}

void CAliceWState_GunMode::OnLoaded(void)
{
	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	pCameraMgr->ChangeCameraController(3, 0.5f);
	pCameraMgr->SetLock(true);

	CUI_InGame* pUIInGame = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
	pUIInGame->GetAimImage()->SetActivated(true);

	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Fire);
	CDynamicObject* pGun = dynamic_cast<CDynamicObject*>(m_rOwner.GetWeapon());
	if (pGun) {
		pGun->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Fire);
	}
	m_eStateType = TYPE_FIRE;

	// TODO : 카메라 컨트롤러를 Gun으로 설정합니다.
}

int CAliceWState_GunMode::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Damage(m_rOwner));
		return 0;
	}

	_vec3 vDir;
	if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
		_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
		m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
	}
	else
		m_rOwner.GetPhysics()->SetVelocityXZ(_vec2(0.f, 0.f));

	switch (m_eStateType) {
	case TYPE_FIRE:
		if (m_rOwner.IsAttackOn(_fDeltaTime) || m_rOwner.IsAttacking(_fDeltaTime)) {
			// TODO : 총을 발사하는 로직을 짭니다.
			// 총의 Look축 방향으로 총을 발사합니다.
		}
		else {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Release);
			CDynamicObject* pGun = dynamic_cast<CDynamicObject*>(m_rOwner.GetWeapon());
			if (pGun) {
				pGun->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Release);
			}
			m_eStateType = TYPE_RELEASE;
		}
		break;
	case TYPE_RELEASE:
		if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
			if (m_rOwner.IsAttackOn(_fDeltaTime) || m_rOwner.IsAttacking(_fDeltaTime)) {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Fire);
				CDynamicObject* pGun = dynamic_cast<CDynamicObject*>(m_rOwner.GetWeapon());
				if (pGun) {
					pGun->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Fire);
				}
				m_eStateType = TYPE_FIRE;
			}
			else {
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Attack_Idle);
				CDynamicObject* pGun = dynamic_cast<CDynamicObject*>(m_rOwner.GetWeapon());
				if (pGun) {
					pGun->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Idle_Pose);
				}
				m_eStateType = TYPE_IDLE_OR_RUN;
			}
		}
		break;
	case TYPE_IDLE_OR_RUN:
		if (m_rOwner.IsFalling(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Jump(m_rOwner, false));
			return 0;
		}

		if (m_rOwner.IsAttackOn(_fDeltaTime) || m_rOwner.IsAttacking(_fDeltaTime)) {
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_WP3_Fire);
			CDynamicObject* pGun = dynamic_cast<CDynamicObject*>(m_rOwner.GetWeapon());
			if (pGun) {
				pGun->GetDynamicMesh()->Set_AnimationSet(ANIM::WP3_Fire);
			}
			m_eStateType = TYPE_FIRE;
		}
		else if (m_rOwner.IsGunModeReleased()) {
			if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
			}
		}
		break;
	}

	return 0;
}

void CAliceWState_GunMode::OnExited(void)
{
	_vec3 vLook = m_rOwner.GetTransform()->GetLook();
	vLook.y = 0.f;
	D3DXVec3Normalize(&vLook, &vLook);
	m_rOwner.GetTransform()->ResetRightUpLook(&(m_rOwner.GetTransform()->GetPos() + vLook), &WORLD_Y_AXIS);
	Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
	pCameraMgr->SetLock(false);

	if (!m_rOwner.IsFalling(0.f) && m_rOwner.IsLockOn())
		pCameraMgr->ChangeCameraController(2, 0.5f);
	else
		pCameraMgr->ChangeCameraController(0, 0.5f);

	CUI_InGame* pUIInGame = dynamic_cast<CUI_InGame*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"UI_InGame").begin());
	pUIInGame->GetAimImage()->SetActivated(false);
}

void CAliceWState_GunMode::Free(void)
{
}
