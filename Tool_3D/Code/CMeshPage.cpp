// CMeshPage.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMeshPage.h"
#include "afxdialogex.h"


// CMeshPage 대화 상자

IMPLEMENT_DYNAMIC(CMeshPage, CPropertyPage)

CMeshPage::CMeshPage()
	: CPropertyPage(IDD_MESH_PAGE)
{

}

CMeshPage::~CMeshPage()
{
}

void CMeshPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeNavi);
}


BEGIN_MESSAGE_MAP(CMeshPage, CPropertyPage)
END_MESSAGE_MAP()


// CMeshPage 메시지 처리기


BOOL CMeshPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CString cst_level0;
	CString cst_entryord, cst_exitorder;

	HTREEITEM h_root;
	HTREEITEM h_entry, h_exit;
	for (int i = 0; i < 5; ++i) {
		cst_level0.Format(L"NXtask=%d", i);
		h_root = m_treeNavi.InsertItem(cst_level0, NULL, NULL);

		h_entry = m_treeNavi.InsertItem(L"Entry", h_root, NULL);
		for (int j = 0; j < 2; ++j) {
			cst_entryord.Format(L"Order%d", j);
			m_treeNavi.InsertItem(cst_entryord, h_entry, NULL);
		}

		h_exit = m_treeNavi.InsertItem(L"Exit", h_root, NULL);
		for (int j = 0; j < 4; ++j) {
			cst_entryord.Format(L"Order%d", j);
			m_treeNavi.InsertItem(cst_entryord, h_exit, NULL);
		}
		
		//m_treeNavi.DeleteItem(h_entry);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
