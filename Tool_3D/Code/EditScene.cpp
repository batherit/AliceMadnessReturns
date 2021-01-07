#include "pch.h"
#include "EditScene.h"
#include "DynamicCamera.h"
#include "Terrain.h"
#include "NaviMesh.h"
//#include "NaviMeshVtxCtrl.h"
#include "NaviMeshVtxMover.h"
#include "NaviMeshInputProcessor.h"
#include "Gizmo.h"
#include "InputProcessor_Terrain.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CMapTab.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
//#include "InputProcessor_Navi.h"

CEditScene::CEditScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
	// 저장되어 있던 정보를 읽어온다.
	//for (auto& rObj : g_pTool3D_Kernel->m_vecStoredDynamicObjects) {
	//	m_vecDynamicObjects.emplace_back(rObj);
	//}
	//g_pTool3D_Kernel->m_vecStoredDynamicObjects.clear();

	//for (auto& rObj : g_pTool3D_Kernel->m_vecStoredStaticObjects) {
	//	m_vecStaticObjects.emplace_back(rObj);
	//}
	//g_pTool3D_Kernel->m_vecStoredStaticObjects.clear();

	//for (auto& rObj : g_pTool3D_Kernel->m_vecStoredTriggerObjects) {
	//	m_vecTriggerObjects.emplace_back(rObj);
	//}
	//g_pTool3D_Kernel->m_vecStoredTriggerObjects.clear();
}

CEditScene::CEditScene(const CEditScene & rhs)
	:
	CScene(rhs)
{
}

CEditScene::~CEditScene(void)
{
	// 저장한다.
}

void CEditScene::ResetScene(void)
{

}

HRESULT CEditScene::Ready(void)
{


	return S_OK;
}

int CEditScene::Update(const _float& fTimeDelta)
{
	//m_pInputProcessorMgr->ProcessInput(fTimeDelta);

	/*static _float fGapX = 0.f;
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_L)) {
		AddDynamicObject(L"AliceW", _vec3(fGapX, 0.f, 0.f));
		fGapX += 20.f;
	}*/

	return CScene::Update(fTimeDelta);
}

void CEditScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();
}

void CEditScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	// 저장되어 있던 정보를 읽어온다.
	for (auto& rObj : g_pTool3D_Kernel->m_vecStoredDynamicObjects) {
		m_vecDynamicObjects.emplace_back(rObj);
	}
	g_pTool3D_Kernel->m_vecStoredDynamicObjects.clear();

	for (auto& rObj : g_pTool3D_Kernel->m_vecStoredStaticObjects) {
		m_vecStaticObjects.emplace_back(rObj);
	}
	g_pTool3D_Kernel->m_vecStoredStaticObjects.clear();

	for (auto& rObj : g_pTool3D_Kernel->m_vecStoredTriggerObjects) {
		m_vecTriggerObjects.emplace_back(rObj);
	}
	g_pTool3D_Kernel->m_vecStoredTriggerObjects.clear();

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer,);

	// 카메라 생성
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDynamicCamera::Create(m_pGraphicDev);		// 동적 카메라
	NULL_CHECK_RETURN(pGameObject,);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject),);

	// 지형 생성
	if (g_pTool3D_Kernel->m_pStoredTerrain)
		pGameObject = g_pTool3D_Kernel->m_pStoredTerrain;
	else
		pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject,);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject),);
	//dynamic_cast<CTerrain*>(pGameObject)->CreateTerrain(2, 2, 129.f, 129.f, 1.f, 1.f, nullptr);
	//dynamic_cast<CTerrain*>(pGameObject)->CreateTerrain(129, 129, 129.f, 129.f, L"../Bin/Resource/Texture/Terrain/Height2.bmp");

	// 네비메쉬 오브젝트 생성
	CNaviMesh* pNaviMesh = nullptr;
	if (g_pTool3D_Kernel->m_pStoredNaviMesh)
		pNaviMesh = g_pTool3D_Kernel->m_pStoredNaviMesh;
	else
		pNaviMesh = CNaviMesh::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pNaviMesh,);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMesh", pNaviMesh),);

	// 네비메쉬버텍스컨트롤러 생성
	//CNaviMeshVtxCtrl* pNaviMeshVtxCtrl = CNaviMeshVtxCtrl::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pNaviMeshVtxCtrl, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMeshVtxCtrl", pNaviMeshVtxCtrl), E_FAIL);
	//pNaviMeshVtxCtrl->SetNaviMesh(pNaviMesh);
	//pNaviMeshVtxCtrl->SetActive(false);

	// 네비메쉬정점이동자 생성
	CNaviMeshVtxMover* pNaviMeshVtxMover = CNaviMeshVtxMover::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pNaviMeshVtxMover,);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMeshVtxMover", pNaviMeshVtxMover),);

	// 오브젝트 기즈모 생성
	CGizmo* pGizmo = CGizmo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGizmo,);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Gizmo", pGizmo),);
	pGizmo->ActivateGizmo(false);

	// 저장된 정보를 레이어에 집어넣는다.
	for (auto& rObj : m_vecDynamicObjects) {
		pLayer->Add_GameObject(rObj);
		//Engine::Safe_AddRef(rObj);
	}
	for (auto& rObj : m_vecStaticObjects) {
		pLayer->Add_GameObject(rObj);
		//Engine::Safe_AddRef(rObj);
	}
	for (auto& rObj : m_vecTriggerObjects) {
		pLayer->Add_GameObject(rObj);
	}

	// 커스텀 오브젝트 생성을 위한 맵을 만든다.
	/*m_mapCustomObjectType[L"HPFlower"] = Engine::TYPE_DYNAMIC;
	m_mapCustomObjectType[L"JumpPad"] = Engine::TYPE_DYNAMIC;
	m_mapCustomObjectType[L"Snail"] = Engine::TYPE_STATIC;
	m_mapCustomObjectType[L"Tooth"] = Engine::TYPE_STATIC;
	m_mapCustomObjectType[L"PopDomino"] = Engine::TYPE_STATIC;
	m_mapCustomObjectType[L"Valve"] = Engine::TYPE_STATIC;*/

	// 편집 레이어 등록
	m_mapLayer.emplace(L"EditLayer", pLayer);

	// 입력 모드 매니저 생성
	/*m_pInputProcessorMgr = Engine::CInputProcessorMgr::Create();
	NULL_CHECK_RETURN(m_pInputProcessorMgr, E_FAIL);
	m_pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Terrain(m_pInputProcessorMgr));*/

	// 컬모드 설정
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 조명 설정
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 알파블렌딩 설정(일단 해제)
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CEditScene::OnExited()
{
	// 씬을 지우기 전에 오브젝트를 저장해둔다.
	// 터레인 저장하기
	g_pTool3D_Kernel->m_pStoredTerrain = GetTerrain();
	Engine::Safe_AddRef(g_pTool3D_Kernel->m_pStoredTerrain);
	// 네비 메쉬 저장하기
	g_pTool3D_Kernel->m_pStoredNaviMesh = GetNaviMesh();
	Engine::Safe_AddRef(g_pTool3D_Kernel->m_pStoredNaviMesh);
	// 동적 메쉬 저장하기
	for (auto& rObj : m_vecDynamicObjects) {
		g_pTool3D_Kernel->m_vecStoredDynamicObjects.emplace_back(rObj);
		Engine::Safe_AddRef(rObj);
	}
	m_vecDynamicObjects.clear();
	m_vecDynamicObjects.shrink_to_fit();
	// 정적 메쉬 저장하기
	for (auto& rObj : m_vecStaticObjects) {
		g_pTool3D_Kernel->m_vecStoredStaticObjects.emplace_back(rObj);
		Engine::Safe_AddRef(rObj);
	}
	m_vecStaticObjects.clear();
	m_vecStaticObjects.shrink_to_fit();
	// 트리거 저장하기
	for (auto& rObj : m_vecTriggerObjects) {
		g_pTool3D_Kernel->m_vecStoredTriggerObjects.emplace_back(rObj);
		Engine::Safe_AddRef(rObj);
	}
	m_vecTriggerObjects.clear();
	m_vecTriggerObjects.shrink_to_fit();
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
	//Engine::Safe_Release(m_pInputProcessorMgr);

	

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

CNaviMeshVtxMover * CEditScene::GetNaviMeshVtxMover() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"NaviMeshVtxMover");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CNaviMeshVtxMover*>(*rLayerList.begin());
}

