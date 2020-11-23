// CMeshTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMeshTab.h"
#include "NaviMesh.h"
#include "afxdialogex.h"


// CMeshTab 대화 상자

IMPLEMENT_DYNAMIC(CMeshTab, CDialogEx)

CMeshTab::CMeshTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESH_TAB, pParent)
	, m_iPickedRadioBtnIndex(0)
{

}

CMeshTab::~CMeshTab()
{
}

void CMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_rbtnNavi);
	DDX_Control(pDX, IDC_RADIO2, m_rbtnObject);
	DDX_Radio(pDX, IDC_RADIO1, m_iPickedRadioBtnIndex);
	DDX_Control(pDX, IDC_TREE1, m_treeNavi);
}


BEGIN_MESSAGE_MAP(CMeshTab, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CMeshTab::OnBnClickedRadioNavi)
	ON_BN_CLICKED(IDC_RADIO2, &CMeshTab::OnBnClickedRadioObject)
END_MESSAGE_MAP()


// CMeshTab 메시지 처리기


void CMeshTab::UpdateNaviTree(Client::CNaviMesh * _pNaviMesh)
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

BOOL CMeshTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	UpdateData(TRUE);
	// 초기 라디오 버튼은 NaviMesh가 선택되도록 한다.
	m_iPickedRadioBtnIndex = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMeshTab::OnBnClickedRadioNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMeshTab::OnBnClickedRadioObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
