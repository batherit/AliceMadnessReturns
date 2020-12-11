// CSphereColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CSphereColTab.h"
#include "afxdialogex.h"


// CSphereColTab 대화 상자

IMPLEMENT_DYNAMIC(CSphereColTab, CDialogEx)

CSphereColTab::CSphereColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPHERE_COLTAB, pParent)
{

}

CSphereColTab::~CSphereColTab()
{
}

void CSphereColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSphereColTab, CDialogEx)
END_MESSAGE_MAP()


// CSphereColTab 메시지 처리기
