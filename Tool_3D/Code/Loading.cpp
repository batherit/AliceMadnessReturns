#include "pch.h"
#include "Loading.h"

#include "Export_Function.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CTerrainTab.h"
#include "CMapTab.h"
#include "CColliderTab.h"


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

	InitializeCriticalSection(&m_Crt);		// 크리티컬 섹션 생성

	// 쓰레드 생성 함수
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	// MapTab에서 쓰일 정적 오브젝트를 세팅한다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pTerrainTab = pTabForm->GetTerrainTab();
	m_pMapTab = pTabForm->GetMapTab();
	m_pColliderTab = pTabForm->GetColliderTab();

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{


	lstrcpy(m_szLoading, L"Buffer Loading.............................");
	
	int i = 0;
	
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_SphereCol", Engine::BUFFER_SPHERECOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_CubeCol", Engine::BUFFER_CUBECOL), E_FAIL);
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



												// 임시 리소스 로드.

	
	// 컴포넌트
	lstrcpy(m_szLoading, L"Registering Component.............................");
	// 클론할 컴포넌트를 프로토타입 매니저에 등록/
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

	// 텍스쳐
	lstrcpy(m_szLoading, L"Texture Loading.............................");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_Player", Engine::TEX_NORMAL, L"../../Resource/Texture/Player/Ma.jpg"), E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												Engine::RESOURCE_STAGE,
												L"Texture_Terrain",
												Engine::TEX_NORMAL,
												L"../../Resource/Texture/Terrain/Grass_%d.tga", 2),
												E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../../Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Height", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/Height2.bmp"), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Texture_SkyBox", Engine::TEX_CUBE, L"../../Resource/Texture/SkyBox/burger%d.dds", 4),  E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"PlayTerrain", Engine::TEX_NORMAL, L"../../Resource/Texture/Terrain/PlayTerrain.bmp"), E_FAIL);
	m_pTerrainTab->m_lbxHeightMap.AddString(L"None");
	m_pTerrainTab->m_lbxHeightMap.AddString(L"PlayTerrain");
	
	m_pTerrainTab->m_lbxTexture.AddString(L"None");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"CobbledStreet_DM", Engine::TEX_NORMAL, L"../../Resource/Terrain/Texture/CobbledStreet_DM.tga"), E_FAIL);
	m_pTerrainTab->m_lbxTexture.AddString(L"CobbledStreet_DM");
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STAGE, L"Rock", Engine::TEX_NORMAL, L"../../Resource/Terrain/Texture/Rock.png"), E_FAIL);
	m_pTerrainTab->m_lbxTexture.AddString(L"Rock");

	lstrcpy(m_szLoading, L"Mesh Loading.............................");
	

	//// Stone 메쉬 원본 생성
	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	Engine::RESOURCE_STAGE,
	//	L"Mesh_Stone",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/TombStone/",
	//	L"TombStone.X"),
	//	E_FAIL);
	//m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Mesh_Stone");

	//// 스톤에 맞는 메쉬 콜라이더 원본 생성.
	//Engine::CStaticMesh* pStaticMesh = nullptr;
	//pStaticMesh = static_cast<Engine::CStaticMesh*>(Engine::GetOriResource(Engine::RESOURCE_STAGE, L"Mesh_Stone"));
	//pComponent = Engine::CMeshCollider::Create(m_pGraphicDev, pStaticMesh->Get_VtxPos(), pStaticMesh->Get_NumVtx(), pStaticMesh->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//Engine::Ready_Proto(L"Collider_Stone", pComponent);

	//LoadMapObjectData();

	// Domino 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Domino",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Domino/",
		L"Domino.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Domino");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Domino");

	// 보팔검 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"VorpalBlade",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/VorpalBlade/",
		L"VorpalBlade.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"VorpalBlade");

	// 말막대기 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"HobbyHorse",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/HobbyHorse/",
		L"HobbyHorse.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"HobbyHorse");

	// 총알 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Bullet",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Bullet/",
		L"Bullet.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Bullet");

	// 토끼폭탄 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"BunnyBomb",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/BunnyBomb/",
		L"BunnyBomb.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"BunnyBomb");

	// 포크 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Fork",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Fork/",
		L"Fork.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Fork");

	// Custom

	// 점프 패드 생성(Dynamic)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"JumpPad",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/JumpPad/",
		L"JumpPad.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"JumpPad");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"JumpPad");

	// HP Flower 생성(Dynamic)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"HPFlower",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/HPFlower/",
		L"HPFlower.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"HPFlower");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"HPFlower");

	// // Snail 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Snail",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Snail/",
		L"Snail.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Snail");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Snail");

	// Tooth 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Tooth",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Tooth/",
		L"Tooth.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Tooth");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Tooth");

	// Valve 생성(Static)
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Valve",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Valve/",
		L"Valve.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Valve");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Valve");

	// CobbledStreet 생성
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"CobbledStreet",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"CobbledStreet.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"CobbledStreet");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"CobbledStreet");

	// CobbledStreet_Even
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"CobbledStreet_Even",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"CobbledStreet_Even.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"CobbledStreet_Even");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"CobbledStreet_Even");

	// PoorBuildingE01
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"PoorBuildingE01",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"PoorBuildingE01.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"PoorBuildingE01");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"PoorBuildingE01");

	// Curb_90
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_90",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_90.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_90");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_90");

	// Curb_Uneven
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Uneven",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_Uneven.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_Uneven");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_Uneven");

	// Curb_Section
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Curb_Section",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/",
		L"Curb_Section.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_Section");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Curb_Section");

	// 돌과 나무들
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DecoMushroom_AlienGroup",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"DecoMushroom_AlienGroup.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DecoMushroom_AlienGroup");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DecoMushroom_AlienGroup");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarARockPillarA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarARockPillarA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarARockPillarA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarARockPillarA");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GiantMossRockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GiantMossRockA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GiantMossRockA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GiantMossRockA");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRockA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRockA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRockA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRockA");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRockB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRockB.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRockB");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRockB");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GiantOrientalRock_Wall",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GiantOrientalRock_Wall.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GiantOrientalRock_Wall");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GiantOrientalRock_Wall");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarA");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarB",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarB.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarB");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarB");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarB2",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarB2.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarB2");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarB2");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"RockPillarC",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"RockPillarC.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarC");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"RockPillarC");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"TearsRock",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"TearsRock.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"TearsRock");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"TearsRock");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"GloryTree_Base",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"GloryTree_Base.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GloryTree_Base");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"GloryTree_Base");


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MossRock_Walkable",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"MossRock_Walkable.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRock_Walkable");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"MossRock_Walkable");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"Keyhole_Glory",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Deco/",
		L"Keyhole_Glory.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Keyhole_Glory");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"Keyhole_Glory");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_CurveA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_CurveA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_CurveA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_CurveA");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_CurveD",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_CurveD.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_CurveD");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_CurveD");

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"DollSlide_StraightA",
		Engine::TYPE_STATIC,
		L"../../Resource/Mesh/StaticMesh/Chapter1/Slide/",
		L"DollSlide_StraightA.X"),
		E_FAIL);
	m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_StraightA");
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_STATIC, L"DollSlide_StraightA");

	// 앨리스W
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"AliceW",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/Alice/",
		L"AliceW.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"AliceW");

	// 매드캡A
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		Engine::RESOURCE_STAGE,
		L"MadCapA",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/MadCap/",
		L"MadCapA.X"),
		E_FAIL);
	m_pColliderTab->RegisterMeshTag(Engine::TYPE_DYNAMIC, L"MadCapA");
	

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Player",
	//	Engine::TYPE_DYNAMIC,
	//	L"../../Resource/Mesh/DynamicMesh/PlayerXfile/",
	//	L"Player.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Tree",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/Tree/",
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
//			CString tcPath = L"../../Resource/Mesh/StaticMesh/";
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
//			m_pMapTab->RegisterMeshTag(Engine::TYPE_STATIC, tcFileName);
//		}
//	}
//
//	return S_OK;
//}

