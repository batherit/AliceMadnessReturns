// CNaviMeshTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CNaviMeshTab.h"
#include "NaviMesh.h"
#include "InputProcessor_Navi.h"
//#include "NaviMeshInputProcessor.h"
//#include "NaviMeshVtxCtrl.h"
#include "NaviMeshVtxMover.h"
#include "EditScene.h"
#include "afxdialogex.h"


// CNaviMeshTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviMeshTab, CDialogEx)

CNaviMeshTab::CNaviMeshTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAVI_MESH_TAB, pParent)
	, m_bIsGrouping(FALSE)
	, m_fGroupRange(0)
	//, m_chkNaviMagnet(FALSE)
	, m_bIsNaviMagnet(FALSE)
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
	DDX_Control(pDX, IDC_BUTTON2, m_btnCombine);
	DDX_Control(pDX, IDC_BUTTON4, m_btnCancel);
	DDX_Check(pDX, IDC_CHECK2, m_bIsGrouping);
	DDX_Control(pDX, IDC_CHECK2, m_chkGroup);
	DDX_Text(pDX, IDC_EDIT5, m_fGroupRange);
	DDX_Control(pDX, IDC_EDIT5, m_editGroupRange);
	DDX_Control(pDX, IDC_RADIO3, m_rbtnNavi);
	DDX_Control(pDX, IDC_RADIO4, m_rbtnVertex);
	DDX_Control(pDX, IDC_RADIO5, m_rbtnTriangle);
	//DDX_Check(pDX, IDC_CHECK1, m_chkNaviMagnet);
	DDX_Check(pDX, IDC_CHECK1, m_bIsNaviMagnet);
}


BEGIN_MESSAGE_MAP(CNaviMeshTab, CDialogEx)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNaviMeshTab::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshTab::OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT1, &CNaviMeshTab::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT10, &CNaviMeshTab::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT3, &CNaviMeshTab::OnEnChangeEditPosZ)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviMeshTab::OnBnClickedButtonCombine)
	ON_BN_CLICKED(IDC_BUTTON4, &CNaviMeshTab::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_CHECK2, &CNaviMeshTab::OnBnClickedCheckGroup)
	ON_EN_CHANGE(IDC_EDIT5, &CNaviMeshTab::OnEnChangeEditGroupRange)
	//ON_NOTIFY(NM_KILLFOCUS, IDC_TREE1, &CNaviMeshTab::OnNMKillfocusTree1)
//	ON_WM_ACTIVATE()
//ON_WM_ENABLE()
	ON_BN_CLICKED(IDC_RADIO3, &CNaviMeshTab::OnBnClickedRadioNavi)
	ON_BN_CLICKED(IDC_CHECK1, &CNaviMeshTab::OnBnClickedCheckNaviMagnet)
	ON_BN_CLICKED(IDC_RADIO4, &CNaviMeshTab::OnBnClickedRadioVertex)
	ON_BN_CLICKED(IDC_RADIO5, &CNaviMeshTab::OnBnClickedRadioTriangle)
	ON_BN_CLICKED(IDC_BUTTON5, &CNaviMeshTab::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON6, &CNaviMeshTab::OnBnClickedButtonLoad)
	//ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &CNaviMeshTab::OnTvnKeydownTree1)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CNaviMeshTab::OnNMClickTree1)
	//ON_NOTIFY(TVN_BEGINRDRAG, IDC_TREE1, &CNaviMeshTab::OnTvnBeginrdragTree1)
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

void CNaviMeshTab::UpdateVertexPos(const _vec3& _vPos)
{
	UpdateData(TRUE);
	m_vVertexPos = _vPos;
	UpdateData(FALSE);
}

BOOL CNaviMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_rbtnNavi.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//void CNaviMeshTab::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	
//
//	*pResult = 0;
//}


void CNaviMeshTab::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 편집하고 있는 네비메쉬 얻기
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

	// 현재 선택한 삼각형이 있는지를 확인한다.
	if (pNaviMesh->GetMarkedTriangleIndex() == -1)
		return;
	// 네비메쉬에서 해당 삼각형을 제거한다.
	pNaviMesh->PopTriangleVertices(pNaviMesh->GetMarkedTriangleIndex());
	// 네비트리 갱신
	UpdateNaviTree(pNaviMesh);
	// 삭제 버튼 비활성화
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
	//auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);
	//_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
	//_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));

	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();
	
	
	UpdateData(TRUE);
	// 정점을 이동시키기 이전에 유효한 삼각형 정점이 세팅되어 있어야 합니다.
	pNaviMeshVtxMover->SetVertexPos(m_vVertexPos);
	UpdateData(FALSE);

	if (m_chkGroup.GetCheck())
		pNaviMeshVtxMover->MoveGroup();
}


void CNaviMeshTab::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);
	// 정점을 이동시키기 이전에 유효한 삼각형 정점이 세팅되어 있어야 합니다.
	pNaviMeshVtxMover->SetVertexPos(m_vVertexPos);
	UpdateData(FALSE);

	if (m_chkGroup.GetCheck())
		pNaviMeshVtxMover->MoveGroup();
}


