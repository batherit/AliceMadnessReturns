// CEmptyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CEmptyForm.h"
#include "Sheet.h"

// CEmptyForm

IMPLEMENT_DYNCREATE(CEmptyForm, CFormView)

CEmptyForm::CEmptyForm()
	: CFormView(IDD_EMPTY_FORM)
{

}

CEmptyForm::~CEmptyForm()
{
}

void CEmptyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEmptyForm, CFormView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEmptyForm 진단

#ifdef _DEBUG
void CEmptyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEmptyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEmptyForm 메시지 처리기


void CEmptyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_pSheet = new Sheet;
	m_pSheet->Create(this, WS_VISIBLE | WS_CHILD);
	m_pSheet->MoveWindow(0, 0, 500, 800, FALSE);
}


void CEmptyForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Delete(m_pSheet);
}
