#include "pch.h"
#include "UI_BloodScreen.h"

CUI_BloodScreen::CUI_BloodScreen(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{

}

CUI_BloodScreen::CUI_BloodScreen(const CUI_BloodScreen & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_BloodScreen::~CUI_BloodScreen()
{
}

HRESULT CUI_BloodScreen::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"EFT_BloodScreen");
	SetRenderColor(D3DCOLOR_ARGB(0, 255, 255, 255));
	SetOutputArea(-100, -100, WINCX + 100, WINCY + 100);

	return S_OK;
}

_int CUI_BloodScreen::Update_Object(const _float & _fDeltaTime)
{
	//if (!m_bIsEffectStarting) return 0;
	if (1 == CUI_Image::Update_Object(_fDeltaTime))
		return 1;

	if (m_bIsProcessing) {
		if (!m_bIsFadeIn) {
			if ((m_fElapsedTime += _fDeltaTime) >= 0.5f) {
				m_fElapsedTime = 0.f;
				m_bIsFadeIn = true;
			}
		}
		else {
			if ((m_fElapsedTime += _fDeltaTime) >= 0.2f) {
				SetRenderColor(D3DCOLOR_ARGB(0, 255, 255, 255));
				m_fElapsedTime = 0.f;
				m_bIsFadeIn = false;
				m_bIsProcessing = false;
				SetActivated(false);
			}
			else {
				_float fT = m_fElapsedTime / 0.2f;	// 1->0으로 변화
				if (fT > 1.f) fT = 1.f;
				if (fT < 0.f) fT = 0.f;
				SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(255 * (1.f - fT)), 255, 255, 255));
			}
		}
	}

	return 0;
}

void CUI_BloodScreen::Render_Object(void)
{
	CUI_Image::Render_Object();
}

CUI_BloodScreen * CUI_BloodScreen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_BloodScreen*	pInstance = new CUI_BloodScreen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_BloodScreen::Free(void)
{
	CUI_Image::Free();
}

void CUI_BloodScreen::StartBloodScreen()
{
	m_fElapsedTime = 0.f;
	m_bIsFadeIn = false;
	m_bIsProcessing = true;
	SetRenderColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	SetActivated(true);
}
