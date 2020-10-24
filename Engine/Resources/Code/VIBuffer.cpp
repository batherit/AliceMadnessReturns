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
		0, // 정적버퍼로 할당하겠다는 옵션
		m_dwFVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt,
		0, // 정적버퍼로 할당하겠다는 옵션
		m_IdxFmt,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	return S_OK;
}

void Engine::CVIBuffer::Render_Buffer(void)
{
	// 그래픽 디바이스 장치에게 현재 내가 그리려는 버퍼를 링크시켜주는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	// 1인자 : 몇 번 슬롯에 보관할 것인가
	// 2인자 : 어떤 것을 넘겨줄 것인가
	// 3인자 : 어디서부터 그릴 것인가
	// 4인자 : 어떤 단위로 표현할 것인가

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