CGizmo * CEditScene::GetGizmo() const
{
	auto pLayer = GetLayer(L"EditLayer");

	if (!pLayer)
		return nullptr;

	auto& rLayerList = pLayer->GetLayerList(L"Gizmo");

	if (rLayerList.empty())
		return nullptr;

	return static_cast<CGizmo*>(*rLayerList.begin());
}

//Engine::CInputProcessor * CEditScene::GetInputProcessor() const
//{
//	return m_pInputProcessorMgr->GetCurInputProcessor();
//}

//CNaviMeshInputProcessor * CEditScene::GetNaviMeshInputProcessor() const
//{
//	auto pLayer = GetLayer(L"EditLayer");
//
//	if (!pLayer)
//		return nullptr;
//
//	auto& rLayerList = pLayer->GetLayerList(L"NaviMeshInputProcessor");
//
//	if (rLayerList.empty())
//		return nullptr;
//
//	return static_cast<CNaviMeshInputProcessor*>(*rLayerList.begin());
//}

Engine::CGameObject * CEditScene::GetPickedObject() const
{
	return GetTerrain();
}

_bool CEditScene::AddStaticObject(const _tchar * _pMeshTag)
{
	CStaticObject* pStaticObject = CStaticObject::Create(m_pGraphicDev);
	pStaticObject->SetRenderInfo(_pMeshTag);

	if (pStaticObject->GetComponent<Engine::CStaticMesh>()) {
		GetLayer(L"EditLayer")->Add_GameObject(pStaticObject);
		m_vecStaticObjects.emplace_back(pStaticObject);
		return true;
	}

	Engine::Safe_Release(pStaticObject);
	return false;
}

_bool CEditScene::AddDynamicObject(const _tchar * _pMeshTag, const _vec3& _vPos)
{
	CDynamicObject* pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
	pDynamicObject->SetRenderInfo(_pMeshTag);

	if (pDynamicObject->GetComponent<Engine::CDynamicMesh>()) {
		GetLayer(L"EditLayer")->Add_GameObject(pDynamicObject);
		pDynamicObject->GetTransform()->SetPos(_vPos);
		m_vecDynamicObjects.emplace_back(pDynamicObject);
		return true;
	}

	Engine::Safe_Release(pDynamicObject);
	return false;
}

_bool CEditScene::AddTriggerObject(TRIGGER::E_TYPE _eTriggerType)
{
	CTrigger* pTriggerObject = CTrigger::Create(m_pGraphicDev);
	pTriggerObject->SetTriggerType(_eTriggerType);
	GetLayer(L"EditLayer")->Add_GameObject(pTriggerObject);
	m_vecTriggerObjects.emplace_back(pTriggerObject);

	return true;
}

