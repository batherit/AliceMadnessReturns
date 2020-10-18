#include "TriCol.h"

USING(Engine)
Engine::CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CTriCol::CTriCol(const CTriCol& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CTriCol::~CTriCol(void)
{

}

HRESULT Engine::CTriCol::Ready_Buffer(void)
{
	//FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_dwFVF = FVF_COL;
	m_dwTriCnt = 1;
	m_dwVtxCnt = 3;
	m_dwVtxSize = sizeof(VTXCOL);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);
	//FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCOL) * 3,
	//	0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
	//	FVF_COL,
	//	D3DPOOL_MANAGED,
	//	&m_pVB,
	//	NULL),
	//	E_FAIL);

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1���� : ��𼭺��� ��� ���ΰ�
	// 2���� : ���ڰ� 0�̸� ��ü ������ ��ٴ�.
	// 3���� : ���ڰ��� ���ؼ� ���ؽ� ���� �� ���� �� ù��° �ּҸ� ���´�.
	// 4���� : ��״� ���¸� ���� ����, ���� ������ ��� 0

	pVertex[0].vPos = _vec3(0.f, 1.f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPos = _vec3(1.f, -1.f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPos = _vec3(-1.f, -1.f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

//void CTriCol::Render_Buffer(void)
//{
//	// �׷��� ����̽� ��ġ���� ���� ���� �׸����� ���۸� ��ũ�����ִ� �Լ�
//	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
//	// 1���� : �� �� ���Կ� ������ ���ΰ�
//	// 2���� : � ���� �Ѱ��� ���ΰ�
//	// 3���� : ��𼭺��� �׸� ���ΰ�
//	// 4���� : � ������ ǥ���� ���ΰ�
//
//	m_pGraphicDev->SetFVF(FVF_COL);
//	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
//}


Engine::CTriCol* Engine::CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol*	pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTriCol::Clone(void)
{
	return new CTriCol(*this);
}

void Engine::CTriCol::Free(void)
{
	CVIBuffer::Free();
}

