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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CColliderTab::OnTcnSelchangeColTab)
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
	*pResult = 0;

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

			if (pDynamicObject)
				// 생성해둔 동적 메쉬가 있다면, 그 메쉬로 본트리를 갱신한다.
				UpdateBoneTree(pDynamicObject);
			else {
				if (pEditScene->AddDynamicObject(strMeshTag)) {
					// 생성해둔 동적 메쉬가 없다면, 생성하고 본트리를 갱신한다.
					pDynamicObject = pEditScene->GetDynamicObject(strMeshTag);
					UpdateBoneTree(pDynamicObject);
				}
				else {
					// 오브젝트 리스트에 있는데 생성을 못한다? => 이상한 걸 뻥!
					abort();
				}
			}
		}
	}
	else {
		// 부모가 없다면, 메쉬 태그와 상관없는 항목을 픽킹한 것이다.
		m_hSelectedMesh = 0;
	}
}


void CColliderTab::OnTcnSelchangeColTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CColliderTab::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for (auto& pColTab : m_vecColTabs) {
		Safe_Delete(pColTab);
	}
	m_vecColTabs.clear();
	m_vecColTabs.shrink_to_fit();
}


void CColliderTab::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
