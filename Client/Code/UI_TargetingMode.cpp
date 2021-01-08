#include "pch.h"
#include "UI_TargetingMode.h"
#include "UI_Image.h"
#include "Attribute.h"
#include "AliceW.h"

CUI_TargetingMode::CUI_TargetingMode(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_TargetingMode::CUI_TargetingMode(const CUI_TargetingMode & rhs)
	:
	CGameObject(rhs)
{
}

CUI_TargetingMode::~CUI_TargetingMode(void)
{
}

HRESULT CUI_TargetingMode::Ready_Object(void)
{
	m_pImageLT = CUI_Image::Create(m_pGraphicDev);
	m_pImageLT->SetTexture(L"UI_TargetingSide");
	m_pImageLT->SetOutputArea(RECT{ 0, 0, static_cast<LONG>(WINCX / 3.f + 1), 128});
	AddChild(m_pImageLT);
	m_pImageMT = CUI_Image::Create(m_pGraphicDev);
	m_pImageMT->SetTexture(L"UI_TargetingMid");
	m_pImageMT->SetOutputArea(RECT{ static_cast<LONG>(WINCX / 3.f), 0, static_cast<LONG>(WINCX * 2.f/ 3.f + 1), 128 });
	AddChild(m_pImageMT);
	m_pImageRT = CUI_Image::Create(m_pGraphicDev);
	m_pImageRT->SetTexture(L"UI_TargetingSide");
	m_pImageRT->GetTransform()->SetScaleX(-1.f);
	m_pImageRT->SetOutputArea(RECT{ static_cast<LONG>(WINCX * 2.f / 3.f), 0, WINCX, 128 });
	AddChild(m_pImageRT);

	m_pImageLB = CUI_Image::Create(m_pGraphicDev);
	m_pImageLB->SetTexture(L"UI_TargetingSide");
	m_pImageLB->GetTransform()->SetScaleXYZ(1.f, -1.f, 1.f);
	m_pImageLB->SetOutputArea(RECT{ 0, WINCY - 128, static_cast<LONG>(WINCX / 3.f + 1), WINCY });
	AddChild(m_pImageLB);
	m_pImageMB = CUI_Image::Create(m_pGraphicDev);
	m_pImageMB->SetTexture(L"UI_TargetingMid");
	m_pImageMB->GetTransform()->SetScaleY(-1.f);
	m_pImageMB->SetOutputArea(RECT{ static_cast<LONG>(WINCX / 3.f), WINCY - 128, static_cast<LONG>(WINCX * 2.f / 3.f + 1), WINCY });
	AddChild(m_pImageMB);
	m_pImageRB = CUI_Image::Create(m_pGraphicDev);
	m_pImageRB->SetTexture(L"UI_TargetingSide");
	m_pImageRB->GetTransform()->SetScaleXYZ(-1.f, -1.f, 1.f);
	m_pImageRB->SetOutputArea(RECT{ static_cast<LONG>(WINCX * 2.f / 3.f), WINCY - 128, WINCX, WINCY });
	AddChild(m_pImageRB);

	return S_OK;
}



_int CUI_TargetingMode::Update_Object(const _float & _fDeltaTime)
{
	// m_fCurHP °»½Å
	CAliceW* pAlice = dynamic_cast<CAliceW*>(m_pPlayer);
	if (pAlice && pAlice->GetTargetObject()) {
		CGameObject::Update_Object(_fDeltaTime);
	}
	
	return 0;
}

void CUI_TargetingMode::Render_Object(void)
{
}

CUI_TargetingMode * CUI_TargetingMode::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_TargetingMode*	pInstance = new CUI_TargetingMode(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_TargetingMode::Free(void)
{
	CGameObject::Free();
}

void CUI_TargetingMode::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
}
