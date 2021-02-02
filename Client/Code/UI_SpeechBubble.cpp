#include "pch.h"
#include "UI_SpeechBubble.h"

CUI_SpeechBubble::CUI_SpeechBubble(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_SpeechBubble::CUI_SpeechBubble(const CUI_SpeechBubble & rhs)
	:
	CGameObject(rhs)
{
}

CUI_SpeechBubble::~CUI_SpeechBubble()
{
}

HRESULT CUI_SpeechBubble::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	return S_OK;
}

_int CUI_SpeechBubble::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (IsVisible()) {
		if ((m_fElapsedTime += _fDeltaTime) > m_fLifeTime) {
			SetVisible(false);
			m_fElapsedTime = 0.f;
			return 1;
		}
		m_pRenderer->Add_RenderGroup(Engine::RENDER_UI, this);
	}

	return 0;
}

void CUI_SpeechBubble::Render_Object(void)
{
	if (!IsVisible())
		return;

	//_vec3 vPos = m_pToothImage->GetTransform()->GetPos();
	//_tchar tcBuffer[20];
	//wsprintf(tcBuffer, L"%s", m_wstrText);
	Engine::Render_Font(L"Font_Jinji", m_wstrText.c_str(), &(m_vOutputPos + _vec2(2.f, 2.f)), D3DXCOLOR(0.f, 0.f, 0.f, 1.f), &_vec2(0.3f, 0.5f));
	Engine::Render_Font(L"Font_Jinji", m_wstrText.c_str(), &m_vOutputPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), &_vec2(0.3f, 0.5f));
}

CUI_SpeechBubble * CUI_SpeechBubble::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_SpeechBubble*	pInstance = new CUI_SpeechBubble(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_SpeechBubble::Free(void)
{
	CGameObject::Free();
}

void CUI_SpeechBubble::SetText(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText, _float _fLifeTime)
{
	if (_wstrText == L"") return;

	m_vOutputPos = _vec2(_fOffsetX, _fOffsetY);
	m_wstrText = _wstrText;
	m_fLifeTime = _fLifeTime;
	m_fElapsedTime = 0.f;
	SetVisible(true);
}
