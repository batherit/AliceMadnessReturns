#include "RcTex.h"

USING(Engine)
Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcTex::~CRcTex(void)
{

}

HRESULT Engine::CRcTex::Ready_Buffer(void)
{
	m_dwFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 6;
	m_dwVtxSize = sizeof(VTXTEX);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);
	
	// 오른쪽 위
	pVertex[0].vPos = _vec3(-1.f, 1.f, 0.f);
//	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(1.f, 1.f, 0.f);
	//pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(1.f, -1.f, 0.f);
	//pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);


	// 왼쪽 아래
	pVertex[3].vPos = _vec3(-1.f, 1.f, 0.f);
	//pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 0.f);

	pVertex[4].vPos = _vec3(1.f, -1.f, 0.f);
	//pVertex[4].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	pVertex[4].vTexUV = _vec2(1.f, 1.f);

	pVertex[5].vPos = _vec3(-1.f, -1.f, 0.f);
	//pVertex[5].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	pVertex[5].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	return S_OK;
}


Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*	pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

void Engine::CRcTex::Free(void)
{
	CVIBuffer::Free();
}