void CNaviMeshTab::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);
	// 정점을 이동시키기 이전에 유효한 삼각형 정점이 세팅되어 있어야 합니다.
	pNaviMeshVtxMover->SetVertexPos(m_vVertexPos);
	UpdateData(FALSE);

	if (m_chkGroup.GetCheck()) {
		pNaviMeshVtxMover->MoveGroup();
	}
		
}


void CNaviMeshTab::OnBnClickedButtonCombine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 컴바인 버튼이 눌리면, 컴바인 캔슬 버튼이 보이게 된다.
	m_btnCombine.EnableWindow(FALSE);
	m_btnCombine.ShowWindow(FALSE);
	m_btnCancel.EnableWindow(TRUE);
	m_btnCancel.ShowWindow(TRUE);

	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();
	pNaviMeshVtxMover->ActivateMoverGizmo(false);	// 이동자 기즈모를 활성화하지 않습니다. (선택과 함수를 통한 이동은 가능합니다.)
}


void CNaviMeshTab::OnBnClickedButtonCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 캔슬 버튼이 눌리면 비활성화된 상태의 컴바인 버튼이 보인다.
	m_btnCombine.EnableWindow(TRUE);
	m_btnCombine.ShowWindow(TRUE);
	m_btnCancel.EnableWindow(FALSE);
	m_btnCancel.ShowWindow(FALSE);

	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();
	pNaviMeshVtxMover->ActivateMoverGizmo(true);	// 이동자 기즈모를 활성화합니다. (기즈모를 통한 드래그 이동이 가능해집니다.)
}


void CNaviMeshTab::OnBnClickedCheckGroup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	CNaviMeshVtxMover* pNaviMeshVtxCtrl = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);

	if (m_chkGroup.GetCheck()) {
		// 활성화된 상태라면,
		m_editGroupRange.EnableWindow(TRUE);
		m_fGroupRange = pNaviMeshVtxCtrl->GetGroupRange();
		pNaviMeshVtxCtrl->FormGroup();
	}
	else {
		m_editGroupRange.EnableWindow(FALSE);
		pNaviMeshVtxCtrl->ReleaseGroup();
	}
	
	UpdateData(FALSE);
}


void CNaviMeshTab::OnEnChangeEditGroupRange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);

	// 새로 지정된 그룹 범위를 이동자에 세팅하고, 새로이 그룹을 지정합니다.
	pNaviMeshVtxMover->SetGroupRange(m_fGroupRange);
	pNaviMeshVtxMover->FormGroup();

	UpdateData(FALSE);
}


void CNaviMeshTab::OnBnClickedRadioNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInputProcessor_Navi* pInputProcessor = dynamic_cast<CInputProcessor_Navi*>(g_pTool3D_Kernel->GetEditScene()->GetInputProcessor());
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);

	if (pInputProcessor) {
		pInputProcessor->ClearPickedVertices();
	}

	UpdateData(FALSE);

	pNaviMeshVtxMover->ActivateMoverGizmo(false);
	pNaviMesh->ReleaseMarkedTriangle();
	m_btnDelete.EnableWindow(FALSE);
	m_chkGroup.SetCheck(FALSE);
	m_chkGroup.EnableWindow(FALSE);
}


void CNaviMeshTab::OnBnClickedCheckNaviMagnet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	UpdateData(FALSE);
}


void CNaviMeshTab::OnBnClickedRadioVertex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInputProcessor_Navi* pInputProcessor = dynamic_cast<CInputProcessor_Navi*>(g_pTool3D_Kernel->GetEditScene()->GetInputProcessor());
	CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);

	if (pInputProcessor) {
		pInputProcessor->ClearPickedVertices();
	}

	UpdateData(FALSE);

	if(pNaviMeshVtxMover->GetVertexIndex() != -1)
		pNaviMeshVtxMover->ActivateMoverGizmo(true);
	pNaviMesh->ReleaseMarkedTriangle();
	m_btnDelete.EnableWindow(FALSE);
	m_chkGroup.EnableWindow(TRUE);
}


void CNaviMeshTab::OnBnClickedRadioTriangle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInputProcessor_Navi* pInputProcessor = dynamic_cast<CInputProcessor_Navi*>(g_pTool3D_Kernel->GetEditScene()->GetInputProcessor());
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	UpdateData(TRUE);

	if (pInputProcessor) {
		pInputProcessor->ClearPickedVertices();
	}

	UpdateData(FALSE);

	pNaviMeshVtxMover->ActivateMoverGizmo(false);
	m_chkGroup.SetCheck(FALSE);
	m_chkGroup.EnableWindow(FALSE);
}


void CNaviMeshTab::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO : 네비메쉬를 저장합니다.
	g_pTool3D_Kernel->GetEditScene()->SaveNaviMesh();
}


void CNaviMeshTab::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO : 네비메쉬를 읽어옵니다.
	g_pTool3D_Kernel->GetEditScene()->LoadNaviMesh();
}