//_bool CEditScene::AddCustomObject(const _tchar * _pMeshTag)
//{
//	auto find_iter = find_if(m_mapCustomObjectType.begin(), m_mapCustomObjectType.end(), Engine::CTag_Finder(_pMeshTag));
//
//	if (find_iter == m_mapCustomObjectType.end())
//		return false;
//
//	Engine::CGameObject* pCustomObject = nullptr;
//	switch (find_iter->second) {
//	case Engine::TYPE_DYNAMIC: {
//		CDynamicObject* pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
//		pDynamicObject->SetRenderInfo(_pMeshTag);
//
//		if (pDynamicObject->GetComponent<Engine::CDynamicMesh>()) {
//			GetLayer(L"EditLayer")->Add_GameObject(pDynamicObject);
//			m_vecCustomObjects.emplace_back(pDynamicObject);
//			return true;
//		}
//
//		Engine::Safe_Release(pDynamicObject);
//		break;
//	}
//	case Engine::TYPE_STATIC: {
//		CStaticObject* pStaticObject = CStaticObject::Create(m_pGraphicDev);
//		pStaticObject->SetRenderInfo(_pMeshTag);
//
//		if (pStaticObject->GetComponent<Engine::CStaticMesh>()) {
//			GetLayer(L"EditLayer")->Add_GameObject(pStaticObject);
//			m_vecCustomObjects.emplace_back(pStaticObject);
//			return true;
//		}
//
//		Engine::Safe_Release(pStaticObject);
//		break;
//	}
//		
//	}
//
//	return false;
//}

CStaticObject * CEditScene::GetStaticObject(_int _iObjectIndex)
{
	if(!IsValidStaticObjectIndex(_iObjectIndex))
		return nullptr;

	return m_vecStaticObjects[_iObjectIndex];
}

CStaticObject * CEditScene::GetStaticObject(const _tchar * _pMeshTag)
{
	for (auto& rObj : m_vecStaticObjects) {
		if (lstrcmp(rObj->GetMeshTag(), _pMeshTag) == 0) {
			return rObj;
		}
	}

	return nullptr;
}

CDynamicObject * CEditScene::GetDynamicObject(_int _iObjectIndex)
{
	if (!IsValidDynamicObjectIndex(_iObjectIndex))
		return nullptr;

	return m_vecDynamicObjects[_iObjectIndex];
}

CDynamicObject * CEditScene::GetDynamicObject(const _tchar * _pMeshTag)
{
	for (auto& rObj : m_vecDynamicObjects) {
		if (lstrcmp(rObj->GetMeshTag(), _pMeshTag) == 0) {
			return rObj;
		}
	}

	return nullptr;
}

CTrigger * CEditScene::GetTriggerObject(_int _iObjectIndex)
{
	if (!IsValidTriggerObjectIndex(_iObjectIndex))
		return nullptr;

	return m_vecTriggerObjects[_iObjectIndex];
}

//Engine::CGameObject * CEditScene::GetCustomObject(_int _iObjectIndex)
//{
//	if (!IsValidCustomObjectIndex(_iObjectIndex))
//		return nullptr;
//
//	return m_vecCustomObjects[_iObjectIndex];
//}


Engine::CGameObject * CEditScene::GetObjectFromTag(const _tchar * _pMeshTag)
{
	Engine::CGameObject* pObject = nullptr;

	pObject = GetDynamicObject(_pMeshTag);
	if (pObject)
		return pObject;
	pObject = GetStaticObject(_pMeshTag);
	return pObject;
}

_bool CEditScene::DeleteStaticObject(_int _iObjectIndex)
{
	if (!IsValidStaticObjectIndex(_iObjectIndex))
		return false;

	m_vecStaticObjects[_iObjectIndex]->SetValid(false);
	m_vecStaticObjects.erase(m_vecStaticObjects.begin() + _iObjectIndex);

	return true;
}

_bool CEditScene::DeleteDynamicObject(_int _iObjectIndex)
{
	if (!IsValidDynamicObjectIndex(_iObjectIndex))
		return false;

	m_vecDynamicObjects[_iObjectIndex]->SetValid(false);
	m_vecDynamicObjects.erase(m_vecDynamicObjects.begin() + _iObjectIndex);

	return true;
}

_bool CEditScene::DeleteTriggerObject(_int _iObjectIndex)
{
	if (!IsValidTriggerObjectIndex(_iObjectIndex))
		return false;

	m_vecTriggerObjects[_iObjectIndex]->SetValid(false);
	m_vecTriggerObjects.erase(m_vecTriggerObjects.begin() + _iObjectIndex);

	return true;
}

