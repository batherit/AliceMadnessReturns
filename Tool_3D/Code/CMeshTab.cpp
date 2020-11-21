// CMeshTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMeshTab.h"
#include "afxdialogex.h"


// CMeshTab 대화 상자

IMPLEMENT_DYNAMIC(CMeshTab, CDialogEx)

CMeshTab::CMeshTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESH_TAB, pParent)
{

}

CMeshTab::~CMeshTab()
{
}

void CMeshTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMeshTab, CDialogEx)
END_MESSAGE_MAP()


// CMeshTab 메시지 처리기
