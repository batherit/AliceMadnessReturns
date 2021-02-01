#include "pch.h"
#include "AliceWState_Jump.h"
#include "AliceWState_SuperJump.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Death.h"
#include "AliceWState_Slide.h"
#include "AliceWState_Dash.h"
#include "AliceWState_JumpDamage.h"
#include "EFT_SmokeBomb.h"
#include "StateMgr.h"
#include "AliceW.h"
#include "Map.h"
#include "Attribute.h"
#include "EFT_FloatingEffect.h"


CAliceWState_Jump::CAliceWState_Jump(CAliceW & _rOwner, _bool _bIsJumping)
	:
	CState(_rOwner)
{
	m_bIsJumping = _bIsJumping;
}

CAliceWState_Jump::~CAliceWState_Jump()
{
}

void CAliceWState_Jump::OnLoaded(void)
{
	if (m_bIsJumping) {
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_Start);
		m_rOwner.GetPhysics()->SetVelocityY(ALICE_JUMP_SPEED);
		m_eJumpStep = STEP_START;
		CSoundMgr::Get_Instance()->PlaySound(L"Alice_Jump.ogg", CSoundMgr::PLAYER);
		GenerateSmokeBombEffect();
		m_fElapsedTime = FLOATINGEFFECT_GEN_TIME;
	}
	else{
		m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_Fall);
		m_eJumpStep = STEP_FALL;
	}
	++m_iJumpNum;
	m_eJumpType = TYPE_FORWARD;
	m_rOwner.SetLanded(false);
	m_pNaviMesh = m_rOwner.GetMap()->GetNaviMesh();
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(0.95f);
}

