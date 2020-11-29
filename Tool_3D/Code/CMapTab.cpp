// CMapTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CMapTab.h"
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
}


BEGIN_MESSAGE_MAP(CMapTab, CDialogEx)
END_MESSAGE_MAP()


// CMapTab 메시지 처리기
