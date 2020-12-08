// CColliderTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CColliderTab.h"
#include "afxdialogex.h"


// CColliderTab 대화 상자

IMPLEMENT_DYNAMIC(CColliderTab, CDialogEx)

CColliderTab::CColliderTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLLIDER_TAB, pParent)
{

}

CColliderTab::~CColliderTab()
{
}

void CColliderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColliderTab, CDialogEx)
END_MESSAGE_MAP()


// CColliderTab 메시지 처리기
