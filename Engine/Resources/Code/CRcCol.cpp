#include "CRcCol.h"

USING(Engine)
Engine::CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcCol::CRcCol(const CRcCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcCol::~CRcCol(void)
{

}

HRESULT Engine::CRcCol::Ready_Buffer(void)
{
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXCOL);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	// ���� ���� ����
	//FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCOL) * 4,
	//	0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
	//	FVF_COL,
	//	D3DPOOL_MANAGED,
	//	&m_pVB,
	//	NULL),
	//	E_FAIL);

	// �ε��� ���� ����
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 0

	pVertex[0].vPos = _vec3(-1.f, 1.f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPos = _vec3(1.f, 1.f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPos = _vec3(1.f, -1.f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	pVertex[3].vPos = _vec3(-1.f, -1.f, 0.f);
	pVertex[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX16 indices[] =
	{
		{ 0, 1, 2 }, { 0, 2, 3 }
	};

	VOID* pIndices;
	m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0);
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	return S_OK;
}

void CRcCol::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	m_pGraphicDev->SetFVF(FVF_COL);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}


Engine::CRcCol* Engine::CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCol*	pInstance = new CRcCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))		// ���� �����ϰ� ���� ������ �����Ѵ�.
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CRcCol::Clone(void)
{
	return new CRcCol(*this);
}

void Engine::CRcCol::Free(void)
{
	CVIBuffer::Free();
}
