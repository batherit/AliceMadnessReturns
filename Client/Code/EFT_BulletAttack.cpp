#include "pch.h"
#include "EFT_BulletAttack.h"
#include "PlateEffect.h"
#include "EffectMgr.h"

CEFT_BulletAttack::CEFT_BulletAttack(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_BulletAttack::CEFT_BulletAttack(const CEFT_BulletAttack & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_BulletAttack::~CEFT_BulletAttack(void)
{
}

HRESULT CEFT_BulletAttack::Ready_Object(void)
{
	//dynamic_cast<CEffectMgr*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"EffectMgr").begin())->SetTimeSpeed(0.08f, 0.13f);

	return S_OK;
}

_int CEFT_BulletAttack::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;


	if (!m_pAttackEffect->IsActivated() && !m_pLightEffect->IsActivated()) {
		SetValid(false);
		return 1;
	}

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CEFT_BulletAttack::Render_Object(void)
{

}

CEFT_BulletAttack * CEFT_BulletAttack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_BulletAttack*	pInstance = new CEFT_BulletAttack(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_BulletAttack::Free(void)
{
	CGameObject::Free();
}

void CEFT_BulletAttack::SetInfo(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos);

	m_pLightEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pLightEffect);
	m_pLightEffect->SetPlateEffectInfo(L"EFT_Light", _vec3(0.f, 0.f, 0.f), _vec2(0.7f, 0.7f), _vec2(1.2f, 1.2f), 0.f, 0.4f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);
	
	m_pAttackEffect = CPlateEffect::Create(m_pGraphicDev);
	AddChild(m_pAttackEffect);
	m_pAttackEffect->SetPlateEffectInfo(L"EFT_HobbyHorseAttackAfterImage", _vec3(0.f, 0.f, 0.f), _vec2(0.7f, 0.7f), _vec2(1.f, 1.f), 0.f, 0.55f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);
}
