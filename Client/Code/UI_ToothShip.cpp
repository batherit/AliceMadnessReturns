#include "pch.h"
#include "UI_ToothShip.h"
#include "Ship.h"

CUI_ToothShip::CUI_ToothShip(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
}

CUI_ToothShip::CUI_ToothShip(const CUI_ToothShip & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_ToothShip::~CUI_ToothShip(void)
{
}

HRESULT CUI_ToothShip::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"UI_WeaponLock_Tooth");
	SetPos(TOOTHSHIP_OFFSET_X + (GetWidth() >> 1), TOOTHSHIP_OFFSET_Y + (GetHeight() >> 1));

	return S_OK;
}

int CUI_ToothShip::Update_Object(const _float & _fDeltaTime)
{
	CUI_Image::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_ToothShip::Render_Object(void)
{
	CUI_Image::Render_Object();

	if (!m_pShip)
		return;
	_vec3 vPos = GetTransform()->GetPos();
	_tchar tcBuffer[20];
	wsprintf(tcBuffer, L"%d", m_pShip->GetToothNum());
	Engine::Render_Font(L"Font_Jinji", tcBuffer, &_vec2(vPos.x - 15.f, vPos.y + 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.5f, 0.5f));
}

CUI_ToothShip * CUI_ToothShip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_ToothShip*	pInstance = new CUI_ToothShip(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_ToothShip::Free(void)
{
	CUI_Image::Free();
}

