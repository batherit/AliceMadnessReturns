#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev),
	m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->CombinedTransformationMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData, 
															CONST D3DXMATERIAL *pMaterials, 
															CONST D3DXEFFECTINSTANCE *pEffectInstances, 
															DWORD NumMaterials, 
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo, 
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH		pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// 메쉬가 지닌 폴리곤 개수를 반환

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF();

	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// fvf 정보만 채워준 채 메쉬 객체를 생성
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

		// 각 폴리곤 또는 정점 기준의 법선을 계산하여 삽입해주는 함수
		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
	}

	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			TCHAR		szFullPath[256] = L"";
			TCHAR		szFileName[256] = L"";

			MultiByteToWideChar(CP_ACP, 
								0, 
								pDerivedMeshContainer->pMaterials[i].pTextureFilename, 
								strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename),
								szFileName, 
								256);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i]), E_FAIL);
		}
	}
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), pDerivedMeshContainer->MeshData.pMesh->GetFVF(), m_pGraphicDev, &pDerivedMeshContainer->pOriMesh);

	// 메쉬 컨테이너가 영향을 받는 뼈의 개수를 반환하는 함수
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);	// 메쉬를 그리기 위한 뼈대들은 고유의 인덱스 값을 가지고 있음. 그래서 순회하면서 행렬 정보를 차례대로 얻어올 수 있다.
	
	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;
	
	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		Safe_Release(pMeshContainer->ppTexture[i]);

	Safe_Delete_Array(pMeshContainer->ppTexture);
	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pAdjacency);
	Safe_Delete_Array(pMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrix);
	
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pOriMesh);
	Safe_Release(pMeshContainer->pSkinInfo);

	Safe_Delete(pMeshContainer);

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(char ** ppName, const char * pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint		iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);

}

CHierarchyLoader * CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath)
{
	return new CHierarchyLoader(pGraphicDev, pPath);
}

_ulong CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

