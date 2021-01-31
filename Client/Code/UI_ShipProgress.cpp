#include "pch.h"
#include "UI_ShipProgress.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_ShipProgress::CUI_ShipProgress(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_ShipProgress::CUI_ShipProgress(const CUI_ShipProgress & rhs)
	:
	CGameObject(rhs)
{
}

CUI_ShipProgress::~CUI_ShipProgress(void)
{
}

HRESULT CUI_ShipProgress::Ready_Object(void)
{
	m_pProgress = CUI_Image::Create(m_pGraphicDev);
	m_pProgress->SetTexture(L"UI_Ship_Progress");
	AddChild(m_pProgress);

	m_pShip = CUI_Image::Create(m_pGraphicDev);
	m_pShip->SetTexture(L"UI_Ship");
	m_pShip->SetPos(-(m_pProgress->GetWidth() >> 1), -10);
	AddChild(m_pShip);

	return S_OK;
}

int CUI_ShipProgress::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_ShipProgress::Render_Object(void)
{
}

CUI_ShipProgress * CUI_ShipProgress::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_ShipProgress*	pInstance = new CUI_ShipProgress(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_ShipProgress::Free(void)
{
	CGameObject::Free();
}

void CUI_ShipProgress::UpdateProgress(_float _fProgress)
{
	m_pShip->SetPos(Engine::GetValueByWeight(_fProgress, -(m_pProgress->GetWidth() >> 1), (m_pProgress->GetWidth() >> 1)), -10);
}