//_bool CEditScene::DeleteCustomObject(_int _iObjectIndex)
//{
//	if (!IsValidCustomObjectIndex(_iObjectIndex))
//		return false;
//
//	m_vecCustomObjects[_iObjectIndex]->SetValid(false);
//	m_vecCustomObjects.erase(m_vecCustomObjects.begin() + _iObjectIndex);
//
//	return true;
//}

void CEditScene::SaveTerrain()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	CFileDialog Dlg(FALSE, L"trr", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.trr) | *.trr||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		GetTerrain()->SaveInfo(hFile);

		CloseHandle(hFile);
	}
}

void CEditScene::LoadTerrain()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"trr", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.trr) | *.trr||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Terrain");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		GetTerrain()->LoadInfo(hFile);

		CloseHandle(hFile);
	}
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
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"navi", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.navi) | *.navi||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Navi");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		GetNaviMesh()->LoadInfo(hFile);

		CloseHandle(hFile);
	}
	//UpdateData(FALSE);
}

void CEditScene::SaveMap()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	CFileDialog Dlg(FALSE, L"map", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.map) | *.map||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Map");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		
		DWORD dwByte = 0;
		
		// Static 저장
		_int iVecSize = m_vecStaticObjects.size();								
		WriteFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);						// vector 사이즈 저장

		_bool bIsCustomed = false;
		_int iStrLength = 0;
		for (auto& rStaticObj : m_vecStaticObjects) {
			rStaticObj->SaveInfo(hFile);														// 오브젝트 정보 저장

			// 커스텀 여부 저장
			bIsCustomed = rStaticObj->IsCustomed();
			WriteFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);				// 커스텀 여부 저장

			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					iStrLength = lstrlen(rStaticObj->GetFactorRef(i));							// 커스텀 팩터 저장
					WriteFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					WriteFile(hFile, rStaticObj->GetFactorRef(i), sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
				}
			}
		}
			
		
		// Dynamic 저장
		iVecSize = m_vecDynamicObjects.size();
		WriteFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
		for (auto& rDynamicObj : m_vecDynamicObjects) {
			rDynamicObj->SaveInfo(hFile);

			// 커스텀 여부 저장
			bIsCustomed = rDynamicObj->IsCustomed();
			WriteFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);

			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					iStrLength = lstrlen(rDynamicObj->GetFactorRef(i));
					WriteFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					WriteFile(hFile, rDynamicObj->GetFactorRef(i), sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
				}
			}
		}

		// Trigger 저장
		iVecSize = m_vecTriggerObjects.size();
		WriteFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
		for (auto& rTriggerObj : m_vecTriggerObjects) {
			rTriggerObj->SaveInfo(hFile);

			// 커스텀 여부 저장
			bIsCustomed = rTriggerObj->IsCustomed();
			WriteFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);

			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					iStrLength = lstrlen(rTriggerObj->GetFactorRef(i));
					WriteFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					WriteFile(hFile, rTriggerObj->GetFactorRef(i), sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
				}
			}
		}

		CloseHandle(hFile);
	}
}

