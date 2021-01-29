#include "pch.h"
#include "UI_HPBar.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_HPBar::CUI_HPBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_HPBar::CUI_HPBar(const CUI_HPBar & rhs)
	:
	CGameObject(rhs)
{
}

CUI_HPBar::~CUI_HPBar(void)
{
}

HRESULT CUI_HPBar::Ready_Object(void)
{
	m_pBackground = CUI_Image::Create(m_pGraphicDev);
	m_pBackground->SetTexture(L"UI_HPBar_BG");
	m_pBackground->SetPos(WINCX >> 1, WINCY * 0.1f);
	//m_pBackground->SetOutputArea(RECT{(WINCX >> 1) - (m_pBackground->GetWidth() >> 1),});
	AddChild(m_pBackground);

	m_pHPGauge = CUI_Image::Create(m_pGraphicDev);
	m_pHPGauge->SetTexture(L"UI_HPBar");
	m_pHPGauge->SetPos(WINCX >> 1, WINCY * 0.1f);
	m_iWidth = m_pHPGauge->GetWidth();
	m_iHeight = m_pHPGauge->GetHeight();
	AddChild(m_pHPGauge);

	return S_OK;
}

int CUI_HPBar::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_pTargetAttribute) {
		UpdateHPInfo(m_pTargetAttribute->GetHP(), m_pTargetAttribute->GetMaxHP());
	}

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_HPBar::Render_Object(void)
{
}

CUI_HPBar * CUI_HPBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_HPBar*	pInstance = new CUI_HPBar(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_HPBar::Free(void)
{
	CGameObject::Free();
}

_bool CUI_HPBar::On(Engine::CGameObject * _pTargetObject)
{
	m_pTargetAttribute = _pTargetObject->GetComponent<CAttribute>();
	if (!m_pTargetAttribute) {
		_pTargetObject = nullptr;
		SetActivated(false);
		return false;
	}
	m_fSmoothGauge = m_pTargetAttribute->GetHP();
	m_pTargetObject = _pTargetObject;
	SetActivated(true);

	return true;
}

void CUI_HPBar::Off()
{
	m_pTargetObject = nullptr;
	m_pTargetAttribute = nullptr;
	SetActivated(false);
}

void CUI_HPBar::UpdateHPInfo(_float _fCurHP, _float _fMaxHP)
{
	m_fSmoothGauge += (_fCurHP - m_fSmoothGauge) * 0.1f;
	// 64 * 128
	m_pHPGauge->SetSyncExtractionArea(RECT{
		static_cast<LONG>(0),
		static_cast<LONG>(0),
		static_cast<LONG>(m_iWidth * m_fSmoothGauge/_fMaxHP),
		static_cast<LONG>(32)
		});

}