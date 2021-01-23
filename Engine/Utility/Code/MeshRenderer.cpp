#include "MeshRenderer.h"
#include "Mesh.h"			// Reference
#include "Transform.h"

USING(Engine)


CMeshRenderer::CMeshRenderer(void)
{
}

CMeshRenderer::CMeshRenderer(const CMeshRenderer & rhs)
	//:
	//m_matWorld(rhs.m_matWorld)
{
}

CMeshRenderer::~CMeshRenderer(void)
{
}

HRESULT CMeshRenderer::Ready(void)
{
	D3DXMatrixIdentity(&m_matWorld);

	return S_OK;
}

_int CMeshRenderer::Update(const _float & _fDeltaTime)
{
	RegisterToRenderer();
	return 0;
}

void CMeshRenderer::Render(LPD3DXEFFECT _pEffect, _uint _uiPassIndex)
{
	if (m_pMesh && m_eRenderID < RENDERID::RENDER_END) {
		if (!_pEffect) {
			m_pOwner->GetGraphicDev()->SetTransform(D3DTS_WORLD, &m_matWorld);
			if (!m_bIsViewCommited) {
				m_pOwner->GetGraphicDev()->SetTransform(D3DTS_VIEW, &m_matView);
				m_bIsViewCommited = true;
			}
			if (!m_bIsProjCommited) {
				m_pOwner->GetGraphicDev()->SetTransform(D3DTS_PROJECTION, &m_matProj);
				m_bIsProjCommited = true;
			}
			
			m_pMesh->Render_Meshes(_pEffect);
		}
		else {
			Engine::Safe_AddRef(_pEffect);
			_uint iMaxPass = 0;
			_pEffect->Begin(&iMaxPass, 0);
			
			
			_pEffect->SetMatrix("g_matWorld", &m_matWorld);
			if (!m_bIsViewCommited) {
				_pEffect->SetMatrix("g_matView", &m_matView);
				m_bIsViewCommited = true;
			}
			else {
				_matrix matView;
				m_pOwner->GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
				_pEffect->SetMatrix("g_matView", &matView);
			}

			if (!m_bIsProjCommited) {
				_pEffect->SetMatrix("g_matProj", &m_matProj);
				m_bIsProjCommited = true;
			}
			else {
				_matrix matProj;
				m_pOwner->GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
				_pEffect->SetMatrix("g_matProj", &matProj);
			}

			//// 렌더러는 기본적으로 월드, 뷰, 프로젝션까지는 세팅해준다.
			//// 재질이나 조명같은 것은 렌더러 외부에서 미리 세팅해야 한다.

			m_pMesh->Render_Meshes(_pEffect, _uiPassIndex);

			_pEffect->EndPass();
			_pEffect->End();
			Engine::Safe_Release(_pEffect);
		}
		
	}
}

void CMeshRenderer::SetRenderInfo(RENDERID _eRenderID, Engine::CMesh * _pMesh)
{
	if (_eRenderID >= RENDERID::RENDER_END || !_pMesh)
		return;

	SetRenderID(_eRenderID);

	Safe_Release(m_pMesh);
	m_pMesh = _pMesh;
	Safe_AddRef(m_pMesh);
}

CMeshRenderer * CMeshRenderer::Create()
{
	CMeshRenderer*	pInstance = new CMeshRenderer();

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CMeshRenderer::Clone()
{
	return new CMeshRenderer(*this);
}

void CMeshRenderer::Free(void)
{
	Safe_Release(m_pMesh);
}
