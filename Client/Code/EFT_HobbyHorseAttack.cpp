#include "pch.h"
#include "EFT_HobbyHorseAttack.h"
#include "PlateEffect.h"
#include "EffectMgr.h"

CEFT_HobbyHorseAttack::CEFT_HobbyHorseAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_HobbyHorseAttack::CEFT_HobbyHorseAttack(const CEFT_HobbyHorseAttack & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_HobbyHorseAttack::~CEFT_HobbyHorseAttack(void)
{
}

HRESULT CEFT_HobbyHorseAttack::Ready_Object(void)
{
	//m_pTimer = Engine::GetTimer(L"Timer_FPS60");
	//m_pTimer->SetDeltaMultiple(0.08f);
	dynamic_cast<CEffectMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"EffectMgr").begin())->SetTimeSpeed(0.04f, 0.2f);
	dynamic_cast<Engine::CCameraMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"CameraMgr").begin())->GetCamera()->Shake(0.12f * 0.04f, 0.06f, 10);
	
	switch (Engine::GetNumberBetweenMinMax(0, 2)) 
	{
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Attack0.ogg", CSoundMgr::PLAYER);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Attack1.ogg", CSoundMgr::PLAYER);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"HobbyHorse_Attack2.ogg", CSoundMgr::PLAYER);
		break;
	}
	
	return S_OK;
}

_int CEFT_HobbyHorseAttack::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	//if (!m_bSlowRelease && (m_fSlowTime += _fDeltaTime / m_pTimer->GetDeltaMultiple()) >= 0.13f) {
	//	m_pTimer->SetDeltaMultiple(1.f);
	//	m_bSlowRelease = true;
	//}


	if (!m_pAttackEffect->IsActivated() && !m_pAttackAfterEffect->IsActivated()) {
		//Engine::GetTimer(L"Timer_FPS60")->SetDeltaMultiple(1.f);
		SetValid(false);
		return 1;
	}

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CEFT_HobbyHorseAttack::Render_Object(void)
{

}

CEFT_HobbyHorseAttack * CEFT_HobbyHorseAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_HobbyHorseAttack*	pInstance = new CEFT_HobbyHorseAttack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_HobbyHorseAttack::Free(void)
{
	CGameObject::Free();
}

void CEFT_HobbyHorseAttack::SetInfo(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos);

	m_pAttackEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pAttackEffect);
	m_pAttackEffect->SetPlateEffectInfo(L"EFT_HobbyHorseAttack", _vec3(0.f, 0.f, 0.f), _vec2(0.5f, 0.5f), _vec2(1.2f, 1.2f), 0.f, 0.3f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);


	m_pAttackAfterEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pAttackAfterEffect);
	m_pAttackAfterEffect->SetPlateEffectInfo(L"EFT_HobbyHorseAttackAfterImage", _vec3(0.f, 0.f, 0.f), _vec2(0.5f, 0.5f), _vec2(1.5f, 1.5f), 0.f, 0.5f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED, 0.15f);
}
