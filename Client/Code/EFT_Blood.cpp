#include "pch.h"
#include "EFT_Blood.h"
#include "AnimUV.h"

CEFT_Blood::CEFT_Blood(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_Blood::CEFT_Blood(const CEFT_Blood & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_Blood::~CEFT_Blood(void)
{
}

HRESULT CEFT_Blood::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA,
		dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")),
		nullptr);

	switch (Engine::GetNumberBetweenMinMax(0, 2)) {
	case 0:
		m_pRenderer->SetTexture(static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Blood0")));
		break;
	case 1:
		m_pRenderer->SetTexture(static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Blood1")));
		break;
	case 2:
		m_pRenderer->SetTexture(static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Blood2")));
		break;
	}

	// AnimUV
	m_pAnimUV = AddComponent<CAnimUV>();
	m_pAnimUV->SetNewAnimInfo(AnimInfo(0, 8, 4, 0, 32, 0.8f, 1));

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Anim"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetGravity(9.8f);
	

	return S_OK;
}

_int CEFT_Blood::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	/*m_fActivationTime = 0.f;
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
	}*/

	if (m_pAnimUV->Update(_fDeltaTime) == 1) {
		if ((m_fElapsedTime + _fDeltaTime) >= 1.f) {
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
	}

	GetTransform()->SetPos(GetPhysics()->GetUpdatedPos(_fDeltaTime, Engine::CTransform::COORD_TYPE_LOCAL));
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CEFT_Blood::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	auto pEffect = m_pShader->Get_EffectHandle();
	pEffect->SetFloat("g_fU", m_pAnimUV->GetAnimU());
	pEffect->SetFloat("g_fV", m_pAnimUV->GetAnimV());
	pEffect->SetFloat("g_fGapU", m_pAnimUV->GetGapU());
	pEffect->SetFloat("g_fGapV", m_pAnimUV->GetGapV());
	pEffect->SetVector("g_vEffectColor", &BLOOD_COLOR);
	m_pRenderer->Render(pEffect);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CEFT_Blood * CEFT_Blood::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_Blood*	pInstance = new CEFT_Blood(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_Blood::Free(void)
{
	CGameObject::Free();
}

void CEFT_Blood::SetBloodInfo(const _vec3 & _vPos, const _vec3& _vScale, E_DESTROY_TYPE _eDestroyType)
{
	GetTransform()->SetPos(_vPos);
	GetTransform()->SetScale(_vScale);
	m_eDestroyType = _eDestroyType;

	// 빌보드 회전
	// 이펙트를 카메라쪽으로 빌보드 회전
	_matrix matView;
	_vec3 vPos = GetTransform()->GetPos();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	_vec3 vNewLook = _vec3(matView._41, matView._42, matView._43) - vPos;		// 카메라 위치를 향하는 look벡터를 구함.
	D3DXVec3Normalize(&vNewLook, &vNewLook);
	GetTransform()->ResetRightUpLook(&(vPos + vNewLook), &WORLD_Y_AXIS);		// 빌보드 회전

	// 임의의 Up축을 생성한다.
	_vec3 vUp = WORLD_Y_AXIS;
	_vec3 vRight = WORLD_X_AXIS;
	_matrix matRot;

	D3DXMatrixRotationY(&matRot, Engine::GetNumberBetweenMinMax(0.f, 2.f * D3DX_PI));
	D3DXVec3TransformNormal(&vRight, &vRight, &matRot);
	D3DXMatrixRotationAxis(&matRot, &vRight, Engine::GetNumberBetweenMinMax(D3DXToRadian(-70.f), D3DXToRadian(70.f)));
	D3DXVec3TransformNormal(&vUp, &vUp, &matRot);

	// 해당 Up축에 대하여 가속한다.
	m_pPhysics->SetVelocity(vUp * Engine::GetNumberBetweenMinMax(2.f, 4.f));
}
