#include "pch.h"
#include "UI_HPPiece.h"

CUI_HPPiece::CUI_HPPiece(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
}

CUI_HPPiece::CUI_HPPiece(const CUI_HPPiece & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_HPPiece::~CUI_HPPiece(void)
{
}

HRESULT CUI_HPPiece::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	return S_OK;
}

int CUI_HPPiece::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CUI_HPPiece::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
	Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"Buffer_SphereCol");
}

CUI_HPPiece * CUI_HPPiece::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_HPPiece*	pInstance = new CUI_HPPiece(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_HPPiece::Free(void)
{
	CUI_Image::Free();
}
