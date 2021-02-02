#include "pch.h"
#include "EFT_Explosion.h"

CEFT_Explosion::CEFT_Explosion(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CEFT_Explosion::CEFT_Explosion(const CEFT_Explosion & rhs)
	:
	CGameObject(rhs)
{
}

CEFT_Explosion::~CEFT_Explosion(void)
{
}

HRESULT CEFT_Explosion::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_ALPHA,
		dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")),
		nullptr);
	m_pRenderer->SetTexture(static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Explosion")));

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Alpha"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	GetTransform()->SetScaleXYZ(2.f, 2.f, 2.f);
	CSoundMgr::Get_Instance()->PlaySound(L"CannonBall_Impact.ogg", CSoundMgr::PLAYER);

	return S_OK;
}

_int CEFT_Explosion::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if ((m_fElapsedTime += _fDeltaTime) >= EXPLOSION_TIME) {
		SetValid(false);
		return 1;
	}

	m_pRenderer->SetTextureIndex(Engine::GetValueByWeight(m_fElapsedTime / EXPLOSION_TIME, 0, 89));
	m_pRenderer->Update(_fDeltaTime);

	return 0;
}

void CEFT_Explosion::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

CEFT_Explosion * CEFT_Explosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEFT_Explosion*	pInstance = new CEFT_Explosion(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEFT_Explosion::Free(void)
{
	CGameObject::Free();
}

void CEFT_Explosion::SetExplosionInfo(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos + _vec3(0.f, 0.f, -2.f));
}
