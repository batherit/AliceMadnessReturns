// CNaviMeshTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CNaviMeshTab.h"
#include "NaviMesh.h"
#include "EditScene.h"
#include "afxdialogex.h"


// CNaviMeshTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviMeshTab, CDialogEx)

CNaviMeshTab::CNaviMeshTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAVI_MESH_TAB, pParent)
{

}

CNaviMeshTab::~CNaviMeshTab()
{
}

void CNaviMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeNavi);
	DDX_Control(pDX, IDC_BUTTON1, m_btnDelete);
}


BEGIN_MESSAGE_MAP(CNaviMeshTab, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNaviMeshTab::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshTab::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CNaviMeshTab 메시지 처리기


void CNaviMeshTab::UpdateNaviTree(Client::CNaviMesh * _pNaviMesh)
{
	auto& rNaviVertices = _pNaviMesh->GetNaviVertices();
	_int iVerticesNum = rNaviVertices.size();

	CString cstTriangleIndex;
	CString cstVertexIndex;

	HTREEITEM hTriangleIndex = nullptr;

	m_treeNavi.DeleteAllItems();
	for (_int i = 0; i < iVerticesNum; ++i) {
		if (i % 3 == 0) {
			// TreeCtrl에 삼각형 인덱스 아이템을 삽입한다.
			cstTriangleIndex.Format(L"%d", i / 3);
			// hTriangeIndex는 hVertexIndex의 부모로 삼기위해 필요하다.
			hTriangleIndex = m_treeNavi.InsertItem(cstTriangleIndex, NULL, NULL);
		}

		cstVertexIndex.Format(L"%d", i % 3);
		m_treeNavi.InsertItem(cstVertexIndex, hTriangleIndex, NULL);
		m_treeNavi.Expand(hTriangleIndex, TVE_EXPAND);
	}
}

BOOL CNaviMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CNaviMeshTab::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_hSelectedTreeItem = pNMTreeView->itemNew.hItem;	// 선택된 아이템

	auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);

	if (hParent) {
		// 최상위 아이템이 아니라면,
		m_btnDelete.EnableWindow(FALSE);
	}
	else {
		m_btnDelete.EnableWindow(TRUE);
	}

	*pResult = 0;
}


void CNaviMeshTab::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_hSelectedTreeItem)
		return;

	CString strIndex = m_treeNavi.GetItemText(m_hSelectedTreeItem);
	_int iIndex = _ttoi(strIndex);
	
	m_treeNavi.DeleteItem(m_hSelectedTreeItem);

	// 네비메쉬 갱신
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
	pNaviMesh->PopTriangleVertices(iIndex);

	// 네비트리 갱신
	UpdateNaviTree(pNaviMesh);

	m_btnDelete.EnableWindow(FALSE);
}
