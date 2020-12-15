// CColliderTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CColliderTab.h"
#include "CSphereColTab.h"
#include "CAABBColTab.h"
#include "COBBColTab.h"
#include "afxdialogex.h"
//#include "ColliderScene.h"
#include "ColliderScene.h"
#include "StaticObject.h"
#include "DynamicObject.h"

// CColliderTab 대화 상자

IMPLEMENT_DYNAMIC(CColliderTab, CDialogEx)

CColliderTab::CColliderTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLLIDER_TAB, pParent)
	, m_cstrColliderTag(_T(""))
{

}

CColliderTab::~CColliderTab()
{
	// OnDestroy가 안돼서 여기에다가 소멸 코드 넣음.
	for (auto& pColTab : m_vecColTabs) {
		Safe_Delete(pColTab);
	}
	m_vecColTabs.clear();
	m_vecColTabs.shrink_to_fit();
}

void CColliderTab::GenerateBoneTree(D3DXFRAME * _pFrame, HTREEITEM _treeParentItem)
{
	if (!_pFrame)
		return;

	CString strBoneName = CString(_pFrame->Name);
	HTREEITEM treeBoneItem = m_treeBoneTree.InsertItem(strBoneName, _treeParentItem, NULL);
	m_treeBoneTree.Expand(_treeParentItem, TVE_EXPAND);

	if (_pFrame->pFrameSibling) {
		strBoneName = CString(_pFrame->pFrameSibling->Name);
		GenerateBoneTree(_pFrame->pFrameSibling, _treeParentItem);
	}

	if (_pFrame->pFrameFirstChild) {
		strBoneName = CString(_pFrame->pFrameFirstChild->Name);
		GenerateBoneTree(_pFrame->pFrameFirstChild, treeBoneItem);
	}
}

void CColliderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_treeObjectList);
	DDX_Control(pDX, IDC_TREE1, m_treeBoneTree);
	DDX_Control(pDX, IDC_TAB2, m_ColTab);
	DDX_Control(pDX, IDC_TREE3, m_treeColliders);
	DDX_Control(pDX, IDC_BUTTON10, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDelete);
	DDX_Control(pDX, IDC_EDIT1, m_editColliderTag);
	DDX_Text(pDX, IDC_EDIT1, m_cstrColliderTag);
}

void CColliderTab::ActivateColTab(const Engine::E_COLLIDER_TYPE _eColTabType)
{
	for (auto& pColTab : m_vecColTabs) {
		pColTab->ShowWindow(SW_HIDE);
	}

	m_ColTab.SetCurSel(_eColTabType);
	m_vecColTabs[_eColTabType]->ShowWindow(SW_SHOW);
}

void CColliderTab::UpdateColliderTag(Engine::CColliderObject * _pCollider)
{
	UpdateData(TRUE);
	m_cstrColliderTag = _pCollider->GetColliderTag();
	UpdateData(FALSE);
}

void CColliderTab::UpdateBoneTree(Engine::CGameObject* _pObject)
{
	m_treeBoneTree.DeleteAllItems();
	m_treeBoneTree.InsertItem(L"None", NULL, NULL);

	CDynamicObject* pDynamicObject = dynamic_cast<CDynamicObject*>(_pObject);
	if(pDynamicObject)
		GenerateBoneTree(pDynamicObject->GetDynamicMesh()->GetRootFrame());
}

void CColliderTab::UpdateAttachedColliders(Engine::CGameObject * _pObject)
{
	if (!_pObject)
		return;

	m_treeColliders.DeleteAllItems();
	CString strTemp;
	auto& rColliders = _pObject->GetColliderList();
	HTREEITEM treeBoneItem;
	_int iColliderIndex = 0;
	for (auto& rPair : rColliders) {
		strTemp = rPair.first.c_str();	// const char*형의 본 이름을 CString 형태로 전환
		treeBoneItem = m_treeColliders.InsertItem(strTemp, NULL, NULL);
		iColliderIndex = 0;
		for (auto& rObj : rPair.second) {
			strTemp.Format(L"%d", iColliderIndex);
			m_treeColliders.InsertItem(strTemp, treeBoneItem, NULL);
			++iColliderIndex;
		}
	}
}

void CColliderTab::RegisterMeshTag(Engine::MESHTYPE _eMeshType, const _tchar * _pMeshTag)
{
	switch (_eMeshType) {
	case Engine::TYPE_STATIC:
		m_treeObjectList.InsertItem(_pMeshTag, m_itemStatic, NULL);
		break;
	case Engine::TYPE_DYNAMIC:
		m_treeObjectList.InsertItem(_pMeshTag, m_itemDynamic, NULL);
		break;
	default:
		break;
	}
}


