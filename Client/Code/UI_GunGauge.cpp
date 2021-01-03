#include "pch.h"
#include "UI_GunGauge.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_GunGauge::CUI_GunGauge(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_GunGauge::CUI_GunGauge(const CUI_GunGauge & rhs)
	:
	CGameObject(rhs)
{
}

CUI_GunGauge::~CUI_GunGauge(void)
{
}

HRESULT CUI_GunGauge::Ready_Object(void)
{
	m_pGunGauge = CUI_Image::Create(m_pGraphicDev);
	m_pGunGauge->SetTexture(L"UI_GunGauge");
	m_pGunGauge->SetPos(WINCX - (m_pGunGauge->GetWidth() >> 1) - OFFSET_X, (m_pGunGauge->GetHeight() >> 1) + OFFSET_Y);
	AddChild(m_pGunGauge);

	m_pGunCover = CUI_Image::Create(m_pGraphicDev);
	m_pGunCover->SetTexture(L"UI_GunCover");
	m_pGunCover->SetPos(WINCX - (m_pGunCover->GetWidth() >> 1) - OFFSET_X, (m_pGunCover->GetHeight() >> 1) + OFFSET_Y);
	AddChild(m_pGunCover);

	m_pGunOverload = CUI_Image::Create(m_pGraphicDev);
	m_pGunOverload->SetTexture(L"UI_GunOverload");
	m_pGunOverload->SetPos(WINCX - (m_pGunOverload->GetWidth() >> 1) - OFFSET_X, (m_pGunOverload->GetHeight() >> 1) + OFFSET_Y);
	AddChild(m_pGunOverload);

	SetOverload(false);

	return S_OK;
}

int CUI_GunGauge::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (IsOverloaded() && (m_fElapsedTime -= _fDeltaTime) <= 0.f) {
		Init();
	}

	// 64 * 128
	m_pGunGauge->SetSyncExtractionArea(RECT{
		static_cast<LONG>(0),
		static_cast<LONG>(Engine::GetValueByWeight(m_fCurGauge, 94.f, 14.f)),
		static_cast<LONG>(64),
		static_cast<LONG>(128)
		});

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_GunGauge::Render_Object(void)
{
}

CUI_GunGauge * CUI_GunGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_GunGauge*	pInstance = new CUI_GunGauge(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_GunGauge::Free(void)
{
	CGameObject::Free();
}

void CUI_GunGauge::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
}

void CUI_GunGauge::Init()
{
	SetOverload(false);
	m_fElapsedTime = 0.f;
	m_fCurGauge = 0.f;
}

void CUI_GunGauge::SetOverload(_bool _bIsOverloaded)
{
	m_bIsOverloaded = _bIsOverloaded;
	m_pGunOverload->SetActivated(_bIsOverloaded);
	if (_bIsOverloaded) {
		m_fElapsedTime = 2.f;
	}
}
