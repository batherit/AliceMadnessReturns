// CColliderTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CColliderTab.h"
#include "CSphereColTab.h"
#include "afxdialogex.h"
#include "EditScene.h"
#include "DynamicObject.h"

// CColliderTab 대화 상자

IMPLEMENT_DYNAMIC(CColliderTab, CDialogEx)

CColliderTab::CColliderTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLLIDER_TAB, pParent)
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
	DDX_Control(pDX, IDC_BUTTON1, m_btdDelete);
}

void CColliderTab::ActivateColTab(const Engine::E_COLLIDER_TYPE _eColTabType)
{
	for (auto& pColTab : m_vecColTabs) {
		pColTab->ShowWindow(SW_HIDE);
	}

	m_ColTab.SetCurSel(_eColTabType);
	m_vecColTabs[_eColTabType]->ShowWindow(SW_SHOW);
}

void CColliderTab::UpdateBoneTree(CDynamicObject* _pDynamicObject)
{
	m_treeBoneTree.DeleteAllItems();
	m_treeBoneTree.InsertItem(L"None", NULL, NULL);
	GenerateBoneTree(_pDynamicObject->GetDynamicMesh()->GetRootFrame());
}

void CColliderTab::UpdateAttachedColliders(CDynamicObject * _pDynamicObject)
{
	if (!_pDynamicObject)
		return;

	m_treeColliders.DeleteAllItems();
	CString strTemp;
	auto& CollidersMap = _pDynamicObject->GetColliderMap();
	HTREEITEM treeBoneItem;
	_int iColliderIndex = 0;
	for (auto& rMap : CollidersMap) {
		strTemp = rMap.first;	// const char*형의 본 이름을 CString 형태로 전환
		treeBoneItem = m_treeColliders.InsertItem(strTemp, NULL, NULL);
		iColliderIndex = 0;
		for (auto& rObj : rMap.second) {
			strTemp.Format(L"%d", iColliderIndex);
			m_treeColliders.InsertItem(strTemp, treeBoneItem, NULL);
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
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON10, &CColliderTab::OnBnClickedButtonAdd)
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
	//m_Tab.InsertItem(1, _T("Navi Mesh"));
	//m_Tab.InsertItem(2, _T("Map"));
	

	CRect rect;
	m_ColTab.GetWindowRect(&rect);

	m_pSphereColTab = new CSphereColTab;
	m_pSphereColTab->Create(IDD_SPHERE_COLTAB, &m_ColTab);
	m_pSphereColTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecColTabs.emplace_back(m_pSphereColTab);

	/*m_pNaviMeshTab = new CNaviMeshTab;
	m_pNaviMeshTab->Create(IDD_NAVI_MESH_TAB, &m_Tab);
	m_pNaviMeshTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pNaviMeshTab);

	m_pMapTab = new CMapTab;
	m_pMapTab->Create(IDD_MAP_TAB, &m_Tab);
	m_pMapTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pMapTab);*/

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

	// 선택된 아이템을 해제합니다.
	m_hSelectedMesh = NULL;
	m_hSelectedBone = NULL;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeObjectList.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeObjectList.HitTest(point, &nFlags);

	if (!hItem)
		return;

	m_hSelectedMesh = hItem;

	hItem = m_treeObjectList.GetParentItem(m_hSelectedMesh);

	m_treeBoneTree.DeleteAllItems();
	if (hItem) {
		// 부모가 있다면, 메쉬 태그를 선택한 것이다.
		if (m_treeObjectList.GetItemText(hItem) == L"Dynamic") {
			CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
			CEditScene* pEditScene = g_pTool3D_Kernel->GetEditScene();
			CDynamicObject* pDynamicObject = pEditScene->GetDynamicObject(strMeshTag);

			if (!pDynamicObject) {
				if (pEditScene->AddDynamicObject(strMeshTag)) {
					// 생성해둔 동적 메쉬가 없다면, 동적 메쉬를 생성한다.
					pDynamicObject = pEditScene->GetDynamicObject(strMeshTag);
					
				}
				else {
					// 오브젝트 리스트에 있는데 생성을 못한다? => 이상한 걸 뻥!
					abort();
				}
			}
			
			UpdateBoneTree(pDynamicObject);
			UpdateAttachedColliders(pDynamicObject);
		}
	}
	else {
		// 부모가 없다면, 메쉬 태그와 상관없는 항목을 픽킹한 것이다.
		m_hSelectedMesh = 0;
	}

	*pResult = 0;
}

void CColliderTab::OnNMClickTreeBoneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Add버튼을 일단 비활성화한다.
	m_btnAdd.EnableWindow(FALSE);

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


void CColliderTab::OnDestroy()
{
	

	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// => 이 함수는 호출이 안되더라.
}


void CColliderTab::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// m_hSelectedMesh와 m_hSelectedBone이 적절히 세팅되어 있음이 보장되어야 합니다.
	// m_hSelectedMesh를 통해 DynamicObject를 얻어옵니다.
	// m_hSelectedBone을 통해 해당 뼈에 콜라이더를 붙입니다.

	CString strMeshTag = m_treeObjectList.GetItemText(m_hSelectedMesh);
	CEditScene* pEditScene = g_pTool3D_Kernel->GetEditScene();
	CDynamicObject* pDynamicObject = pEditScene->GetDynamicObject(strMeshTag);

	CString strBoneName = m_treeBoneTree.GetItemText(m_hSelectedBone);

	Engine::CColliderObject* pCollider = nullptr;
	switch (static_cast<Engine::E_COLLIDER_TYPE>(m_ColTab.GetCurSel())) {
	case Engine::TYPE_SPHERE:
		pCollider = Engine::CColliderObject_Sphere::Create(g_pTool3D_Kernel->GetGraphicDev());
		dynamic_cast<Engine::CColliderObject_Sphere*>(pCollider)->SetRadius(50.f);
		break;
	}

	if (strBoneName == L"None") {
		pDynamicObject->AddCollider(pCollider);
	}
	else {
		CStringA straConv(strBoneName);
		const char* szBoneName = straConv;
		pDynamicObject->AddCollider(pCollider, szBoneName);
	}
	UpdateAttachedColliders(pDynamicObject);
}


