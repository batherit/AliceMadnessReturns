#include "pch.h"
#include "EFT_FloatingEffect.h"

CEFT_FloatingEffect::CEFT_FloatingEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_FloatingEffect::CEFT_FloatingEffect(const CEFT_FloatingEffect & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_FloatingEffect::~CEFT_FloatingEffect(void)
{
}

HRESULT CEFT_FloatingEffect::Ready_Object(void)
{
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"EFT_FloatingEffect"));
	m_mapComponent[Engine::ID_STATIC][Engine::CStaticMesh::GetComponentTag()] = m_pMesh;

	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_PlateEffect"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	//m_pPhysics = AddComponent<Engine::CPhysics>();

	m_fLifeTime = FLOATINGEFFECT_LIFETIME;

	return S_OK;
}

_int CEFT_FloatingEffect::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fLifeTime -= _fDeltaTime) <= 0.f) {
		m_fLifeTime = 0.f;
		switch (m_eDestroyType) {
		case DESTROY_INVALID:
			SetValid(false);
			break;
		case DESTROY_UNACTIVATED:
			SetActivated(false);
			break;
		}
		return 1;
	}

	// 크기 조정
	_float fT = 1.f - m_fLifeTime / FLOATINGEFFECT_LIFETIME;
	_vec3 vNewSize = _vec3(Engine::GetValueByWeight(fT, 0.4f, 1.2f), Engine::GetValueByWeight(fT, 0.f, 5.f), Engine::GetValueByWeight(fT, 0.4f, 1.2f));
	GetTransform()->SetScaleXYZ(vNewSize.x, vNewSize.y, vNewSize.z);

	// 회전 조정
	GetTransform()->RotateByUp(m_fSign * D3DXToRadian(FLOATINGEFFECT_ROT_DEGREE_SPEED) * _fDeltaTime);

	// 위치 조정
	GetTransform()->SetPos(m_pTargetObject->GetTransform()->GetPos());

	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CEFT_FloatingEffect::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	auto pEffect = m_pShader->Get_EffectHandle();
	pEffect->SetFloat("g_fT", Engine::GetValueByWeight(1.f - m_fLifeTime / FLOATINGEFFECT_LIFETIME, 0.f, 1.f));
	pEffect->SetVector("g_vEffectColor", &FLOATINGEFFECT_COLOR);
	m_pRenderer->Render(pEffect, 1);
}

CEFT_FloatingEffect * CEFT_FloatingEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_FloatingEffect*	pInstance = new CEFT_FloatingEffect(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_FloatingEffect::Free(void)
{
	CGameObject::Free();
}

void CEFT_FloatingEffect::SetFloatingEffectInfo(Engine::CGameObject* _pTargetObject, E_DESTROY_TYPE _eDestroyType)
{
	m_pTargetObject = _pTargetObject;
	GetTransform()->SetPos(m_pTargetObject->GetTransform()->GetPos());
	
	// 임의의 Look축을 세팅한다.
	GetTransform()->RotateByUp(Engine::GetNumberBetweenMinMax(0.f, 2.f*D3DX_PI));
	GetTransform()->RotateByRight(Engine::GetNumberBetweenMinMax(D3DXToRadian(-20.f), D3DXToRadian(20.f)));
	// Up축으로 임의로 회전시킨다.
	GetTransform()->RotateByUp(Engine::GetNumberBetweenMinMax(0.f, 2.f*D3DX_PI));

	// 회전 방향 결정하기
	m_fSign = Engine::GetRandomBoolean() ? -1.f : 1.f;

	m_eDestroyType = _eDestroyType;
}