//void CNaviMeshTab::OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	
//
//	*pResult = 0;
//}


void CNaviMeshTab::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	POINT point = Engine::GetClientCursorPoint(m_treeNavi.m_hWnd);
	UINT nFlags = 0;
	HTREEITEM hItem = m_treeNavi.HitTest(point, &nFlags);
	
	if (!hItem)
		return;

	//m_hSelectedTreeItem = pNMTreeView->itemNew.hItem;	// 선택된 아이템
	m_hSelectedTreeItem = hItem;
	auto hParent = m_treeNavi.GetParentItem(m_hSelectedTreeItem);
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();
	CInputProcessor_Navi* pInputProcessor = dynamic_cast<CInputProcessor_Navi*>(g_pTool3D_Kernel->GetEditScene()->GetInputProcessor());

	if (hParent) {
		// 삼각형의 정점 중 하나를 선택한 경우
		
		// Position Edit Ctrl 활성화
		m_editPosX.EnableWindow(TRUE);
		m_editPosY.EnableWindow(TRUE);
		m_editPosZ.EnableWindow(TRUE);

		// 그룹 체크 박스 활성화
		m_chkGroup.EnableWindow(TRUE);
		//if (m_chkGroup.GetCheck())
		//	pNaviMeshVtxMover->FormGroup();

		// 삼각형 삭제 버튼 비활성화
		m_btnDelete.EnableWindow(FALSE);
		
		// 픽모드를 변경한다.
		pInputProcessor->ClearPickedVertices();
		m_rbtnNavi.SetCheck(FALSE);
		m_rbtnVertex.SetCheck(TRUE);
		m_rbtnTriangle.SetCheck(FALSE);

		_int iTriangleIndex = _ttoi(m_treeNavi.GetItemText(hParent));
		_int iVertexIndex = _ttoi(m_treeNavi.GetItemText(m_hSelectedTreeItem));
		CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
		pNaviMesh->ReleaseMarkedTriangle();

		if (m_btnCombine.IsWindowVisible()) {
			// 초기 상태에서 정점을 선택하여 컴바인 버튼이 활성화된 상태로 전환
			pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
			pNaviMeshVtxMover->ActivateMoverGizmo(true);
			m_btnCombine.EnableWindow(TRUE);
		}
		else if (m_btnCancel.IsWindowVisible()) {
			if (m_pairLastPickedVertex.first != iTriangleIndex) {
				// 컴바인을 수행한다.
				// 이동할 지점의 위치를 얻어온다.
				_vec3 vDestPos = pNaviMesh->GetTriangleVertexPosition(iTriangleIndex, iVertexIndex);
				pNaviMeshVtxMover->SetVertexPos(vDestPos);
				if (m_chkGroup.GetCheck()) 
					pNaviMeshVtxMover->MoveGroup();
				else 
					pNaviMeshVtxMover->ReleaseGroup();
				// 이동을 완료하였다면, 클릭한 정점을 선택한다.
				pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
			}
			// 이동 기즈모를 활성화한다.
			pNaviMeshVtxMover->ActivateMoverGizmo(true);

			// 초기 상태로 돌아간다.
			m_btnCombine.EnableWindow(TRUE);
			m_btnCombine.ShowWindow(TRUE);
			m_btnCancel.EnableWindow(FALSE);
			m_btnCancel.ShowWindow(FALSE);
		}

		if (m_chkGroup.GetCheck())
			pNaviMeshVtxMover->FormGroup();

		// UI 동기화
		UpdateData(TRUE);
		m_vVertexPos = pNaviMeshVtxMover->GetVertexPos();
		UpdateData(FALSE);
	}
	else {
		// 삼각형 자체를 선택한 경우
		CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

		// 이동 기즈모를 비활성화시킨다.
		pNaviMeshVtxMover->ActivateMoverGizmo(false);

		CString strIndex = m_treeNavi.GetItemText(m_hSelectedTreeItem);
		_int iIndex = _ttoi(strIndex);
		pNaviMesh->MarkTriangle(iIndex);

		
		// Position Edit Ctrl 비활성화
		m_editPosX.EnableWindow(FALSE);
		m_editPosY.EnableWindow(FALSE);
		m_editPosZ.EnableWindow(FALSE);
		m_chkGroup.EnableWindow(FALSE);
		m_editGroupRange.EnableWindow(FALSE);

		// 삼각형 삭제 버튼을 활성화한다.
		m_btnDelete.EnableWindow(TRUE);

		// 삼각형 픽모드로 세팅한다.
		m_rbtnNavi.SetCheck(FALSE);
		m_rbtnVertex.SetCheck(FALSE);
		m_rbtnTriangle.SetCheck(TRUE);

		// 초기 상태로 돌아간다.
		m_btnCombine.EnableWindow(FALSE);
		m_btnCombine.ShowWindow(TRUE);
		m_btnCancel.EnableWindow(FALSE);
		m_btnCancel.ShowWindow(FALSE);
	}



	*pResult = 0;
}


//void CNaviMeshTab::OnTvnBeginrdragTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}
