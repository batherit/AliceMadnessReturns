#include "pch.h"
#include "Tool3D_Kernel.h"
#include "LoadScene.h"
#include "EditScene.h"
#include "ColliderScene.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Terrain.h"
#include "NaviMesh.h"
#include "InputProcessor_Terrain.h"


CTool3D_Kernel::CTool3D_Kernel(void)
{

}

CTool3D_Kernel::~CTool3D_Kernel(void)
{

}

//LRESULT CTool3D_Kernel::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	return Engine::CManagement::GetInstance()->GetSceneMgr()->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

HRESULT CTool3D_Kernel::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, Engine::MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	Engine::Safe_AddRef(m_pDeviceClass);

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	Engine::Safe_AddRef(m_pGraphicDev);

	// 임시 리소스 로드.
	FAILED_CHECK_RETURN(Engine::Reserve_ContainerSize(Engine::RESOURCE_END), E_FAIL);
	Client::Safe_Release(m_pDeviceClass);

	// 샘플링 상태 설정
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

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

	// 입력 처리기 세팅
	m_pInputProcessorMgr = Engine::CInputProcessorMgr::Create();
	NULL_CHECK_RETURN(m_pInputProcessorMgr, E_FAIL);
	m_pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Terrain(m_pInputProcessorMgr));

	// 첫 씬을 Logo로 설정한다.
	Engine::CManagement::GetInstance()->SetNextScene(CLoadScene::Create(m_pGraphicDev));
	
	return S_OK;
}

_int CTool3D_Kernel::Update_MainApp(const _float& fTimeDelta)
{
	m_pInputProcessorMgr->ProcessInput(fTimeDelta);
	if (!Engine::CManagement::GetInstance()->ConfirmValidScene())
		return -1;
	Engine::CDirectInputMgr::GetInstance()->Update();
	Engine::CManagement::GetInstance()->UpdateScene(fTimeDelta);

	return 0;
}

void CTool3D_Kernel::Render_MainApp(void)
{
	Engine::Render_Begin(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.f));

	// 현재 씬을 렌더한다.
	Engine::CManagement::GetInstance()->RenderScene();

	Engine::Render_End();
}

CTool3D_Kernel* CTool3D_Kernel::Create(void)
{
	CTool3D_Kernel*	pInstance = new CTool3D_Kernel;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("CTool3D_Kernel Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool3D_Kernel::Free(void)
{
	Engine::Safe_Release(m_pInputProcessorMgr);

	Client::Safe_Release(m_pGraphicDev);

	Engine::Release_Resoures();
	Engine::Release_Utility();

	for_each(m_vecStoredDynamicObjects.begin(), m_vecStoredDynamicObjects.end(), Engine::CDeleteObj());
	m_vecStoredDynamicObjects.clear();
	m_vecStoredDynamicObjects.shrink_to_fit();
	for_each(m_vecStoredStaticObjects.begin(), m_vecStoredStaticObjects.end(), Engine::CDeleteObj());
	m_vecStoredStaticObjects.clear();
	m_vecStoredStaticObjects.shrink_to_fit();
	for_each(m_vecStoredTriggerObjects.begin(), m_vecStoredTriggerObjects.end(), Engine::CDeleteObj());
	m_vecStoredTriggerObjects.clear();
	m_vecStoredTriggerObjects.shrink_to_fit();

	Engine::Safe_Release(m_pStoredTerrain);
	Engine::Safe_Release(m_pStoredNaviMesh);
	for_each(m_vecStoredDynamicObjects_Collider.begin(), m_vecStoredDynamicObjects_Collider.end(), Engine::CDeleteObj());
	m_vecStoredDynamicObjects_Collider.clear();
	m_vecStoredDynamicObjects_Collider.shrink_to_fit();
	for_each(m_vecStoredStaticObjects_Collider.begin(), m_vecStoredStaticObjects_Collider.end(), Engine::CDeleteObj());
	m_vecStoredStaticObjects_Collider.clear();
	m_vecStoredStaticObjects_Collider.shrink_to_fit();

	Engine::Release_System();
}

CEditScene * CTool3D_Kernel::GetEditScene() const
{
	return dynamic_cast<CEditScene*>(Engine::CManagement::GetInstance()->GetSceneMgr()->GetCurScene());
}

CColliderScene * CTool3D_Kernel::GetColliderScene() const
{
	return dynamic_cast<CColliderScene*>(Engine::CManagement::GetInstance()->GetSceneMgr()->GetCurScene());
}

Engine::CScene * CTool3D_Kernel::GetCurScene() const
{
	return Engine::CManagement::GetInstance()->GetSceneMgr()->GetCurScene();
}

Engine::CInputProcessor * CTool3D_Kernel::GetInputProcessor() const
{
	return m_pInputProcessorMgr->GetCurInputProcessor();
}

//void CTool3D_Kernel::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	CEditScene* pEditScene = GetEditScene();
//	if (!pEditScene) return;
//
//
//}