BEGIN_MESSAGE_MAP(CColliderTab, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_TREE2, &CColliderTab::OnNMClickTreeObjectList)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CColliderTab::OnNMClickTreeBoneTree)
//	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON10, &CColliderTab::OnBnClickedButtonAdd)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CColliderTab::OnTcnSelchangeColTab)
	ON_BN_CLICKED(IDC_BUTTON1, &CColliderTab::OnBnClickedButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_TREE3, &CColliderTab::OnNMClickTreeColliders)
	ON_EN_CHANGE(IDC_EDIT1, &CColliderTab::OnEnChangeEditColliderTag)
	ON_BN_CLICKED(IDC_BUTTON7, &CColliderTab::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON8, &CColliderTab::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CColliderTab 메시지 처리기


BOOL CColliderTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 오브젝트 리스트 트리에 Static과 Dynamic 항목을 추가합니다.
	m_itemStatic = m_treeObjectList.InsertItem(L"Static", NULL, NULL);
	m_itemDynamic = m_treeObjectList.InsertItem(L"Dynamic", NULL, NULL);

	m_ColTab.InsertItem(0, _T("Sphere"));
	m_ColTab.InsertItem(1, _T("AABB"));
	m_ColTab.InsertItem(2, _T("OBB"));
	

	CRect rect;
	m_ColTab.GetWindowRect(&rect);

	m_pSphereColTab = new CSphereColTab;
	m_pSphereColTab->Create(IDD_SPHERE_COLTAB, &m_ColTab);
	m_pSphereColTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pSphereColTab->m_pColTab = this;
	m_vecColTabs.emplace_back(m_pSphereColTab);

	m_pAABBColTab = new CAABBColTab;
	m_pAABBColTab->Create(IDD_AABB_COLTAB, &m_ColTab);
	m_pAABBColTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pAABBColTab->m_pColTab = this;
	m_vecColTabs.emplace_back(m_pAABBColTab);

	m_pOBBColTab = new COBBColTab;
	m_pOBBColTab->Create(IDD_OBB_COLTAB, &m_ColTab);
	m_pOBBColTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pOBBColTab->m_pColTab = this;
	m_vecColTabs.emplace_back(m_pOBBColTab);

	// 초기 터레인 탭으로 설정
	ActivateColTab(Engine::TYPE_SPHERE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColliderTab::OnNMClickTreeObjectList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Add버튼을 비활성화합니다.
	m_btnAdd.EnableWindow(FALSE);
	// Delete버튼을 비활성화합니다.
	m_btnDelete.EnableWindow(FALSE);

	// 선택된 아이템을 해제합니다.
	m_hSelectedMesh = NULL;
	m_hSelectedBone = NULL;
	m_hSelectedCollider = NULL;
	m_pSelectedCollider = nullptr;
	m_editColliderTag.EnableWindow(FALSE);
	m_pSphereColTab->EnableTab(FALSE);
	m_pAABBColTab->EnableTab(FALSE);
	m_pOBBColTab->EnableTab(FALSE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeObjectList.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeObjectList.HitTest(point, &nFlags);

	if (!hItem)
		return;

	m_hSelectedMesh = hItem;

	hItem = m_treeObjectList.GetParentItem(m_hSelectedMesh);

	m_treeBoneTree.DeleteAllItems();
	m_treeColliders.DeleteAllItems();
	if (hItem) {
		// 부모가 있다면, 메쉬 태그를 선택한 것이다.
		if (m_treeObjectList.GetItemText(hItem) == L"Dynamic") {
			CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
			CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
			CDynamicObject* pDynamicObject = pColliderScene->GetDynamicObject(strMeshTag);
			if (!pDynamicObject) {
				if (pColliderScene->AddDynamicObject(strMeshTag)) {
					// 생성해둔 동적 메쉬가 없다면, 동적 메쉬를 생성한다.
					pDynamicObject = pColliderScene->GetDynamicObject(strMeshTag);
				}
				else {
					// 오브젝트 리스트에 있는데 생성을 못한다? => 이상한 걸 뻥!
					abort();
				}
			}

			pColliderScene->ActivateObject(pDynamicObject->GetMeshTag());
			UpdateBoneTree(pDynamicObject);
			UpdateAttachedColliders(pDynamicObject);
		}
		else if (m_treeObjectList.GetItemText(hItem) == L"Static") {
			CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
			CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
			CStaticObject* pStaticObject = pColliderScene->GetStaticObject(strMeshTag);
			if (!pStaticObject) {
				if (pColliderScene->AddStaticObject(strMeshTag)) {
					// 생성해둔 정적 메쉬가 없다면, 정적 메쉬를 생성한다.
					pStaticObject = pColliderScene->GetStaticObject(strMeshTag);
				}
				else {
					// 오브젝트 리스트에 있는데 생성을 못한다? => 이상한 걸 뻥!
					abort();
				}
			}

			pColliderScene->ActivateObject(pStaticObject->GetMeshTag());
			UpdateBoneTree(pStaticObject);
			UpdateAttachedColliders(pStaticObject);
		}
		else
			// Dynamic, Static 부모밖에 없는데 다른게 클릭되었다구? => 펑!
			abort();
	}
	else {
		// 부모가 없다면, 메쉬 태그와 상관없는 항목을 픽킹한 것이다.
		m_hSelectedMesh = NULL;
	}

	*pResult = 0;
}

void CColliderTab::OnNMClickTreeBoneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Add버튼을 일단 비활성화한다.
	m_btnAdd.EnableWindow(FALSE);
	// Delete버튼을 비활성화합니다.
	m_btnDelete.EnableWindow(FALSE);
	m_hSelectedBone = NULL;
	m_hSelectedCollider = NULL;
	m_pSelectedCollider = nullptr;
	m_editColliderTag.EnableWindow(FALSE);
	m_pSphereColTab->EnableTab(FALSE);
	m_pAABBColTab->EnableTab(FALSE);
	m_pOBBColTab->EnableTab(FALSE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeBoneTree.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeBoneTree.HitTest(point, &nFlags);

	if (!hItem)
		return;

	m_hSelectedBone = hItem;
	// Add버튼을 활성화한다.
	m_btnAdd.EnableWindow(TRUE);

	*pResult = 0;
}


//void CColliderTab::OnDestroy()
//{
//	
//
//	CDialogEx::OnDestroy();
//
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// => 이 함수는 호출이 안되더라.
//}


void CColliderTab::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_hSelectedMesh와 m_hSelectedBone이 적절히 세팅되어 있음이 보장되어야 합니다.
	// m_hSelectedMesh를 통해 DynamicObject를 얻어옵니다.
	// m_hSelectedBone을 통해 해당 뼈에 콜라이더를 붙입니다.

	m_editColliderTag.EnableWindow(FALSE);
	m_pSphereColTab->EnableTab(FALSE);
	m_pAABBColTab->EnableTab(FALSE);
	m_pOBBColTab->EnableTab(FALSE);

	CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
	CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
	Engine::CGameObject* pObject = pColliderScene->GetObjectFromTag(strMeshTag);

	CString strBoneName = m_treeBoneTree.GetItemText(m_hSelectedBone);

	m_pSelectedCollider = nullptr;
	switch (static_cast<Engine::E_COLLIDER_TYPE>(m_ColTab.GetCurSel())) {
	case Engine::TYPE_SPHERE:
		m_pSelectedCollider = Engine::CColliderObject_Sphere::Create(g_pTool3D_Kernel->GetGraphicDev());
		dynamic_cast<Engine::CColliderObject_Sphere*>(m_pSelectedCollider)->SetRadius(25.f);
		m_pSphereColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_Sphere*>(m_pSelectedCollider));
		m_pSphereColTab->EnableTab(TRUE);
		break;
	case Engine::TYPE_AABB:
		// TODO : AABB 콜라이더 장착 코드 구현
		m_pSelectedCollider = Engine::CColliderObject_AABB::Create(g_pTool3D_Kernel->GetGraphicDev());
		m_pSelectedCollider->GetTransform()->SetScale(_vec3(25.f, 25.f, 25.f));
		m_pAABBColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_AABB*>(m_pSelectedCollider));
		m_pAABBColTab->EnableTab(TRUE);
		break;
	case Engine::TYPE_OBB:
		// TODO : OBB 콜라이더 장착 코드 구현
		m_pSelectedCollider = Engine::CColliderObject_OBB::Create(g_pTool3D_Kernel->GetGraphicDev());
		m_pSelectedCollider->GetTransform()->SetScale(_vec3(25.f, 25.f, 25.f));
		m_pOBBColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_OBB*>(m_pSelectedCollider));
		m_pOBBColTab->EnableTab(TRUE);
		break;
	}
	UpdateColliderTag(m_pSelectedCollider);
	m_editColliderTag.EnableWindow(TRUE);

	CStringA straConv(strBoneName);
	const char* szBoneName = straConv;
	if (strBoneName == L"None") {
		pObject->AddCollider(m_pSelectedCollider);
	}
	else {
		pObject->AddCollider(m_pSelectedCollider, szBoneName);
	}
	UpdateAttachedColliders(pObject);
}




