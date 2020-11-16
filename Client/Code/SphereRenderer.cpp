#include "pch.h"
#include "SphereRenderer.h"

CSphereRenderer::CSphereRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CSphereRenderer::CSphereRenderer(const CSphereRenderer & rhs)
	:
	CGameObject(rhs)
{
}

CSphereRenderer::~CSphereRenderer(void)
{
}

HRESULT CSphereRenderer::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	return S_OK;
}

int CSphereRenderer::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CSphereRenderer::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_SphereCol");
}

CSphereRenderer * CSphereRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereRenderer*	pInstance = new CSphereRenderer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CSphereRenderer::Free(void)
{
	CGameObject::Free();
}
