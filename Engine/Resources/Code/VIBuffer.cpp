#include "VIBuffer.h"

USING(Engine)
Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwVtxSize(0)
	, m_dwFVF(0)
	, m_dwIdxSize(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CResources(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

//HRESULT Engine::CVIBuffer::Ready_Buffer(void)
//{
//	return S_OK;
//}
//
//void Engine::CVIBuffer::Render_Buffer(void)
//{
//}
//
//void Engine::CVIBuffer::Free(void)
//{
//	Safe_Release(m_pVB);
//	Safe_Release(m_pIB);
//
//	CResources::Free();
//}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt,
		0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0, // �������۷� �Ҵ��ϰڴٴ� �ɼ�
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// �׷��� ����̽� ��ġ���� ���� ���� �׸����� ���۸� ��ũ�����ִ� �Լ�
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// 1���� : �� �� ���Կ� ������ ���ΰ�
	// 2���� : � ���� �Ѱ��� ���ΰ�
	// 3���� : ��𼭺��� �׸� ���ΰ�
	// 4���� : � ������ ǥ���� ���ΰ�

	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void Engine::CVIBuffer::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CResources::Free();
}
