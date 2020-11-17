#include "SphereCol.h"

USING(Engine)
Engine::CSphereCol::CSphereCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CSphereCol::CSphereCol(const CSphereCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CSphereCol::~CSphereCol(void)
{

}

HRESULT Engine::CSphereCol::Ready_Buffer(void)
{
	D3DXCreateSphere(m_pGraphicDev, 1.f, 10, 10, &m_pSphereMesh, NULL);
	m_pSphereMesh->CloneMeshFVF(0, FVF_COL, m_pGraphicDev, &m_pClone);

	if (SUCCEEDED(m_pClone->GetVertexBuffer(&m_pVB))) {
		_int iNumVertices = m_pClone->GetNumVertices();
		
		VTXCOL*		pVertices = NULL;
		m_pVB->Lock(0, 0, (void**)&pVertices, NULL);
		for (_int i = 0; i < iNumVertices; ++i) {
			pVertices[i].dwColor = D3DCOLOR_ARGB(200, 200, 200, 200);
		}
		m_pVB->Unlock();
	}

	return S_OK;
}

void CSphereCol::Render_Buffer(void)
{
	//m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	//m_pGraphicDev->SetFVF(FVF_COL);
	//m_pGraphicDev->SetIndices(m_pIB);
	//m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pClone->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}


Engine::CSphereCol* Engine::CSphereCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereCol*	pInstance = new CSphereCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// 버퍼 생성하고 정점 정보를 세팅한다.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CSphereCol::Clone(void)
{
	return new CSphereCol(*this);
}

void Engine::CSphereCol::Free(void)
{
	Safe_Release(m_pSphereMesh);
	Safe_Release(m_pClone);
	CVIBuffer::Free();
}
