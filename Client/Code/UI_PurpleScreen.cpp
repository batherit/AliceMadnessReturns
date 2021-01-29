#include "pch.h"
#include "UI_PurpleScreen.h"

CUI_PurpleScreen::CUI_PurpleScreen(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{

}

CUI_PurpleScreen::CUI_PurpleScreen(const CUI_PurpleScreen & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_PurpleScreen::~CUI_PurpleScreen()
{
}

HRESULT CUI_PurpleScreen::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"UI_Layer");
	SetRenderColor(D3DCOLOR_ARGB(40, 139, 0, 255));
	SetOutputArea(0, 0, WINCX, WINCY);
	SetActivated(false);

	return S_OK;
}

_int CUI_PurpleScreen::Update_Object(const _float & _fDeltaTime)
{
	//if (!m_bIsEffectStarting) return 0;
	if (1 == CUI_Image::Update_Object(_fDeltaTime))
		return 1;

	return 0;
}

void CUI_PurpleScreen::Render_Object(void)
{
	CUI_Image::Render_Object();
}

CUI_PurpleScreen * CUI_PurpleScreen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_PurpleScreen*	pInstance = new CUI_PurpleScreen(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_PurpleScreen::Free(void)
{
	CUI_Image::Free();
}

void CUI_PurpleScreen::On()
{
	SetActivated(true);
}

void CUI_PurpleScreen::Off()
{
	SetActivated(false);
}
