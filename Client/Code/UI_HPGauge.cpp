#include "pch.h"
#include "UI_HPGauge.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_HPGauge::CUI_HPGauge(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_HPGauge::CUI_HPGauge(const CUI_HPGauge & rhs)
	:
	CGameObject(rhs)
{
}

CUI_HPGauge::~CUI_HPGauge(void)
{
}

HRESULT CUI_HPGauge::Ready_Object(void)
{
	m_pBackImage = CUI_Image::Create(m_pGraphicDev);
	m_pBackImage->SetTexture(L"UI_HP_Branch");
	//m_pBackImage->SetExtractionArea(RECT{ 0, 0, WINCX >> 1, WINCY >> 1 });
	m_pBackImage->SetPos(HPGAUGE_OFFSET_X + (m_pBackImage->GetWidth() >> 1), HPGAUGE_OFFSET_Y + (m_pBackImage->GetHeight() >> 1));
	//m_pBackImage->SetOutputArea(RECT{ OFFSET_X, OFFSET_Y, m_pBackImage->GetWidth() + OFFSET_X, m_pBackImage->GetHeight() + OFFSET_Y });
	AddChild(m_pBackImage);

	CUI_Image *pImage = nullptr;
	for (_int i = 0; i < 8; ++i) {
		pImage = CUI_Image::Create(m_pGraphicDev);
		m_vecPiece.emplace_back(pImage);
		pImage->SetTexture(L"UI_HP");
		pImage->SetPos(
			static_cast<LONG>((120.f  + 18.f * i) - m_pBackImage->GetWidth()* 0.5f) , 
			static_cast<LONG>((i % 2 == 0 ? 53.f : 18.f) - m_pBackImage->GetHeight() * 0.5f));
		pImage->GetTransform()->SetScaleXYZ(0.7f, 0.7f, 1.f);
		pImage->SetTextureIndex(0);
		pImage->SetActivated(false);
		m_pBackImage->AddChild(pImage);
	}

	return S_OK;
}

int CUI_HPGauge::Update_Object(const _float & _fDeltaTime)
{
	// m_fCurHP °»½Å
	
	if (m_pPlayer) {
		_int iOldIndex = static_cast<_int>(m_fCurHP - 1.f);// Engine::Clamp(m_fCurHP - 1.f, 0.f, m_pPlayer->GetComponent<CAttribute>()->GetMaxHP() - 1.f);
		
		_float fDelta = m_pPlayer->GetComponent<CAttribute>()->GetHP() - m_fCurHP;

		if (fDelta > 0.f) {
			fDelta = 0.5f;
		}
		else if (fDelta < 0.f) {
				fDelta = -0.5f;
		}
		Engine::Clamp(&(m_fCurHP += fDelta), 0.f, m_pPlayer->GetComponent<CAttribute>()->GetMaxHP());
		_int iIndex = static_cast<_int>(m_fCurHP - 1.f);
		if (iIndex >= 0) {
			if (iOldIndex / 8 < iIndex / 8) {
				m_vecPiece[iIndex / 8]->SetActivated(true);
			}
			else if (iOldIndex / 8 > iIndex / 8) {
				m_vecPiece[iOldIndex / 8]->SetActivated(false);
			}
			else {
				if (iOldIndex == 0 && fDelta > 0.f) {
					m_vecPiece[0]->SetActivated(true);
				}
				else if (iIndex == 0 && fDelta < 0.f) {
					m_vecPiece[0]->SetActivated(false);
				}
			}
			m_vecPiece[iIndex / 8]->SetTextureIndex(iIndex % 8);
		}
	}

	CGameObject::Update_Object(_fDeltaTime);
	return 0;
}

void CUI_HPGauge::Render_Object(void)
{
}

CUI_HPGauge * CUI_HPGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_HPGauge*	pInstance = new CUI_HPGauge(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_HPGauge::Free(void)
{
	CGameObject::Free();
}

void CUI_HPGauge::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer) 
		return; 
	
	m_pPlayer = _pPlayer; 
}
