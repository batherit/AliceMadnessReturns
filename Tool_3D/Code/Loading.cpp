#include "pch.h"
#include "Loading.h"

#include "Export_Function.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CTerrainTab.h"
#include "CMapTab.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();		
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	/*m_mapMapObjects[L"Chapter1/"].emplace_back(L"CobbledStreet");
	m_mapMapObjects[L"Chapter1/"].emplace_back(L"CobbledStreet_Even");
	m_mapMapObjects[L"Chapter1/"].emplace_back(L"PoorBuildingE01");
	m_mapMapObjects[L"Chapter1/"].emplace_back(L"Curb_90");
	m_mapMapObjects[L"Chapter1/"].emplace_back(L"Curb_Uneven");
	m_mapMapObjects[L"Chapter1/"].emplace_back(L"Curb_Section");*/

	InitializeCriticalSection(&m_Crt);		// ũ��Ƽ�� ���� ����

	// ������ ���� �Լ�
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	// MapTab���� ���� ���� ������Ʈ�� �����Ѵ�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pTerrainTab = pTabForm->GetTerrainTab();
	m_pMapTab = pTabForm->GetMapTab();

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{


	lstrcpy(m_szLoading, L"Buffer Loading.............................");
	
	int i = 0;
	
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_SphereCol", Engine::BUFFER_SPHERECOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"M_Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												Engine::RESOURCE_STATIC,
												L"Buffer_TerrainTex",
												Engine::BUFFER_TERRAINTEX,
												VTXCNTX,
												VTXCNTZ,
												VTXITV),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												Engine::RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);



												// �ӽ� ���ҽ� �ε�.

	
	// ������Ʈ
	lstrcpy(m_szLoading, L"Registering Component.............................");
	// Ŭ���� ������Ʈ�� ������Ÿ�� �Ŵ����� ���/
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CTransform::GetComponentTag(), pComponent);

	// Renderer
	pComponent = Engine::CRenderer::GetInstance();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CRenderer::GetComponentTag(), pComponent);

	// PolygonRenderer
	pComponent = Engine::CPolygonRenderer::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CPolygonRenderer::GetComponentTag(), pComponent);

	// MeshRenderer
	pComponent = Engine::CMeshRenderer::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CMeshRenderer::GetComponentTag(), pComponent);

	// TerrainTex
	pComponent = Engine::CTerrainTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CTerrainTex::GetComponentTag(), pComponent);

	// ManualCol
	pComponent = Engine::CManualCol::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CManualCol::GetComponentTag(), pComponent);

	// SphereCollider
	pComponent = Engine::CSphereCollider::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CSphereCollider::GetComponentTag(), pComponent);
	
	// Physics
	pComponent = Engine::CPhysics::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CPhysics::GetComponentTag(), pComponent);

	// �ؽ���
	lstrcpy(m_szLoading, L"Texture Loading.............................");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Player", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg"), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												Engine::RESOURCE_STAGE,
												L"Texture_Terrain",
												Engine::TEX_NORMAL,
												L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
												E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Height", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Height2.bmp"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SkyBox", Engine::TEX_CUBE, L"../Bin/Resource/Texture/SkyBox/burger%d.dds", 4),  E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"CobbledStreet_DM", Engine::TEX_NORMAL, L"../Bin/Resource/Terrain/Texture/CobbledStreet_DM.tga"), E_FAIL);
	m_pTerrainTab->m_lbxTexture.AddString(L"CobbledStreet_DM");

	lstrcpy(m_szLoading, L"Mesh Loading.............................");
	

	// Stone �޽� ���� ����
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Mesh_Stone",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/TombStone/",
		L"TombStone.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"Mesh_Stone", NULL, NULL);

	// ���濡 �´� �޽� �ݶ��̴� ���� ����.
	Engine::CStaticMesh* pStaticMesh = nullptr;
	pStaticMesh = static_cast<Engine::CStaticMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	pComponent = Engine::CMeshCollider::Create(m_pGraphicDev, pStaticMesh->Get_VtxPos(), pStaticMesh->Get_NumVtx(), pStaticMesh->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(L"Collider_Stone", pComponent);

	//LoadMapObjectData();

	// CobbledStreet ����
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"CobbledStreet",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"CobbledStreet.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"CobbledStreet", NULL, NULL);

	// CobbledStreet_Even
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"CobbledStreet_Even",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"CobbledStreet_Even.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"CobbledStreet_Even", NULL, NULL);

	// PoorBuildingE01
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorBuildingE01",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"PoorBuildingE01.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"PoorBuildingE01", NULL, NULL);

	// Curb_90
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_90",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_90.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"Curb_90", NULL, NULL);

	// Curb_Uneven
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Uneven",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_Uneven.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"Curb_Uneven", NULL, NULL);

	// Curb_Section
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Section",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_Section.X"),
		E_FAIL);
	m_pMapTab->m_treeObjectList.InsertItem(L"Curb_Section", NULL, NULL);



	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Player",
	//	Engine::TYPE_DYNAMIC,
	//	L"../Bin/Resource/Mesh/DynamicMesh/PlayerXfile/",
	//	L"Player.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Tree",
	//	Engine::TYPE_STATIC,
	//	L"../Bin/Resource/Mesh/StaticMesh/Tree/",
	//	L"Tree01.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Navi",
	//	Engine::TYPE_NAVI,
	//	NULL,
	//	NULL),
	//	E_FAIL);

	
	lstrcpy(m_szLoading, L"Loading Complete!!!");

	m_bFinish = true;


	return 0;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

//HRESULT CLoading::LoadMapObjectData()
//{
//	for (auto& mapData : m_mapMapObjects) {
//		for (auto& tcStr : mapData.second) {
//			CString tcPath = L"../Bin/Resource/Mesh/StaticMesh/";
//			tcPath += mapData.first;
//			CString tcFileName = tcStr;
//			CString tcFileNameWithExt = tcFileName + L".X";
//			
//			FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
//				Engine::RESOURCE_STAGE,
//				tcFileName,
//				Engine::TYPE_STATIC,
//				tcPath,
//				tcFileNameWithExt),
//				E_FAIL);
//			m_pMapTab->m_treeObjectList.InsertItem(tcFileName, NULL, NULL);
//		}
//	}
//
//	return S_OK;
//}

