// CAABBColTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CAABBColTab.h"
#include "afxdialogex.h"


// CAABBColTab 대화 상자

IMPLEMENT_DYNAMIC(CAABBColTab, CDialogEx)

CAABBColTab::CAABBColTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AABB_COLTAB, pParent)
{

}

CAABBColTab::~CAABBColTab()
{
}

void CAABBColTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAABBColTab, CDialogEx)
END_MESSAGE_MAP()


// CAABBColTab 메시지 처리기
