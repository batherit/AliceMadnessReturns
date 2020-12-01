// CMapTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMapTab.h"
#include "EditScene.h"
#include "afxdialogex.h"


// CMapTab 대화 상자

IMPLEMENT_DYNAMIC(CMapTab, CDialogEx)

CMapTab::CMapTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_TAB, pParent)
{

}

CMapTab::~CMapTab()
{
}

void CMapTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeObjectList);
	DDX_Control(pDX, IDC_TREE2, m_treeAddedObject);
}


BEGIN_MESSAGE_MAP(CMapTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTab::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTab::OnBnClickedButtonDelete)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CMapTab::OnNMClickTreeObjectToAdd)
END_MESSAGE_MAP()


// CMapTab 메시지 처리기


void CMapTab::OnBnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_hSelectedTreeItem == nullptr)
		return;

	//const _tchar* pMeshTag =  m_treeObjectList.GetItemText(m_hSelectedTreeItem);
	CString pMeshTag = m_treeObjectList.GetItemText(m_hSelectedTreeItem);
	if (g_pTool3D_Kernel->GetEditScene()->AddStaticObject(pMeshTag)) {
		m_treeAddedObject.InsertItem(pMeshTag, NULL, NULL);
	}
}


void CMapTab::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTab::OnNMClickTreeObjectToAdd(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeObjectList.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeObjectList.HitTest(point, &nFlags);

	if (!hItem)
		return;

	//m_hSelectedTreeItem = pNMTreeView->itemNew.hItem;	// 선택된 아이템
	m_hSelectedTreeItem = hItem;

	*pResult = 0;
}
