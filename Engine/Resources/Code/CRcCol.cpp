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

	// 정점 버퍼 생성
	//FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCOL) * 4,
	//	0, // 정적버퍼로 할당하겠다는 옵션
	//	FVF_COL,
	//	D3DPOOL_MANAGED,
	//	&m_pVB,
	//	NULL),
	//	E_FAIL);

	// 인덱스 버퍼 생성
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

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

	if (FAILED(pInstance->Ready_Buffer()))		// 버퍼 생성하고 정점 정보를 세팅한다.
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
