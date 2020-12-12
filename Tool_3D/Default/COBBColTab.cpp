// COBBColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "COBBColTab.h"
#include "afxdialogex.h"


// COBBColTab 대화 상자

IMPLEMENT_DYNAMIC(COBBColTab, CDialogEx)

COBBColTab::COBBColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBB_COLTAB, pParent)
{

}

COBBColTab::~COBBColTab()
{
}

void COBBColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBBColTab, CDialogEx)
END_MESSAGE_MAP()


// COBBColTab 메시지 처리기
