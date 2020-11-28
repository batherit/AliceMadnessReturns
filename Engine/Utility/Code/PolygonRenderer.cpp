#include "PolygonRenderer.h"
#include "Texture.h"		// Reference
#include "VIBuffer.h"		// Reference
#include "Transform.h"


USING(Engine)

CPolygonRenderer::CPolygonRenderer(void)
{
}

CPolygonRenderer::CPolygonRenderer(const CPolygonRenderer & rhs)
{
}

CPolygonRenderer::~CPolygonRenderer(void)
{
}

HRESULT CPolygonRenderer::Ready(void)
{
	return S_OK;
}

int CPolygonRenderer::Update(const _float & _fDeltaTime)
{
	RegisterToRenderer();
	return 0;
}

void CPolygonRenderer::Render()
{
	if (m_pBuffer && m_eRenderID < RENDERID::RENDER_END) {
		m_pOwner->GetGraphicDev()->SetTransform(D3DTS_WORLD, &m_pOwner->GetTransform()->GetObjectMatrix());
		if (m_pTexture)
			m_pTexture->Render_Texture(m_uiTextureIndex);
		else
			m_pOwner->GetGraphicDev()->SetTexture(0, NULL);

		m_pBuffer->Render_Buffer();
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

void CPolygonRenderer::SetRenderInfo(RENDERID _eRenderID, CVIBuffer * _pBuffer, CTexture * _pTexture)
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