void CColliderTab::OnTcnSelchangeColTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ActivateColTab(static_cast<Engine::E_COLLIDER_TYPE>(m_ColTab.GetCurSel()));

	*pResult = 0;
}


void CColliderTab::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_hSelectedMesh, m_hSelectedBone, m_hSelectedCollider이 유효하다는 것을 보장해야 합니다.

	CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
	CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
	Engine::CGameObject* pObject = pColliderScene->GetObjectFromTag(strMeshTag);

	CString cstrBoneName = m_treeColliders.GetItemText(m_treeColliders.GetParentItem(m_hSelectedCollider));
	_int iColliderIndex = _ttoi(m_treeColliders.GetItemText(m_hSelectedCollider));

	if (pObject) {
		auto& rColliderList = pObject->GetColliderList();
		if (!rColliderList.empty()) {
			for (auto& iter = rColliderList.begin(); iter != rColliderList.end(); ++iter) {
				if (cstrBoneName == iter->first.c_str()) {
					Engine::Safe_Release(iter->second[iColliderIndex]);
					iter->second.erase(iter->second.begin() + iColliderIndex);

					if (iter->second.empty()) {
						// 해당 본에 충돌체가 존재하지 않는다면, iter부분을 리스트에서 제거한다.
						rColliderList.erase(iter);
					}

					// 콜라이더 트리를 새로 갱신한다.
					UpdateAttachedColliders(pObject);

					m_hSelectedCollider = NULL;
					m_pSelectedCollider = nullptr;
					m_editColliderTag.EnableWindow(FALSE);
					m_pSphereColTab->EnableTab(FALSE);
					m_pAABBColTab->EnableTab(FALSE);
					m_pOBBColTab->EnableTab(FALSE);
					break;
				}
			}
		}
	}

	
	m_btnDelete.EnableWindow(FALSE);
}


