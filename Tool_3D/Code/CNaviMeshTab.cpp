﻿// CNaviMeshTab.cpp: 구현 파일
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
	DDX_Text(pDX, IDC_EDIT1, m_vVertexPos.x);
	DDX_Text(pDX, IDC_EDIT10, m_vVertexPos.y);
	DDX_Text(pDX, IDC_EDIT3, m_vVertexPos.z);
	DDX_Control(pDX, IDC_EDIT1, m_editPosX);
	DDX_Control(pDX, IDC_EDIT10, m_editPosY);
	DDX_Control(pDX, IDC_EDIT3, m_editPosZ);
}


BEGIN_MESSAGE_MAP(CNaviMeshTab, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNaviMeshTab::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshTab::OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT1, &CNaviMeshTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT10, &CNaviMeshTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CNaviMeshTab::OnEnChangeEditPosZ)
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
		// 삼각형의 정점 중 하나를 선택한 경우
		m_btnDelete.EnableWindow(FALSE);

		_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
		_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));

		// Position Edit Ctrl 활성화
		m_editPosX.EnableWindow(TRUE);
		m_editPosY.EnableWindow(TRUE);
		m_editPosZ.EnableWindow(TRUE);

		// Position Edit Ctrl 갱신
		UpdateData(TRUE);
		CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
		auto& rVertices = pNaviMesh->GetNaviVertices();
		m_vVertexPos = rVertices[iTriangleIndex * 3 + iVertexIndex];
		UpdateData(FALSE);
	}
	else {
		// 삼각형 자체를 선택한 경우
		m_btnDelete.EnableWindow(TRUE);
		CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

		CString strIndex = m_treeNavi.GetItemText(m_hSelectedTreeItem);
		_int iIndex = _ttoi(strIndex);
		pNaviMesh->MarkTriangle(iIndex);

		// Position Edit Ctrl 비활성화
		m_editPosX.EnableWindow(FALSE);
		m_editPosY.EnableWindow(FALSE);
		m_editPosZ.EnableWindow(FALSE);
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


void CNaviMeshTab::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//m_hSelectedTreeItem;	=> 삼각형의 정점 중 하나로 해석된다.
	auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);
	_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
	_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

	UpdateData(TRUE);

	pNaviMesh->SetTriangleVertexPosition(iTriangleIndex, iVertexIndex, m_vVertexPos);

	UpdateData(FALSE);
}


void CNaviMeshTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//m_hSelectedTreeItem;	=> 삼각형의 정점 중 하나로 해석된다.
	auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);
	_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
	_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

	UpdateData(TRUE);

	pNaviMesh->SetTriangleVertexPosition(iTriangleIndex, iVertexIndex, m_vVertexPos);

	UpdateData(FALSE);
}


void CNaviMeshTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//m_hSelectedTreeItem;	=> 삼각형의 정점 중 하나로 해석된다.
	auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);
	_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
	_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

	UpdateData(TRUE);

	pNaviMesh->SetTriangleVertexPosition(iTriangleIndex, iVertexIndex, m_vVertexPos);

	UpdateData(FALSE);
}