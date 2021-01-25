#include "pch.h"
#include "EFT_SmokeBomb.h"
#include "PlateEffect.h"
#include "EffectMgr.h"

CEFT_SmokeBomb::CEFT_SmokeBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_SmokeBomb::CEFT_SmokeBomb(const CEFT_SmokeBomb & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_SmokeBomb::~CEFT_SmokeBomb(void)
{
}

HRESULT CEFT_SmokeBomb::Ready_Object(void)
{
	return S_OK;
}

_int CEFT_SmokeBomb::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	/*_bool bIsValid = false;
	for (auto& rSmoke : m_vecChildList) {
		if (rSmoke->IsActivated()) {
			bIsValid = true;
			break;
		}
	}
	if (!bIsValid) {
		SetValid(false);
		return 1;
	}*/

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CEFT_SmokeBomb::Render_Object(void)
{

}

CEFT_SmokeBomb * CEFT_SmokeBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_SmokeBomb*	pInstance = new CEFT_SmokeBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_SmokeBomb::Free(void)
{
	CGameObject::Free();
}

void CEFT_SmokeBomb::SetInfo(const _vec3 & _vPos, const _vec3& _vLook, _float _fRadius, _float _fOffset, _int _iParticleNum, _float _fParticleSpeed, _float _fResistanceXZ, const _vec2& _vStartSize, const _vec2& _vEndSize)
{
	GetTransform()->SetPos(_vPos);
	if (_vLook == WORLD_Y_AXIS) {
		GetTransform()->ResetRightUpLook(&(_vPos + WORLD_Y_AXIS), &WORLD_X_AXIS);
	}
	else {
		GetTransform()->ResetRightUpLook(&(_vPos + _vLook), &WORLD_Y_AXIS);
	}

	CPlateEffect* pEffect = nullptr;
	_vec3 vLook = GetTransform()->GetLook();
	_vec3 vUp = GetTransform()->GetUp();
	GetTransform()->SetAngle(0.f, 0.f, 0.f);
	_float fRotGap = 2.f * D3DX_PI / _iParticleNum;
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vLook, fRotGap);

	for (_int i = 0; i < _iParticleNum; ++i) {
		pEffect = CPlateEffect::Create(m_pGraphicDev);
		Engine::GetLayer(L"Environment")->Add_GameObject(L"Effect", pEffect);
		//AddChild(pEffect);
		pEffect->SetPlateEffectInfo(
			L"EFT_Smoke",
			_vPos + vUp * (_fRadius + Engine::GetNumberBetweenMinMax(-_fOffset, _fOffset)),
			_vStartSize, _vEndSize,
			Engine::GetNumberBetweenMinMax(0.f, 2.f * D3DX_PI),
			0.4f, _vec3(1.f, 1.f, 1.f), 
			CPlateEffect::DESTROY_INVALID);
		pEffect->GetPhysics()->SetSpeed(_fParticleSpeed);
		pEffect->GetPhysics()->SetDirection(vUp);
		pEffect->GetPhysics()->SetResistanceCoefficientXZ(_fResistanceXZ);
		pEffect->GetPhysics()->SetResistanceCoefficientY(_fResistanceXZ);
		pEffect->GetPhysics()->SetGravity(-8.5f);
		D3DXVec3TransformNormal(&vUp, &vUp, &matRot);
	}

	SetValid(false);
}
//
//void CEFT_SmokeBomb::SetInfo(const _vec3 & _vPos)
//{
//	GetTransform()->SetPos(_vPos);
//
//	m_pLightEffect = CPlateEffect::Create(m_pGraphicDev);
//	AddChild(m_pLightEffect);
//	m_pLightEffect->SetPlateEffectInfo(L"EFT_Light", _vec3(0.f, 0.f, 0.f), _vec2(0.7f, 0.7f), _vec2(1.2f, 1.2f), 0.f, 0.4f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);
//
//	m_pAttackEffect = CPlateEffect::Create(m_pGraphicDev);
//	AddChild(m_pAttackEffect);
//	m_pAttackEffect->SetPlateEffectInfo(L"EFT_HobbyHorseAttackAfterImage", _vec3(0.f, 0.f, 0.f), _vec2(0.7f, 0.7f), _vec2(1.f, 1.f), 0.f, 0.55f, _vec3(1.f, 1.f, 1.f), CPlateEffect::DESTROY_UNACTIVATED);
//}
