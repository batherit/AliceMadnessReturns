#include "MeshRenderer.h"
#include "Mesh.h"			// Reference
#include "Transform.h"

USING(Engine)


CMeshRenderer::CMeshRenderer(void)
{
}

CMeshRenderer::CMeshRenderer(const CMeshRenderer & rhs)
{
}

CMeshRenderer::~CMeshRenderer(void)
{
}

HRESULT CMeshRenderer::Ready(void)
{
	return S_OK;
}

int CMeshRenderer::Update(const _float & _fDeltaTime)
{
	RegisterToRenderer();
	return 0;
}

void CMeshRenderer::Render()
{
	if (m_pMesh && m_eRenderID < RENDERID::RENDER_END) {
		m_pOwner->GetGraphicDev()->SetTransform(D3DTS_WORLD, &m_pOwner->GetTransform()->GetObjectMatrix());
		m_pMesh->Render_Meshes();
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
