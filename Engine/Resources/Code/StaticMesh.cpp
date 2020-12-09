#include "StaticMesh.h"
#include "Transform.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubset(rhs.m_pSubset)
	, m_pMesh(rhs.m_pMesh)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pMtrl(rhs.m_pMtrl)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_dwNumVtx(rhs.m_dwNumVtx)	
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwStride(rhs.m_dwStride)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		Safe_AddRef(m_ppTextures[i]);
	}
	
	Safe_AddRef(m_pMesh);
	Safe_AddRef(m_pOriMesh);
	Safe_AddRef(m_pAdjacency);
	Safe_AddRef(m_pSubset);
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar		szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev,
		&m_pAdjacency, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pOriMesh)))
		return E_FAIL;

	_ulong	dwFVF = m_pOriMesh->GetFVF();	// 메쉬가 지닌 정점 FVF정보를 얻어오는 함수

	// 노말 값이 없는 경우
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	void*		pVertex = nullptr;

	m_dwNumVtx = m_pMesh->GetNumVertices();	// 메쉬가 지닌 정점의 개수를 반환
	m_pVtxPos = new _vec3[m_dwNumVtx];
	
	m_pMesh->LockVertexBuffer(0, &pVertex);

	// 정점 정보 중 position의 위치 찾기
	D3DVERTEXELEMENT9			Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if(Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}
	// FVF 정보를 토대로 정점의 크기를 반환하는 함수
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffset)));
	}
	
	m_pMesh->UnlockVertexBuffer();

	// 메쉬가 지닌 재질 정보 중 첫 번째 주소를 반환하여 저장
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 
			0,
			m_pMtrl[i].pTextureFilename,
			strlen(m_pMtrl[i].pTextureFilename), 
			szFileName, 
			256);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
			return E_FAIL;
	}

	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(LPD3DXEFFECT _pEffect)
{
	if(!_pEffect)
		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
			m_pMesh->DrawSubset(i);
		}
	else {
		for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		{
			//_uint iMaxPass = 0;
		//	_pEffect->BeginPass(0);

		//	_matrix	matView, matProj;
		////	m_pOwner->GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
		//	m_pOwner->GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

			// 렌더러는 기본적으로 월드, 뷰, 프로젝션까지는 세팅해준다.
			// 재질이나 조명같은 것은 렌더러 외부에서 미리 세팅해야 한다.
		//	_pEffect->SetMatrix("g_matWorld", &m_pOwner->GetTransform()->GetObjectMatrix());
		//	_pEffect->SetMatrix("g_matView", &matView);
		//	_pEffect->SetMatrix("g_matProj", &matProj);
			_pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
			m_pMesh->DrawSubset(i);

		//	_pEffect->EndPass();
			
			//m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
			

		}
	}
}


Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);


	Safe_Release(m_pSubset);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);

	CMesh::Free();
}
