#include "pch.h"
#include "ColliderScene.h"
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
//#include "InputProcessor_Navi.h"

CColliderScene::CColliderScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CColliderScene::CColliderScene(const CColliderScene & rhs)
	:
	CScene(rhs)
{
}

CColliderScene::~CColliderScene(void)
{
	// 저장한다.
}

void CColliderScene::ResetScene(void)
{

}

HRESULT CColliderScene::Ready(void)
{
	

	return S_OK;
}

int CColliderScene::Update(const _float& fTimeDelta)
{
	return CScene::Update(fTimeDelta);
}

void CColliderScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject(m_pGraphicDev);
}

void CColliderScene::OnLoaded()
{
	Engine::CCollisionMgr::GetInstance()->ClearGameObjectList();
	// 저장되어 있던 정보를 읽어온다.
	for (auto& rObj : g_pTool3D_Kernel->m_vecStoredDynamicObjects_Collider) {
		m_vecDynamicObjects.emplace_back(rObj);
	}
	g_pTool3D_Kernel->m_vecStoredDynamicObjects_Collider.clear();

	for (auto& rObj : g_pTool3D_Kernel->m_vecStoredStaticObjects_Collider) {
		m_vecStaticObjects.emplace_back(rObj);
	}
	g_pTool3D_Kernel->m_vecStoredStaticObjects_Collider.clear();

	m_pSelectedObject = g_pTool3D_Kernel->m_pStoredSelectedObject_Collider;

	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer,);

	// 카메라 생성
	Engine::CGameObject* pGameObject = nullptr;
	pGameObject = CDynamicCamera::Create(m_pGraphicDev);		// 동적 카메라
	NULL_CHECK_RETURN(pGameObject,);
	//m_pCamera->SetParent(m_pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject),);

	// 저장된 정보를 레이어에 집어넣는다.
	for (auto& rObj : m_vecDynamicObjects) {
		pLayer->Add_GameObject(rObj);
		//Engine::Safe_AddRef(rObj);
	}
	for (auto& rObj : m_vecStaticObjects) {
		pLayer->Add_GameObject(rObj);
		//Engine::Safe_AddRef(rObj);
	}

	// 편집 레이어 등록
	m_mapLayer.emplace(L"ColliderLayer", pLayer);

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

void CColliderScene::OnExited()
{
	// 씬을 지우기 전에 오브젝트를 저장해둔다.
// 저장되어 있던 정보를 읽어온다.
	for (auto& rObj : m_vecDynamicObjects) {
		g_pTool3D_Kernel->m_vecStoredDynamicObjects_Collider.emplace_back(rObj);
		Engine::Safe_AddRef(rObj);
	}
	m_vecDynamicObjects.clear();
	m_vecDynamicObjects.shrink_to_fit();

	for (auto& rObj : m_vecStaticObjects) {
		g_pTool3D_Kernel->m_vecStoredStaticObjects_Collider.emplace_back(rObj);
		Engine::Safe_AddRef(rObj);
	}
	m_vecStaticObjects.clear();
	m_vecStaticObjects.shrink_to_fit();

	g_pTool3D_Kernel->m_pStoredSelectedObject_Collider = m_pSelectedObject;
}

CColliderScene * CColliderScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderScene*	pInstance = new CColliderScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CColliderScene::Free(void)
{

	CScene::Free();
}

//CNaviMeshInputProcessor * CColliderScene::GetNaviMeshInputProcessor() const
//{
//	auto pLayer = GetLayer(L"ColliderLayer");
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

//Engine::CGameObject * CColliderScene::GetPickedObject() const
//{
//	return GetTerrain();
//}

_bool CColliderScene::AddStaticObject(const _tchar * _pMeshTag)
{
	CStaticObject* pStaticObject = CStaticObject::Create(m_pGraphicDev);
	pStaticObject->SetRenderInfo(_pMeshTag);

	if (pStaticObject->GetComponent<Engine::CStaticMesh>()) {
		GetLayer(L"ColliderLayer")->Add_GameObject(pStaticObject);
		m_vecStaticObjects.emplace_back(pStaticObject);
		return true;
	}

	Engine::Safe_Release(pStaticObject);
	return false;
}

_bool CColliderScene::AddDynamicObject(const _tchar * _pMeshTag, const _vec3& _vPos)
{
	CDynamicObject* pDynamicObject = CDynamicObject::Create(m_pGraphicDev);
	pDynamicObject->SetRenderInfo(_pMeshTag);

	if (pDynamicObject->GetComponent<Engine::CDynamicMesh>()) {
		GetLayer(L"ColliderLayer")->Add_GameObject(pDynamicObject);
		pDynamicObject->GetTransform()->SetPos(_vPos);
		m_vecDynamicObjects.emplace_back(pDynamicObject);
		return true;
	}

	Engine::Safe_Release(pDynamicObject);
	return false;
}

CStaticObject * CColliderScene::GetStaticObject(_int _iObjectIndex)
{
	if (!IsValidObjectIndex(_iObjectIndex))
		return nullptr;

	return m_vecStaticObjects[_iObjectIndex];
}

CStaticObject * CColliderScene::GetStaticObject(const _tchar * _pMeshTag)
{
	for (auto& rObj : m_vecStaticObjects) {
		if (lstrcmp(rObj->GetMeshTag(), _pMeshTag) == 0) {
			return rObj;
		}
	}

	return nullptr;
}

CDynamicObject * CColliderScene::GetDynamicObject(const _tchar * _pMeshTag)
{
	for (auto& rObj : m_vecDynamicObjects) {
		if (lstrcmp(rObj->GetMeshTag(), _pMeshTag) == 0) {
			return rObj;
		}
	}

	return nullptr;
}

Engine::CGameObject * CColliderScene::GetObjectFromTag(const _tchar * _pMeshTag)
{
	Engine::CGameObject* pObject = nullptr;

	pObject = GetDynamicObject(_pMeshTag);
	if (pObject)
		return pObject;
	pObject = GetStaticObject(_pMeshTag);
	return pObject;
}

_bool CColliderScene::ActivateObject(const _tchar * _pMeshTag)
{
	if (m_pSelectedObject)
		// 기존 것은 비활성화한다.
		m_pSelectedObject->SetActivated(false);

	m_pSelectedObject = GetObjectFromTag(_pMeshTag);
	
	if (!m_pSelectedObject) {
		return false;
	}
	
	m_pSelectedObject->SetActivated(true);

	return false;
}

_bool CColliderScene::DeleteStaticObject(_int _iObjectIndex)
{
	if (!IsValidObjectIndex(_iObjectIndex))
		return false;

	m_vecStaticObjects[_iObjectIndex]->SetValid(false);
	m_vecStaticObjects.erase(m_vecStaticObjects.begin() + _iObjectIndex);

	return true;
}




void CColliderScene::SaveColliders(Engine::CGameObject* _pObject)
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

void CColliderScene::LoadColliders(Engine::CGameObject* _pObject)
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

_bool CColliderScene::IsValidObjectIndex(_int _iObjectIndex)
{
	if (_iObjectIndex < 0 || _iObjectIndex >= static_cast<_int>(m_vecStaticObjects.size()))
		return false;
	return true;
}
