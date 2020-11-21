
// Tool_3DView.cpp: CTool3DView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool_3D.h"
#endif

#include "Tool_3DDoc.h"
#include "Tool_3DView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTool3DView

HWND g_hWnd;
IMPLEMENT_DYNCREATE(CTool3DView, CView)

BEGIN_MESSAGE_MAP(CTool3DView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_GETMINMAXINFO()
//ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CTool3DView 생성/소멸

CTool3DView::CTool3DView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTool3DView::~CTool3DView()
{
}

BOOL CTool3DView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTool3DView 그리기

void CTool3DView::OnDraw(CDC* /*pDC*/)
{
	CTool3DDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTool3DView 인쇄

BOOL CTool3DView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTool3DView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTool3DView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTool3DView 진단

#ifdef _DEBUG
void CTool3DView::AssertValid() const
{
	CView::AssertValid();
}

void CTool3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTool3DDoc* CTool3DView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool3DDoc)));
	return (CTool3DDoc*)m_pDocument;
}
#endif //_DEBUG


// CTool3DView 메시지 처리기


void CTool3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// Gap을 구하기 위해서 MainFrame 크기를 구한다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);														// MainFrame 렉트를 구하고,
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);		// 렉트 시작 위치를 (0, 0)으로 변환

	// Gap을 구하기 위해서 View 크기를 구한다.
	RECT rcView = {};
	GetClientRect(&rcView); ;
	float fGapX = rcMain.right - float(rcView.right);
	float fGapY = WINCY + (float(rcMain.bottom) - rcView.bottom);

	pMain->SetWindowPos(nullptr, 0, 0, LONG(WINCX + fGapX), LONG(fGapY), SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_hWnd = m_hWnd;

	// D2D같은 경우 여기에서 장치와 에디터툴 객체를 생성하였다.
}


void CTool3DView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CView::PostNcDestroy();
}


//void CTool3DView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	lpMMI->ptMinTrackSize.x = WINCX - 300;
//	lpMMI->ptMinTrackSize.y = WINCY - 300;
//	lpMMI->ptMaxTrackSize.x = WINCX - 300;
//	lpMMI->ptMaxTrackSize.y = WINCY - 300;
//
//	CView::OnGetMinMaxInfo(lpMMI);
//}


//void CTool3DView::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	g_pTool3D_Kernel->OnLButtonDown(nFlags, point);
//
//	CView::OnLButtonDown(nFlags, point);
//}