void CColliderTab::OnNMClickTreeColliders(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Delete버튼을 비활성화합니다.
	m_btnDelete.EnableWindow(FALSE);
	m_hSelectedCollider = NULL;
	m_pSelectedCollider = nullptr;
	m_editColliderTag.EnableWindow(FALSE);
	m_pSphereColTab->EnableTab(FALSE);
	m_pAABBColTab->EnableTab(FALSE);
	m_pOBBColTab->EnableTab(FALSE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeColliders.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeColliders.HitTest(point, &nFlags);

	if (!hItem)
		return;

	m_hSelectedCollider = hItem;

	hItem = m_treeObjectList.GetParentItem(m_hSelectedCollider);

	if (hItem) {
		// 충돌체를 클릭했다는 것이다. => 삭제할 것이 골라졌으므로 삭제 버튼 활성화
		CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
		CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
		Engine::CGameObject* pObject = pColliderScene->GetObjectFromTag(strMeshTag);

		CString cstrBoneName = m_treeColliders.GetItemText(m_treeColliders.GetParentItem(m_hSelectedCollider));
		_int iColliderIndex = _ttoi(m_treeColliders.GetItemText(m_hSelectedCollider));

		if (pObject) {
			auto& rColliderList = pObject->GetColliderList();
			if (!rColliderList.empty()) {
				for (auto& iter = rColliderList.begin(); iter != rColliderList.end(); ++iter) {
					if (cstrBoneName == iter->first.c_str()) {
						// 찾은 충돌체를 저장해둔다.
						m_pSelectedCollider = iter->second[iColliderIndex];

						switch (m_pSelectedCollider->GetColliderType()) {
						case Engine::TYPE_SPHERE:
							ActivateColTab(Engine::TYPE_SPHERE);
							m_pSphereColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_Sphere*>(m_pSelectedCollider));
							m_pSphereColTab->EnableTab(TRUE);
							break;
						case Engine::TYPE_AABB:
							ActivateColTab(Engine::TYPE_AABB);
							m_pAABBColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_AABB*>(m_pSelectedCollider));
							m_pAABBColTab->EnableTab(TRUE);
							break;
						case Engine::TYPE_OBB:
							ActivateColTab(Engine::TYPE_OBB);
							m_pOBBColTab->UpdateTab(dynamic_cast<Engine::CColliderObject_OBB*>(m_pSelectedCollider));
							m_pOBBColTab->EnableTab(TRUE);
							break;
						}

						UpdateColliderTag(m_pSelectedCollider);
						m_editColliderTag.EnableWindow(TRUE);
						break;
					}
				}
			}
		}
		
		m_btnDelete.EnableWindow(TRUE);
	}
	else {
		m_hSelectedCollider = NULL;
		m_pSelectedCollider = nullptr;
	}

	*pResult = 0;
}





void CColliderTab::OnEnChangeEditColliderTag()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_pSelectedCollider) {
		m_pSelectedCollider->SetColliderTag(m_cstrColliderTag);
	}
	UpdateData(FALSE);
}


void CColliderTab::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
	CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
	Engine::CGameObject* pObject = pColliderScene->GetObjectFromTag(strMeshTag);

	pColliderScene->SaveColliders(pObject);
}


void CColliderTab::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
	CColliderScene* pColliderScene = g_pTool3D_Kernel->GetColliderScene();
	Engine::CGameObject* pObject = pColliderScene->GetObjectFromTag(strMeshTag);

	pColliderScene->LoadColliders(pObject);
	
	UpdateAttachedColliders(pObject);
}
