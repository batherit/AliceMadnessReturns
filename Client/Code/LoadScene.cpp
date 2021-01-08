#include "pch.h"
#include "LoadScene.h"
#include "Export_Function.h"
#include "PlayScene.h"
#include "UI_Image.h"
#include "Attribute.h"


CLoadScene::CLoadScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
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
			Engine::CManagement::GetInstance()->SetNextScene(CPlayScene::Create(m_pGraphicDev));

			return iExit;
		}	
	}

	return iExit;
}

void CLoadScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();
	// DEBUG ��
	Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_LoadString(), &_vec2(WINCX / 6.f, WINCY - WINCY / 6.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

void CLoadScene::OnLoaded()
{
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
	// ������Ʈ
	//lstrcpy(m_szLoading, L"Registering Component.............................");
	// Ŭ���� ������Ʈ�� ������Ÿ�� �Ŵ����� ���/
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(Engine::CTransform::GetComponentTag(), pComponent);

	// Renderer (������ �ͷ��� ���� �ֵ��� ���� �ֱ� ������ �ϴ� ����.)
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

	pComponent = CAttribute::Create();
	NULL_CHECK_RETURN(pComponent, );
	Engine::Ready_Proto(CAttribute::GetComponentTag(), pComponent);

	Engine::CShader* pShader = nullptr;

	// Sample
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pShader, );
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Sample", pShader), );

	// Terrain
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pShader, );
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Terrain", pShader), );

	// Object
	//pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Reference/Header/Shader_Object.hpp");
	pShader = Engine::CShader::Create(m_pGraphicDev, L"../../Engine/Utility/Code/Shader_Object.hpp");
	NULL_CHECK_RETURN(pShader, );
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Object", pShader), );
}


CLoadScene* CLoadScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadScene*	pInstance = new CLoadScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CLoadScene::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

