#include "pch.h"
#include "EFT_SlashAttack.h"
#include "PlateEffect.h"
#include "EffectMgr.h"

CEFT_SlashAttack::CEFT_SlashAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_SlashAttack::CEFT_SlashAttack(const CEFT_SlashAttack & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_SlashAttack::~CEFT_SlashAttack(void)
{
}

HRESULT CEFT_SlashAttack::Ready_Object(void)
{
	dynamic_cast<CEffectMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"EffectMgr").begin())->SetTimeSpeed(0.04f, 0.1f);
	//dynamic_cast<CEffectMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"EffectMgr").begin())->SetTimeSpeed(0.08f, 0.13f);
	PlaySlashSound();
	return S_OK;
}

_int CEFT_SlashAttack::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;


	if (/*!m_pSlashEffect->IsActivated() && */!m_pLightEffect->IsActivated()) {
		SetValid(false);
		return 1;
	}

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CEFT_SlashAttack::Render_Object(void)
{

}

CEFT_SlashAttack * CEFT_SlashAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_SlashAttack*	pInstance = new CEFT_SlashAttack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_SlashAttack::Free(void)
{
	CGameObject::Free();
}

void CEFT_SlashAttack::SetInfo(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos);

	m_pSlashEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pSlashEffect);
	m_pSlashEffect->SetPlateEffectInfo(L"EFT_Slash", _vec3(0.f, 0.f, 0.f), _vec2(1.8f, 0.20f), _vec2(2.3f, 0.f), Engine::GetNumberBetweenMinMax(0.f, 2.f * D3DX_PI), 0.4f, _vec3(0.8f, 0.8f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);


	m_pLightEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pLightEffect);
	m_pLightEffect->SetPlateEffectInfo(L"EFT_Light", _vec3(0.f, 0.f, 0.f), _vec2(1.f, 1.f), _vec2(0.5f, 0.5f), 0.f, 0.3f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED, 0.15f);
}

void CEFT_SlashAttack::PlaySlashSound()
{
	switch (Engine::GetNumberBetweenMinMax(0, 3)) {
	case 0:
		CSoundMgr::Get_Instance()->PlaySound(L"Vorpal_Attack0.ogg", CSoundMgr::EFFECT);
		break;
	case 1:
		CSoundMgr::Get_Instance()->PlaySound(L"Vorpal_Attack1.ogg", CSoundMgr::EFFECT);
		break;
	case 2:
		CSoundMgr::Get_Instance()->PlaySound(L"Vorpal_Attack2.ogg", CSoundMgr::EFFECT);
		break;
	case 3:
		CSoundMgr::Get_Instance()->PlaySound(L"Vorpal_Attack3.ogg", CSoundMgr::EFFECT);
		break;
	}
}
