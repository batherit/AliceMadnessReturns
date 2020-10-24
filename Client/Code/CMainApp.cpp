#include "pch.h"
#include "CMainApp.h"
#include "PlayScene.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

LRESULT CMainApp::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return Engine::CManagement::GetInstance()->GetSceneMgr()->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	Engine::Safe_AddRef(m_pDeviceClass);

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	Engine::Safe_AddRef(m_pGraphicDev);

	// 인풋 매니져 세팅
	FAILED_CHECK_RETURN(Engine::CDirectInputMgr::GetInstance()->Ready(g_hInst, g_hWnd), E_FAIL);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_UP", DIK_UP);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_DOWN", DIK_DOWN);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_LEFT", DIK_LEFT);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_RIGHT", DIK_RIGHT);

	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_W", DIK_W);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_A", DIK_A);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_S", DIK_S);
	Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_D", DIK_D);
	
	// 폰트 세팅
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY), E_FAIL);

	// 임시 리소스 로드.
	FAILED_CHECK_RETURN(Engine::Reserve_ContainerSize(Engine::RESOURCE_END), E_FAIL);
	Client::Safe_Release(m_pDeviceClass);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Texture_Logo", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Logo/Logo.jpg"), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev, Engine::RESOURCE_STATIC, L"M_Buffer_TriCol", Engine::BUFFER_TRICOL), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev, Engine::RESOURCE_STATIC, L"Height", Engine::TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Height1.bmp"), E_FAIL);
	
	// 클론할 컴포넌트를 프로토타입 매니저에 등록/
	Engine::CComponent* pComponent = nullptr;
	pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CTransform::GetComponentTag(), pComponent);

	pComponent = Engine::CRenderer::GetInstance();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CRenderer::GetComponentTag(), pComponent);

	pComponent = Engine::CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Proto(Engine::CTerrain::GetComponentTag(), pComponent);

	// 키 세팅
	//Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_LBUTTON", VK_LBUTTON);
	//Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_W", 'W');
	//Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_A", 'A');
	//Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_S", 'S');
	//Engine::CKeyMgr::GetInstance()->BindKeyStringToKey(L"KEY_D", 'D');


	// 첫 씬을 Logo로 설정한다.
	Engine::CManagement::GetInstance()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
	
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	Engine::CDirectInputMgr::GetInstance()->Update();
	Engine::CManagement::GetInstance()->UpdateScene(fTimeDelta);

	return 0;
}

void CMainApp::Render_MainApp(void)
{
	Engine::Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	// 현재 씬을 렌더한다.
	Engine::CManagement::GetInstance()->RenderScene();

	Engine::Render_End();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("CMainApp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	Client::Safe_Release(m_pGraphicDev);

	Engine::Release_Resoures();
	Engine::Release_Utility();
	Engine::Release_System();
}

