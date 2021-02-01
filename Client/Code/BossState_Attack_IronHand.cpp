#include "pch.h"
#include "BossState_Attack_IronHand.h"
#include "BossState_Idle.h"
#include "BossState_Run.h"
#include "BossState_Walk.h"
#include "BossState_Death.h"
#include "BossState_Attack_Mixer.h"
//#include "BossState_Attack_JumpDown.h"
//#include "BossState_Alert.h"
//#include "BossState_Damage.h"
//#include "BossState_Attack.h"
#include "StateMgr.h"
#include "Boss.h"
#include "Map.h"
#include "Attribute.h"
#include "IronHand.h"
#include "SplashAttack.h"


CBossState_Attack_IronHand::CBossState_Attack_IronHand(CBoss & _rOwner)
	:
	CState(_rOwner)
{
}

CBossState_Attack_IronHand::~CBossState_Attack_IronHand()
{
}

void CBossState_Attack_IronHand::OnLoaded(void)
{
	m_rOwner.GetDynamicMesh()->Set_AnimationSet(ANIM::Executioner_Attack05);
	m_rOwner.GetPhysics()->SetSpeed(0.f);
	//m_rOwner.GetAttackCollider()->SetActivated(true);

	if (!m_rOwner.GetTargetObject())
		m_rOwner.SetTargetObject(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
}

int CBossState_Attack_IronHand::Update(const _float& _fDeltaTime)
{
	if (m_rOwner.IsDead()) {
		m_rOwner.GetStateMgr()->SetNextState(new CBossState_Death(m_rOwner));
		return 0;
	}

	if (m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.95f) {
		switch (Engine::GetNumberBetweenMinMax(0, 2)) {
		case 0:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Idle(m_rOwner));
			break;
		case 1:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Walk(m_rOwner));
			break;
		case 2:
			m_rOwner.GetStateMgr()->SetNextState(new CBossState_Attack_Mixer(m_rOwner));
			break;
		}
		return 0;
	}
	else if (/*!m_bIsAttack && */m_rOwner.GetDynamicMesh()->GetAnimationProgress() >= 0.5f) {
		// TODO : 주먹 공격을 생성합니다.
		if (!m_bIsEffectOn) {
			CSplashAttack* pSplashAttack = CSplashAttack::Create(m_rOwner.GetGraphicDev());
			pSplashAttack->SetSplashAttackInfo(L"EnemyAttack", L"Player",
				m_rOwner.GetTransform()->GetPos(), 10.f, 4.f, 0.5f, WORLD_X_AXIS, 180.f);
			Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pSplashAttack);

			m_bIsEffectOn = true;
		}

		if (m_iAttackCount < 5 && (m_fTickTime += _fDeltaTime) >= 0.3f) {
			//Engine::GetTimer(L"Timer_FPS60")->RunToPause();
			_vec3 vPlayerPos = m_rOwner.GetTargetObject()->GetTransform()->GetPos();
			_vec3 vPos = m_rOwner.GetTransform()->GetPos();
			_vec3 vToPlayer = vPlayerPos - vPos;
			vToPlayer.y = vPos.y;

			_float fLength = D3DXVec3Length(&vToPlayer);
			_vec3 vStartPos;

			if (fLength > 0.f) {
				//_float fGap = fLength / 5.f;
				vStartPos = vPos + 5.f * (m_iAttackCount + 1) * vToPlayer / fLength;
			}
			else {
				vStartPos = vPlayerPos;
			}
			vStartPos.y -= 0.3f;

			CIronHand* pIronHand = CIronHand::Create(m_rOwner.GetGraphicDev());
			pIronHand->SetIronHandInfo(vStartPos, 0.2f);
			Engine::GetLayer(L"Environment")->Add_GameObject(pIronHand);

			++m_iAttackCount;
			m_fTickTime = 0.f;

			Engine::CCameraMgr* pCameraMgr = dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin());
			_float fT = (1.f - Engine::GetWeightByValue(fLength, 0.f, 50.f));
			pCameraMgr->GetCamera()->Shake(0.7f * fT, 0.3f * fT, 20 * fT);
		//	Engine::GetTimer(L"Timer_FPS60")->PauseToRun();
		}

		//m_bIsAttack = true;
	}

	return 0;
}

void CBossState_Attack_IronHand::OnExited(void)
{
	m_rOwner.GetPhysics()->SetResistanceCoefficientXZ(1.f);
	//m_rOwner.GetAttackCollider()->SetActivated(false);
}

void CBossState_Attack_IronHand::Free(void)
{
}