int CAliceWState_Jump::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsSuperJumpOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_SuperJump(m_rOwner));
		m_rOwner.SetSuperJumped(false);
		return 0;
	}

	if (m_rOwner.GetAttribute()->IsDamaged()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_JumpDamage(m_rOwner));
		return 0;
	}

	if (m_rOwner.IsDashOn()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Dash(m_rOwner, m_rOwner.GetTransform()->GetLook()));
		return 0;
	}

	_vec3 vDir;
	if (!m_rOwner.IsLanded()) {
		// TODO : 바닥에 닿았는지 여부를 따져야합니다.
		if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
			_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
			m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED * 0.8f);
		}
	}
	else {
		switch (m_eJumpType) {
		case TYPE_FORWARD:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_LandLow);
			break;
		case TYPE_RIGHT:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Land);
			break;
		case TYPE_LEFT:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Land);
			break;
		}
		m_eJumpStep = STEP_LAND;
		// SOUND : JUMPLAND => 빼는게 더 낫다
		//CSoundMgr::Get_Instance()->PlaySound(L"Alice_JumpLand.ogg", CSoundMgr::PLAYER);
	}
		
	// Jump => Death, Jump, Run, Attack
	switch (m_eJumpStep) {
	case STEP_START:
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// Start 애니메이션이 끝난 상황
			switch (m_eJumpType) {
			case TYPE_FORWARD:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_Rise);
				break;
			case TYPE_RIGHT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Rise);
				break;
			case TYPE_LEFT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Rise);
				break;
			}
			// Rise 상태로 전환
			m_eJumpStep = STEP_RISE;
		}
		break;
	case STEP_RISE:
		if (m_eJumpType == TYPE_FORWARD) {
			if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
				// 포워드 점프인 경우 애니메이션이 다 끝나고 나서야 입력을 받을 수 있습니다.
				if (m_rOwner.IsJumpOn(_fDeltaTime) && m_iJumpNum < 4) {
					m_rOwner.GetPhysics()->SetVelocityY(ALICE_JUMP_SPEED * 0.5f);

					if (Engine::GetRandomBoolean())
						m_eJumpType = TYPE_RIGHT;
					else
						m_eJumpType = TYPE_LEFT;

					switch (m_eJumpType) {
					case TYPE_RIGHT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Start);
						break;
					case TYPE_LEFT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Start);
						break;
					}
					++m_iJumpNum;
					m_eJumpStep = STEP_START;

					GenerateSmokeBombEffect();
					m_fElapsedTime = FLOATINGEFFECT_GEN_TIME;

				}
				else {
					// 어떤 입력도 없다면 활강 애니메이션을 진행한다.
					switch (m_eJumpType) {
					case TYPE_FORWARD:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpFwd_Fall);
						break;
					case TYPE_RIGHT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Fall);
						break;
					case TYPE_LEFT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Fall);
						break;
					}
					m_eJumpStep = STEP_FALL;
				}
			}
		}
		else {
			if (m_rOwner.IsJumpOn(_fDeltaTime) && m_iJumpNum < 4) {
				m_rOwner.GetPhysics()->SetVelocityY(ALICE_JUMP_SPEED * 0.5f);

				if (Engine::GetRandomBoolean())
					m_eJumpType = TYPE_RIGHT;
				else
					m_eJumpType = TYPE_LEFT;

				switch (m_eJumpType) {
				case TYPE_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Start);
					break;
				case TYPE_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Start);
					break;
				}
				++m_iJumpNum;
				m_eJumpStep = STEP_START;
				GenerateSmokeBombEffect();
				m_fElapsedTime = FLOATINGEFFECT_GEN_TIME;
			}
			else if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
				// 어떤 입력도 없다면 활강 애니메이션을 진행한다.
				switch (m_eJumpType) {
				case TYPE_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Fall);
					break;
				case TYPE_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Fall);
					break;
				}
				m_eJumpStep = STEP_FALL;
			}
		}
		break;
	case STEP_FALL:
		if (m_rOwner.IsFalling(_fDeltaTime) && m_rOwner.IsFloatingOn(_fDeltaTime)) {
			m_rOwner.GetPhysics()->SetVelocityY(-2.f);
			GenerateFloatingEffect(_fDeltaTime);
		}
		// 활강 상태에선 점프를 다시 할 수 있습니다.
		if (m_rOwner.IsJumpOn(_fDeltaTime) && m_iJumpNum < 4) {
			m_rOwner.GetPhysics()->SetVelocityY(ALICE_JUMP_SPEED * 0.5f);

			if (Engine::GetRandomBoolean())
				m_eJumpType = TYPE_RIGHT;
			else
				m_eJumpType = TYPE_LEFT;

			switch (m_eJumpType) {
			case TYPE_RIGHT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Rgt_Start);
				break;
			case TYPE_LEFT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::AliceW_JumpD_Lft_Start);
				break;
			}
			++m_iJumpNum;
			m_eJumpStep = STEP_START;
			GenerateSmokeBombEffect();
			m_fElapsedTime = FLOATINGEFFECT_GEN_TIME;



			// SOUND : DoubleJump
			switch (Engine::GetNumberBetweenMinMax(0, 3)) {
			case 0:
				CSoundMgr::Get_Instance()->PlaySound(L"Alice_DoubleJump0.ogg", CSoundMgr::PLAYER);
				break;
			case 1:
				CSoundMgr::Get_Instance()->PlaySound(L"Alice_DoubleJump1.ogg", CSoundMgr::PLAYER);
				break;
			case 2:
				CSoundMgr::Get_Instance()->PlaySound(L"Alice_DoubleJump2.ogg", CSoundMgr::PLAYER);
				break;
			case 3:
				CSoundMgr::Get_Instance()->PlaySound(L"Alice_DoubleJump3.ogg", CSoundMgr::PLAYER);
				break;
			}

		}
		break;
	case STEP_LAND:
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			m_rOwner.GetPhysics()->SetGravity(9.8f * 3.f);
			if (m_rOwner.IsSliding(_fDeltaTime)) {
				if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
					_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
					m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
				}
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Slide(m_rOwner));
			}
			else if (m_rOwner.IsRunOn(_fDeltaTime, &vDir)) {
				_vec2 vDirXZ = _vec2(vDir.x, vDir.z);
				m_rOwner.GetPhysics()->SetVelocityXZ(vDirXZ * ALICE_RUN_SPEED);
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

void CAliceWState_Jump::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
}

void CAliceWState_Jump::Free(void)
{
}

void CAliceWState_Jump::GenerateSmokeBombEffect()
{
	CEFT_SmokeBomb* pEffect = CEFT_SmokeBomb::Create(m_rOwner.GetGraphicDev());
	pEffect->SetInfo(m_rOwner.GetTransform()->GetPos() + _vec3(0.f, 0.5f, 0.f), WORLD_Y_AXIS, 0.2f, 0.f, Engine::GetNumberBetweenMinMax(3, 5), 10.f, 0.4f, _vec2(0.3f, 0.3f), _vec2(0.5f, 0.5f));
	Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
}

void CAliceWState_Jump::GenerateFloatingEffect(const _float & _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) >= FLOATINGEFFECT_GEN_TIME) {
		CEFT_FloatingEffect* pEffect = CEFT_FloatingEffect::Create(m_rOwner.GetGraphicDev());
		pEffect->SetFloatingEffectInfo(&m_rOwner);
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
		m_fElapsedTime = 0.f;
	}
}
