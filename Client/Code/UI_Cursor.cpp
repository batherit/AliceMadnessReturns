#include "pch.h"
#include "UI_Cursor.h"

CUI_Cursor::CUI_Cursor(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_Cursor::~CUI_Cursor(void)
{
}

HRESULT CUI_Cursor::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	m_pCursorImage = CUI_Image::Create(m_pGraphicDev);
	m_pCursorImage->SetTexture(L"UI_Cursor");
	AddChild(m_pCursorImage);

	m_iOffsetX = m_pCursorImage->GetWidth() >> 1;
	m_iOffsetY = m_pCursorImage->GetHeight() >> 1;

	return S_OK;
}

int CUI_Cursor::Update_Object(const _float & _fDeltaTime)
{
	CUI_Image::Update_Object(_fDeltaTime);

	POINT ptPos = Engine::GetClientCursorPoint(g_hWnd);
	GetTransform()->SetPos(static_cast<_float>(ptPos.x + m_iOffsetX), static_cast<_float>(ptPos.y + m_iOffsetY), 0.f);

	return 0;
}

void CUI_Cursor::Render_Object(void)
{
	CUI_Image::Render_Object();
}

CUI_Cursor * CUI_Cursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Cursor*	pInstance = new CUI_Cursor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_Cursor::Free(void)
{
	CUI_Image::Free();
}