#include "pch.h"
#include "UI_Tooth.h"
#include "AliceW.h"

CUI_Tooth::CUI_Tooth(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
}

CUI_Tooth::CUI_Tooth(const CUI_Tooth & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_Tooth::~CUI_Tooth(void)
{
}

HRESULT CUI_Tooth::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"UI_WeaponLock_Tooth");
	SetPos(TOOTH_OFFSET_X + (GetWidth() >> 1), TOOTH_OFFSET_Y + (GetHeight() >> 1));

	return S_OK;
}

int CUI_Tooth::Update_Object(const _float & _fDeltaTime)
{
	CUI_Image::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_Tooth::Render_Object(void)
{
	CUI_Image::Render_Object();

	if (!m_pAliceW)
		return;
	_vec3 vPos = GetTransform()->GetPos();
	_tchar tcBuffer[20];
	wsprintf(tcBuffer, L"%d", m_pAliceW->GetToothNum());
	Engine::Render_Font(L"Font_Jinji", tcBuffer, &_vec2(vPos.x - 15.f, vPos.y + 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.5f, 0.5f));
}

CUI_Tooth * CUI_Tooth::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Tooth*	pInstance = new CUI_Tooth(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_Tooth::Free(void)
{
	CUI_Image::Free();
}

