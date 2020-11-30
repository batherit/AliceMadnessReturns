#include "pch.h"
#include "EditScene.h"
#include "DynamicCamera.h"
#include "Terrain.h"
#include "NaviMesh.h"
#include "NaviMeshVtxCtrl.h"
#include "NaviMeshInputProcessor.h"
#include "InputMode_Terrain.h"
#include "MainFrm.h"
#include "CTabForm.h"
//#include "InputMode_Navi.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CEditScene::CEditScene(const CEditScene & rhs)
	:
	CScene(rhs)
{
}

CEditScene::~CEditScene(void)
{
}

void CEditScene::ResetScene(void)
{

}

HRESULT CEditScene::Ready(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	// ī�޶� ����
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDynamicCamera::Create(m_pGraphicDev);		// ���� ī�޶�
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject), E_FAIL);

	// ���� ����
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);
	dynamic_cast<CTerrain*>(pGameObject)->CreateTerrain(129, 129, 129.f, 129.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");

	// �׺�޽� ������Ʈ ����
	CNaviMesh* pNaviMesh = CNaviMesh::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pNaviMesh, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMesh", pNaviMesh), E_FAIL);

	// �׺�޽����ؽ���Ʈ�ѷ� ����
	CNaviMeshVtxCtrl* pNaviMeshVtxCtrl = CNaviMeshVtxCtrl::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pNaviMeshVtxCtrl, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMeshVtxCtrl", pNaviMeshVtxCtrl), E_FAIL);
	pNaviMeshVtxCtrl->SetNaviMesh(pNaviMesh);
	pNaviMeshVtxCtrl->SetActive(false);

	// �׺�޽� �Է� ó���� ����
	CNaviMeshInputProcessor* pNaviMeshInputProcessor = CNaviMeshInputProcessor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pNaviMeshInputProcessor, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMeshInputProcessor", pNaviMeshInputProcessor), E_FAIL);

	// ���� ���̾� ���
	m_mapLayer.emplace(L"EditLayer", pLayer);

	// �Է� ��� �Ŵ��� ����
	m_pInputModeMgr = Engine::CInputModeMgr::Create();
	NULL_CHECK_RETURN(m_pInputModeMgr, E_FAIL);
	m_pInputModeMgr->SetNextInputMode(new CInputMode_Terrain(m_pInputModeMgr));

	// �ø�� ����
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// ���� ����
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���ĺ��� ����
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return S_OK;
}

int CEditScene::Update(const _float& fTimeDelta)
{
	m_pInputModeMgr->ProcessInput(fTimeDelta);

	return CScene::Update(fTimeDelta);
}

void CEditScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();
}

CEditScene * CEditScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEditScene*	pInstance = new CEditScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CEditScene::Free(void)
{
	Engine::Safe_Release(m_pInputModeMgr);
	CScene::Free();
}

CTerrain * CEditScene::GetTerrain() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"Terrain");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CTerrain*>(*rLayerList.begin());
}

CNaviMesh * CEditScene::GetNaviMesh() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"NaviMesh");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CNaviMesh*>(*rLayerList.begin());
}

CNaviMeshVtxCtrl * CEditScene::GetNaviMeshVtxCtrl() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"NaviMeshVtxCtrl");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CNaviMeshVtxCtrl*>(*rLayerList.begin());
}

CNaviMeshInputProcessor * CEditScene::GetNaviMeshInputProcessor() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"NaviMeshInputProcessor");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CNaviMeshInputProcessor*>(*rLayerList.begin());
}

Engine::CGameObject * CEditScene::GetPickedObject() const
{
	return GetTerrain();
}

void CEditScene::SaveNaviMesh()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	CFileDialog Dlg(FALSE, L"navi", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.navi) | *.navi||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Bin\\Resource\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		GetNaviMesh()->SaveInfo(hFile);

		CloseHandle(hFile);
	}
}

void CEditScene::LoadNaviMesh()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE, L"nav", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.navi) | *.navi||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Bin\\Resource\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		GetNaviMesh()->LoadInfo(hFile);

		CloseHandle(hFile);
	}
	//UpdateData(FALSE);
}