void CEditScene::LoadMap()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"map", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.map) | *.map||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Map");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		// 기존 정보들은 파기한다.
		for (auto& rObj : m_vecStaticObjects) {
			rObj->SetValid(false);
		}
		m_vecStaticObjects.clear();
		for (auto& rObj : m_vecDynamicObjects) {
			rObj->SetValid(false);
		}
		m_vecDynamicObjects.clear();
		for (auto& rObj : m_vecTriggerObjects) {
			rObj->SetValid(false);
		}
		m_vecTriggerObjects.clear();

		// 객체 정보를 로드한다.
		DWORD dwByte = 0;

		// Static
		_int iVecSize = 0;
		ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);			// vector 사이즈 로드

		_bool bIsCustomed = false;
		_int iStrLength = 0;
		_tchar tcFactor[50] = L"";
		CStaticObject* pStaticObject = nullptr;
		for (_int i = 0; i < iVecSize; ++i) {
			pStaticObject = CStaticObject::Create(m_pGraphicDev);

			if (!pStaticObject->LoadInfo(hFile))
				Engine::Safe_Release(pStaticObject);
			else {
				GetLayer(L"EditLayer")->Add_GameObject(pStaticObject);
				m_vecStaticObjects.emplace_back(pStaticObject);
			}

			ReadFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);		// 커스텀 여부 저장
			pStaticObject->SetCustomed(bIsCustomed);
			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					ReadFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					ReadFile(hFile, tcFactor, sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
					pStaticObject->GetFactorRef(i) = tcFactor;
				}
			}
		}

		// Dynamic
		iVecSize = 0;
		ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
		CDynamicObject* pDynamicObject = nullptr;
		for (_int i = 0; i < iVecSize; ++i) {
			pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
			if (!pDynamicObject->LoadInfo(hFile))
				Engine::Safe_Release(pDynamicObject);
			else {
				GetLayer(L"EditLayer")->Add_GameObject(pDynamicObject);
				m_vecDynamicObjects.emplace_back(pDynamicObject);
			}

			ReadFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);		// 커스텀 여부 저장
			pDynamicObject->SetCustomed(bIsCustomed);
			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					ReadFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					ReadFile(hFile, tcFactor, sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
					pDynamicObject->GetFactorRef(i) = tcFactor;
				}
			}
		}

		// Trigger
		iVecSize = 0;
		ReadFile(hFile, &iVecSize, sizeof(iVecSize), &dwByte, nullptr);
		CTrigger* pTriggerObject = nullptr;
		for (_int i = 0; i < iVecSize; ++i) {
			pTriggerObject = CTrigger::Create(m_pGraphicDev);
			if (!pTriggerObject->LoadInfo(hFile))
				Engine::Safe_Release(pTriggerObject);
			else {
				GetLayer(L"EditLayer")->Add_GameObject(pTriggerObject);
				m_vecTriggerObjects.emplace_back(pTriggerObject);
			}

			ReadFile(hFile, &bIsCustomed, sizeof(bIsCustomed), &dwByte, nullptr);		// 커스텀 여부 저장
			pTriggerObject->SetCustomed(bIsCustomed);
			if (bIsCustomed) {
				for (_int i = 0; i < 6; ++i) {
					ReadFile(hFile, &iStrLength, sizeof(iStrLength), &dwByte, nullptr);
					ReadFile(hFile, tcFactor, sizeof(_tchar) * (iStrLength + 1), &dwByte, nullptr);
					pTriggerObject->GetFactorRef(i) = tcFactor;
				}
			}
		}

		CloseHandle(hFile);
	}
	//UpdateData(FALSE);
}

void CEditScene::SaveColliders(Engine::CGameObject* _pObject)
{
	if (!_pObject)
		return;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	CFileDialog Dlg(FALSE, L"col", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.col) | *.col||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_pObject->SaveCollidersInfo(hFile);

		CloseHandle(hFile);
	}
}

void CEditScene::LoadColliders(Engine::CGameObject* _pObject)
{
	if (!_pObject)
		return;

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"col", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.col) | *.col||", pTabForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFileW(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		_pObject->LoadCollidersInfo(hFile);

		CloseHandle(hFile);
	}
}

_bool CEditScene::IsValidStaticObjectIndex(_int _iObjectIndex)
{
	if (_iObjectIndex < 0 || _iObjectIndex >= static_cast<_int>(m_vecStaticObjects.size()))
		return false;
	return true;
}

_bool CEditScene::IsValidDynamicObjectIndex(_int _iObjectIndex)
{
	if (_iObjectIndex < 0 || _iObjectIndex >= static_cast<_int>(m_vecDynamicObjects.size()))
		return false;
	return true;
}

_bool CEditScene::IsValidTriggerObjectIndex(_int _iObjectIndex)
{
	if (_iObjectIndex < 0 || _iObjectIndex >= static_cast<_int>(m_vecTriggerObjects.size()))
		return false;
	return true;
}
