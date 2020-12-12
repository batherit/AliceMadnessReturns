#include "PolygonRenderer.h"
#include "Texture.h"		// Reference
#include "VIBuffer.h"		// Reference
#include "Transform.h"


USING(Engine)

CPolygonRenderer::CPolygonRenderer(void)
{
}

CPolygonRenderer::CPolygonRenderer(const CPolygonRenderer & rhs)
	:
	m_matWorld(rhs.m_matWorld)
{
}

CPolygonRenderer::~CPolygonRenderer(void)
{
}

HRESULT CPolygonRenderer::Ready(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	return S_OK;
}

_int CPolygonRenderer::Update(const _float & _fDeltaTime)
{
	RegisterToRenderer();
	return 0;
}

void CPolygonRenderer::Render(LPD3DXEFFECT _pEffect, _uint _uiPassIndex)
{
	if (!m_pBuffer || m_eRenderID >= RENDERID::RENDER_END)
		return;

	if (!_pEffect) {
		m_pOwner->GetGraphicDev()->SetTransform(D3DTS_WORLD, &m_matWorld);
		if (m_pTexture)
			// 텍스처가 있는 경우
			m_pTexture->Render_Texture(m_uiTextureIndex);
		else
			// 텍스처가 없는 경우
			m_pOwner->GetGraphicDev()->SetTexture(0, NULL);

		m_pBuffer->Render_Buffer();
	}
	else {
		Engine::Safe_AddRef(_pEffect);
		_uint iMaxPass = 0;

		_pEffect->Begin(&iMaxPass, 0);
		_pEffect->BeginPass(_uiPassIndex);

		_matrix	matView, matProj;
		m_pOwner->GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
		m_pOwner->GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

		// 렌더러는 기본적으로 월드, 뷰, 프로젝션까지는 세팅해준다.
		// 재질이나 조명같은 것은 렌더러 외부에서 미리 세팅해야 한다.
		_pEffect->SetMatrix("g_matWorld", &m_matWorld);
		_pEffect->SetMatrix("g_matView", &matView);
		_pEffect->SetMatrix("g_matProj", &matProj);

		if (m_pTexture)
			// 텍스처가 있는 경우 
			m_pTexture->Set_Texture(_pEffect, "g_BaseTexture");
		else
			// 텍스처가 없는 경우
			_pEffect->SetTexture("g_BaseTexture", NULL);

		m_pBuffer->Render_Buffer();

		_pEffect->EndPass();
		_pEffect->End();
		Engine::Safe_Release(_pEffect);
	}
}

CPolygonRenderer * CPolygonRenderer::Create()
{
	CPolygonRenderer*	pInstance = new CPolygonRenderer();

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CPolygonRenderer::Clone()
{
	return new CPolygonRenderer(*this);
}

void CPolygonRenderer::SetRenderInfo(RENDERID _eRenderID, CVIBuffer * _pBuffer, CTexture * _pTexture, LPD3DXEFFECT _pEffect)
{
	if (_eRenderID >= RENDERID::RENDER_END || !_pBuffer)
		return;

	SetRenderID(_eRenderID);

	Safe_Release(m_pBuffer);
	m_pBuffer = _pBuffer;
	Safe_AddRef(m_pBuffer);

	Safe_Release(m_pTexture);
	m_pTexture = _pTexture;
	Safe_AddRef(m_pTexture);
}

void CPolygonRenderer::Free(void)
{
	Safe_Release(m_pBuffer);
	Safe_Release(m_pTexture);
}
