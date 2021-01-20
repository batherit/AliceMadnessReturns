#include "pch.h"
#include "LoadScene.h"
#include "PlayScene.h"
#include "Play2Scene.h"
#include "BossScene.h"
#include "PoolScene.h"
#include "Export_Function.h"
#include "TitleScene.h"
#include "UI_Image.h"
#include "Attribute.h"
#include "Route.h"


CLoadScene::CLoadScene(LPDIRECT3DDEVICE9 m_pGraphicDev)
	: Engine::CScene(m_pGraphicDev)
{

}

CLoadScene::~CLoadScene(void)
{

}

void CLoadScene::ResetScene(void)
{
}


HRESULT CLoadScene::Ready(void)
{


	return S_OK;
}

Engine::_int CLoadScene::Update(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			//Engine::CManagement::GetInstance()->SetNextScene(CTitleScene::Create(m_pGraphicDev));
			//Engine::CManagement::GetInstance()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
			//Engine::CManagement::GetInstance()->SetNextScene(CPlay2Scene::Create(m_pGraphicDev));
			Engine::CManagement::GetInstance()->SetNextScene(CBossScene::Create(m_pGraphicDev));
			//Engine::CManagement::GetInstance()->SetNextScene(CPoolScene::Create(m_pGraphicDev));

			return iExit;
		}	
	}

	return iExit;
}

void CLoadScene::Render(void)
{
	//Engine::Get_Renderer()->Render_GameObject();
	Engine::CScene::Render();
	// DEBUG 용
	Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_LoadString(), &_vec2(WINCX / 6.f, WINCY - WINCY / 6.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CLoadScene::OnLoaded()
{
	// 커서 없애기
	ShowCursor(false);

	LoadComponents();

	Engine::CLayer* pLayer = Engine::CLayer::Create();

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"UI_AliceTitle", Engine::TEX_NORMAL, L"../../Resource/Texture/UI/UI_AliceTitle.tga"), );
	CUI_Image* pImage = CUI_Image::Create(m_pGraphicDev);
	pImage->SetTexture(L"UI_AliceTitle");
	pImage->SetExtractionArea(RECT{ 0, 0, 993, 607 });
	pImage->SetOutputArea(RECT{ 0, 0, WINCX, WINCY });
	pLayer->Add_GameObject(pImage);

	m_mapLayer.emplace(L"LoadLayer", pLayer);
	

	// Loading Thread
	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	FAILED_CHECK_RETURN(m_pLoading, );
}

void CLoadScene::OnExited()
{
}

void CLoadScene::LoadComponents()
{
	// 컴포넌트
	//lstrcpy(m_szLoading, L"Registering Component.............................");
	// 클론할 컴포넌트를 프로토타입 매니저에 등록/
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CTransform::GetComponentTag(), pComponent);

	// Renderer (지금은 터레인 같은 애들이 쓰고 있기 때문에 일단 놔둠.)
	pComponent = Engine::CRenderer::GetInstance();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CRenderer::GetComponentTag(), pComponent);

	// PolygonRenderer
	pComponent = Engine::CPolygonRenderer::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CPolygonRenderer::GetComponentTag(), pComponent);

	// MeshRenderer
	pComponent = Engine::CMeshRenderer::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CMeshRenderer::GetComponentTag(), pComponent);

	// TerrainTex
	pComponent = Engine::CTerrainTex::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CTerrainTex::GetComponentTag(), pComponent);

	// SphereCollider
	pComponent = Engine::CSphereCollider::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CSphereCollider::GetComponentTag(), pComponent);

	// Physics
	pComponent = Engine::CPhysics::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CPhysics::GetComponentTag(), pComponent);

	// Optimization
	pComponent = Engine::COptimization::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::COptimization::GetComponentTag(), pComponent);

	// Attribute
	pComponent = CAttribute::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(CAttribute::GetComponentTag(), pComponent);

	// Route
	pComponent = CRoute::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(CRoute::GetComponentTag(), pComponent);

	// RenderTarget _Deffered
	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(m_pGraphicDev,
		L"Target_Albedo",
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), );

	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Albedo", 0.f, 0.f, 200.f, 200.f), );

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(m_pGraphicDev,
		L"Target_Normal",
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), );
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Normal", 0.f, 200.f, 200.f, 200.f), );

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(m_pGraphicDev,
		L"Target_Shade",
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), );
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Shade", 200.f, 0.f, 200.f, 200.f), );

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(m_pGraphicDev,
		L"Target_Specular",
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A16B16G16R16F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), );
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Specular", 200.f, 200.f, 200.f, 200.f), );

	FAILED_CHECK_RETURN(Engine::Ready_RenderTarget(m_pGraphicDev,
		L"Target_Depth",
		ViewPort.Width,
		ViewPort.Height,
		D3DFMT_A32B32G32R32F,
		D3DXCOLOR(0.f, 0.f, 0.f, 1.f)), );
	FAILED_CHECK_RETURN(Engine::Ready_DebugBuffer(L"Target_Depth", 0.f, 400.f, 200.f, 200.f), );


	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Target_Albedo"), );
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Target_Normal"), );
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_Deferred", L"Target_Depth"), );

	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Target_Shade"), );
	FAILED_CHECK_RETURN(Engine::Ready_MRT(L"MRT_LightAcc", L"Target_Specular"), );

	// Shader
	Engine::CShader* pShader = nullptr;

	// Sample
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader,);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Sample", pShader),);

	// Terrain
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader,);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Terrain", pShader),);

	// Terrain
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pShader,);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Mesh", pShader),);

	// Shade
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Shade.hpp");
	NULL_CHECK_RETURN(pShader,);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Shade", pShader),);

	// Blend
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Blend.hpp");
	NULL_CHECK_RETURN(pShader,);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Blend", pShader),);
}


CLoadScene* CLoadScene::Create(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	CLoadScene*	pInstance = new CLoadScene(m_pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CLoadScene::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

