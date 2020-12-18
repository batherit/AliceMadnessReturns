#include "pch.h"
#include "AliceWState_Jump.h"
#include "AliceWState_Idle.h"
#include "AliceWState_Run.h"
#include "AliceWState_Attack.h"
#include "AliceWState_Death.h"
#include "StateMgr.h"
#include "AliceW.h"


CAliceWState_Jump::CAliceWState_Jump(CAliceW & _rOwner)
	:
	CState(_rOwner)
{
}

CAliceWState_Jump::~CAliceWState_Jump()
{
}

void CAliceWState_Jump::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpFwd_Start);
	++m_iJumpNum;
	m_bIsJumping = true;
	m_eJumpStep = STEP_START;
	m_eJumpType = TYPE_FORWARD;
}

int CAliceWState_Jump::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Death(m_rOwner));
		return 0;
	}

	// TODO : 바닥에 닿았는지 여부를 따져야합니다.

	if (m_eJumpStep != STEP_LAND)
		m_rOwner.ProcessMove(_fDeltaTime);

	// Jump => Death, Jump, Run, Attack
	switch (m_eJumpStep) {
	case STEP_START:
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			// Start 애니메이션이 끝난 상황
			switch (m_eJumpType) {
			case TYPE_FORWARD:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpFwd_Rise);
				break;
			case TYPE_RIGHT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Rise);
				break;
			case TYPE_LEFT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Rise);
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
					if (Engine::GetRandomBoolean())
						m_eJumpType = TYPE_RIGHT;
					else
						m_eJumpType = TYPE_LEFT;

					switch (m_eJumpType) {
					case TYPE_RIGHT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Start);
						break;
					case TYPE_LEFT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Start);
						break;
					}
					++m_iJumpNum;
					m_eJumpStep = STEP_START;

					// TODO : 점프에 대한 물리 처리를 해주어야 합니다.
				}
				else {
					// 어떤 입력도 없다면 활강 애니메이션을 진행한다.
					switch (m_eJumpType) {
					case TYPE_FORWARD:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpFwd_Fall);
						break;
					case TYPE_RIGHT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Fall);
						break;
					case TYPE_LEFT:
						m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Fall);
						break;
					}
					m_eJumpStep = STEP_FALL;
				}
			}
		}
		else {
			if (m_rOwner.IsJumpOn(_fDeltaTime) && m_iJumpNum < 4) {
				if (Engine::GetRandomBoolean())
					m_eJumpType = TYPE_RIGHT;
				else
					m_eJumpType = TYPE_LEFT;

				switch (m_eJumpType) {
				case TYPE_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Start);
					break;
				case TYPE_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Start);
					break;
				}
				++m_iJumpNum;
				m_eJumpStep = STEP_START;
			}
			else if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
				// 어떤 입력도 없다면 활강 애니메이션을 진행한다.
				switch (m_eJumpType) {
				case TYPE_RIGHT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Fall);
					break;
				case TYPE_LEFT:
					m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Fall);
					break;
				}
				m_eJumpStep = STEP_FALL;
			}
		}
		break;
	case STEP_FALL:
		// 활강 상태에선 점프를 다시 할 수 있습니다.
		if (m_rOwner.IsJumpOn(_fDeltaTime) && m_iJumpNum < 4) {
			if (Engine::GetRandomBoolean())
				m_eJumpType = TYPE_RIGHT;
			else
				m_eJumpType = TYPE_LEFT;

			switch (m_eJumpType) {
			case TYPE_RIGHT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Start);
				break;
			case TYPE_LEFT:
				m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Start);
				break;
			}
			++m_iJumpNum;
			m_eJumpStep = STEP_START;

			// TODO : 점프에 대한 물리 처리를 해주어야 합니다.
		}
		break;
	case STEP_LAND:
		if (m_rOwner.GetDynamicMesh()->Is_AnimationSetEnd()) {
			if (m_rOwner.ProcessMove(_fDeltaTime)) {
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Run(m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CAliceWState_Idle(m_rOwner));
			}
		}
		break;
	}

	// TODO : 착륙 시 상태를 전환하는 코드를 만들어야 합니다.
	// 지금은 임시로 점프 4회가 되면 종료.
	if (m_eJumpStep != STEP_LAND && m_iJumpNum >= 4) {
		// 착지했다면 임시 조건입니다.
		switch (m_eJumpType) {
		case TYPE_FORWARD:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpFwd_LandLow);
			break;
		case TYPE_RIGHT:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Rgt_Land);
			break;
		case TYPE_LEFT:
			m_rOwner.GetDynamicMesh()->Set_AnimationSet(ALICE::AliceW_JumpD_Lft_Land);
			break;
		}

		m_eJumpStep = STEP_LAND;
	}

	return 0;
}

void CAliceWState_Jump::OnExited(void)
{
}

void CAliceWState_Jump::Free(void)
{
}
