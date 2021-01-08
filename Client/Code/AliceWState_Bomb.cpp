#include "pch.h"
#include "AliceWState_Bomb.h"
#include "AliceWState_Run.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Death.h"
#include "AliceWState_Damage.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "DynamicObject.h"
#include "Map.h"
#include "Attribute.h"
#include "BunnyBomb.h"


CAliceWState_Bomb::CAliceWState_Bomb(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Bomb::~CAliceWState_Bomb()
{
}

void CAliceWState_Bomb::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_ClockBomb_Spawn_Still);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
}

int CAliceWState_Bomb::Update(const _float& _fDeltaTime)
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
	if (!m_bIsGenerated && m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.2f) {
		// TODO : Åä³¢ ÆøÅºÀ» »ý¼ºÇÕ´Ï´Ù.
		CBunnyBomb* pBomb = CBunnyBomb::Create(m_rOwner.GetGraphicDev());
		_vec3 vLook = m_rOwner.GetTransform()->GetLook();
		_vec3 vPos = m_rOwner.GetTransform()->GetPos();
		vPos.y += 1.f;
		pBomb->GetTransform()->SetPos(vPos);
		pBomb->GetTransform()->ResetRightUpLook(&(vPos + vLook), &WORLD_Y_AXIS);

		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &m_rOwner.GetTransform()->GetRight(), -D3DXToRadian(45.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		pBomb->GetPhysics()->SetVelocity(vLook * 7.f);
		Engine::GetLayer(L"Environment")->Add_GameObject(L"BunnyBomb", pBomb);
		m_bIsGenerated = true;
	}
	else if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
		if (m_rOwner.IsMoving(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
		}
		else
			m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
	}

	return 0;
}

void CAliceWState_Bomb::OnExited(void)
{
}

void CAliceWState_Bomb::Free(void)
{
}
