#include "pch.h"
#include "UI_BunnyBomb.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_BunnyBomb::CUI_BunnyBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_BunnyBomb::CUI_BunnyBomb(const CUI_BunnyBomb & rhs)
	:
	CGameObject(rhs)
{
}

CUI_BunnyBomb::~CUI_BunnyBomb(void)
{
}

HRESULT CUI_BunnyBomb::Ready_Object(void)
{
	m_pBunnyBomb = CUI_Image::Create(m_pGraphicDev);
	m_pBunnyBomb->SetTexture(L"UI_BunnyBomb");
	m_pBunnyBomb->SetPos(WINCX - (m_pBunnyBomb->GetWidth() >> 1) - OFFSET_X, (m_pBunnyBomb->GetHeight() >> 1) + OFFSET_Y);
	AddChild(m_pBunnyBomb);

	m_pArrow = CUI_Image::Create(m_pGraphicDev);
	m_pArrow->SetTexture(L"UI_Arrow");
	m_pArrow->SetPos(0.f, 30.f);
	m_pBunnyBomb->AddChild(m_pArrow);

	return S_OK;
}

int CUI_BunnyBomb::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_BunnyBomb::Render_Object(void)
{
}

CUI_BunnyBomb * CUI_BunnyBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_BunnyBomb*	pInstance = new CUI_BunnyBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_BunnyBomb::Free(void)
{
	CGameObject::Free();
}

void CUI_BunnyBomb::SetProgress(_float _fProgress)
{
	{
		m_fProgress = Engine::Clamp(_fProgress, 0.f, 1.f);
		m_pArrow->GetTransform()->SetAngleZ(2.f * D3DX_PI * m_fProgress);
	}
}
