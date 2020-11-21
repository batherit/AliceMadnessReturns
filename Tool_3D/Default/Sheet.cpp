// Sheet.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "Sheet.h"


// Sheet

IMPLEMENT_DYNAMIC(Sheet, CPropertySheet)

Sheet::Sheet()
{
	AddPage(&m_tTerrainPage);
	AddPage(&m_tMeshPage);
}

Sheet::Sheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

Sheet::Sheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

Sheet::~Sheet()
{
}


BEGIN_MESSAGE_MAP(Sheet, CPropertySheet)
END_MESSAGE_MAP()


// Sheet 메시지 처리기
