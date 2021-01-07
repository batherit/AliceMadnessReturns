#include "pch.h"
#include "UI_FadeInOut.h"

CUI_FadeInOut::CUI_FadeInOut(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CUI_Image(pGraphicDev)
{
	
}

CUI_FadeInOut::CUI_FadeInOut(const CUI_FadeInOut & rhs)
	:
	CUI_Image(rhs)
{
}

CUI_FadeInOut::~CUI_FadeInOut()
{
}

HRESULT CUI_FadeInOut::Ready_Object(void)
{
	CUI_Image::Ready_Object();
	SetTexture(L"UI_Layer");
	SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));
	SetOutputArea(0, 0, WINCX, WINCY);

	return S_OK;
}

_int CUI_FadeInOut::Update_Object(const _float & _fDeltaTime)
{
	//if (!m_bIsEffectStarting) return 0;
	if (1 == CUI_Image::Update_Object(_fDeltaTime))
		return 1;

	if ((m_fKeepTime -= _fDeltaTime) > 0.f) {
		float fT = m_fKeepTime / m_fMaxKeepTime;	// 1->0으로 변화
		if (fT > 1.f) fT = 1.f;
		if (fT < 0.f) fT = 0.f;
		if (m_bIsFadeIn) {
			// 점점 밝아지는
			SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(255 * fT), 0, 0, 0));
		}
		else {
			// 점점 어두워지는
			SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(255 * (1.f - fT)), 0, 0, 0));
		}
	}
	else {
		if (m_bIsFadeIn) {
			// 점점 밝아지는
			SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));
			SetActivated(false);
		}
		else {
			// 점점 어두워지는
			SetRenderColor(D3DCOLOR_ARGB(255, 0, 0, 0));
		}
		m_fKeepTime = 0.f;
		//m_bIsEffectStarting = false;

		// 효과가 종료되었을때 1을 반환한다.
		return 1;
	}

	return 0;
}

void CUI_FadeInOut::Render_Object(void)
{
	CUI_Image::Render_Object();
}

CUI_FadeInOut * CUI_FadeInOut::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_FadeInOut*	pInstance = new CUI_FadeInOut(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_FadeInOut::Free(void)
{
	CUI_Image::Free();
}

void CUI_FadeInOut::StartFadeInOut(_float _fKeepTime, _bool _bIsFadeIn)
{
	if (_fKeepTime <= 0.f) return;
	
	m_fKeepTime = _fKeepTime;
	m_fMaxKeepTime = _fKeepTime;
	m_bIsFadeIn = _bIsFadeIn;

	if (m_bIsFadeIn)
		// 점점 밝아지는 => 게임 진입 시도 //타이틀 씬
		SetRenderColor(D3DCOLOR_ARGB(255, 0, 0, 0));
	else
		// 점점 어두워지는 => 게임 진입 완료 // 플레이 씬
		SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));

	SetActivated(true);
	//m_bIsEffectStarting = true;
}
