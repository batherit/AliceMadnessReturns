#include "pch.h"
#include "CMainApp.h"
#include "LoadScene.h"
#include "PoolScene.h"
#include "PlayScene.h"
#include "Play2Scene.h"
#include "BossScene.h"
//#include "PlayScene.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

//LRESULT CMainApp::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	return Engine::CManagement::GetInstance()->GetSceneMgr()->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	Engine::Safe_AddRef(m_pDeviceClass);

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	Engine::Safe_AddRef(m_pGraphicDev);

	// 디퍼드 렌더링을 사용하기 위하여 호출함.
	FAILED_CHECK_RETURN(Engine::Get_Renderer()->Ready_Renderer(m_pGraphicDev), E_FAIL)

	// 임시 리소스 로드.
	FAILED_CHECK_RETURN(Engine::Reserve_ContainerSize(Engine::RESOURCE_END), E_FAIL);
	Client::Safe_Release(m_pDeviceClass);

	// 사운드 매니져 초기화
	CSoundMgr::Get_Instance()->Initialize();

	// 샘플링 상태 설정
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// 인풋 매니져 세팅
	FAILED_CHECK_RETURN(Engine::CDirectInputMgr::GetInstance()->Ready(g_hInst, g_hWnd), E_FAIL);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_UP", DIK_UP);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_DOWN", DIK_DOWN);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_LEFT", DIK_LEFT);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_RIGHT", DIK_RIGHT);

	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_W", DIK_W);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_A", DIK_A);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_S", DIK_S);
	//Engine::CDirectInputMgr::GetInstance()->BindKeyStringToKey(L"KEY_D", DIK_D);
	
	// 폰트 세팅
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Jinji", L"궁서", 30, 30, FW_HEAVY), E_FAIL);

	// 첫 씬을 Logo로 설정한다.
	Engine::CManagement::GetInstance()->SetNextScene(CLoadScene::Create(m_pGraphicDev));

	// 랜덤 씨드 할당
	srand((_uint)time(NULL));
	
	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	Engine::CDirectInputMgr::GetInstance()->Update();

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_7)) {
		Engine::CManagement::GetInstance()->SetNextScene(CPoolScene::Create(m_pGraphicDev));
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_8)) {
		Engine::CManagement::GetInstance()->SetNextScene(CPlayScene::Create(m_pGraphicDev));
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_9)) {
		Engine::CManagement::GetInstance()->SetNextScene(CPlay2Scene::Create(m_pGraphicDev));
	}
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_0)) {
		Engine::CManagement::GetInstance()->SetNextScene(CBossScene::Create(m_pGraphicDev));
	}

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
	CSoundMgr::Destroy_Instance();
	CDataMgr::DestroyInstance();
}

