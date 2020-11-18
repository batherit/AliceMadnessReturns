#include "MeshCollider.h"

USING(Engine)
Engine::CMeshCollider::CMeshCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)	
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CMeshCollider::CMeshCollider(const CMeshCollider& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
{
	for (_uint i = 0; i < COL_END; ++i)
		m_pTexture[i] = rhs.m_pTexture[i];
	
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

Engine::CMeshCollider::~CMeshCollider(void)
{

}

HRESULT Engine::CMeshCollider::Ready_MeshCollider(const _vec3* pVtxPos, const _ulong& dwNumVtxCnt, const _ulong& dwStride)
{
	// 인자로 받아들인 메쉬에 맞는 사이즈의 박스를 생성
	D3DXComputeBoundingBox(pVtxPos, dwNumVtxCnt, sizeof(_vec3), &m_vMin, &m_vMax);


	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0, // 정적버퍼로 할당하겠다는 옵션
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0, // 정적버퍼로 할당하겠다는 옵션
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL),
		E_FAIL);

	VTXCUBE*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, NULL);

	// 전면
	pVertex[0].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVertex[0].vTexUV = pVertex[0].vPos;

	pVertex[1].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVertex[1].vTexUV = pVertex[1].vPos;

	pVertex[2].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVertex[2].vTexUV = pVertex[2].vPos;

	pVertex[3].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVertex[3].vTexUV = pVertex[3].vPos;

	// 후면
	pVertex[4].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVertex[4].vTexUV = pVertex[4].vPos;

	pVertex[5].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVertex[5].vTexUV = pVertex[5].vPos;

	pVertex[6].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVertex[6].vTexUV = pVertex[6].vPos;

	pVertex[7].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVertex[7].vTexUV = pVertex[7].vPos;

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// x+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// x-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}
	

	return S_OK;
}

Engine::CMeshCollider* Engine::CMeshCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pVtxPos, const _ulong& dwNumVtxCnt, const _ulong& dwStride)
{
	CMeshCollider*	pInstance = new CMeshCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_MeshCollider(pVtxPos, dwNumVtxCnt, dwStride)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CMeshCollider::Render_MeshCollider(COLLTYPE eType, const _matrix* pMeshColliderMatrix)
{
	m_matCollMatrix = *pMeshColliderMatrix;

#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pMeshColliderMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);

	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pGraphicDev->SetFVF(FVF_CUBE);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif


}

CComponent * CMeshCollider::Clone(void)
{
	return new CMeshCollider(*this);
}

void Engine::CMeshCollider::Free(void)
{
	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pGraphicDev);
}

HRESULT CMeshCollider::Ready(void)
{
	return E_NOTIMPL;
}

int CMeshCollider::Update(const _float & _fDeltaTime)
{
	return 0;
}

