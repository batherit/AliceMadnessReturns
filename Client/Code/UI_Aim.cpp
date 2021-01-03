#include "pch.h"
#include "UI_Aim.h"
#include "UI_Image.h"
#include "Attribute.h"

CUI_Aim::CUI_Aim(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_Aim::CUI_Aim(const CUI_Aim & rhs)
	:
	CGameObject(rhs)
{
}

CUI_Aim::~CUI_Aim(void)
{
}

HRESULT CUI_Aim::Ready_Object(void)
{
	m_pAimImage = CUI_Image::Create(m_pGraphicDev);
	m_pAimImage->SetTexture(L"UI_Aim");
	m_pAimImage

	m_pBackImage = CUI_Image::Create(m_pGraphicDev);
	m_pBackImage->SetTexture(L"UI_HP_Branch");
	m_pBackImage->SetOutputArea(RECT{ OFFSET_X, OFFSET_Y, m_pBackImage->GetWidth() + OFFSET_X, m_pBackImage->GetHeight() + OFFSET_Y });
	AddChild(m_pBackImage);

	return S_OK;
}

int CUI_Aim::Update_Object(const _float & _fDeltaTime)
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

void CUI_Aim::Render_Object(void)
{
}

CUI_Aim * CUI_Aim::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Aim*	pInstance = new CUI_Aim(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_Aim::Free(void)
{
	CGameObject::Free();
}

void CUI_Aim::SetPlayer(Engine::CGameObject * _pPlayer)
{
	if (!_pPlayer)
		return;

	m_pPlayer = _pPlayer;
}
