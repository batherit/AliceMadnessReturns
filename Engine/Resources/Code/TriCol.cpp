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
	//	0, // 정적버퍼로 할당하겠다는 옵션
	//	FVF_COL,
	//	D3DPOOL_MANAGED,
	//	&m_pVB,
	//	NULL),
	//	E_FAIL);

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	// 1인자 : 어디서부터 잠글 것인가
	// 2인자 : 숫자가 0이면 전체 영역을 잠근다.
	// 3인자 : 인자값을 통해서 버텍스 버퍼 내 정점 중 첫번째 주소를 얻어온다.
	// 4인자 : 잠그는 형태를 묻는 인자, 정적 버퍼인 경우 0

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
//	// 그래픽 디바이스 장치에게 현재 내가 그리려는 버퍼를 링크시켜주는 함수
//	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
//	// 1인자 : 몇 번 슬롯에 보관할 것인가
//	// 2인자 : 어떤 것을 넘겨줄 것인가
//	// 3인자 : 어디서부터 그릴 것인가
//	// 4인자 : 어떤 단위로 표현할 것인가
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

