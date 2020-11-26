// CTabForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CTabForm.h"
#include "CTerrainTab.h"
#include "CNaviMeshTab.h"
#include "NaviMesh.h"
#include "NaviMeshVtxCtrl.h"
#include "EditScene.h"
#include "InputMode_Terrain.h"
#include "InputMode_Navi.h"


// CTabForm

IMPLEMENT_DYNCREATE(CTabForm, CFormView)

CTabForm::CTabForm()
	: CFormView(IDD_TAB_FORM)
{

}

CTabForm::~CTabForm()
{
}

void CTabForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CTabForm, CFormView)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabForm::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CTabForm 진단

#ifdef _DEBUG
void CTabForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTabForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTabForm 메시지 처리기

void CTabForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab.InsertItem(0, _T("Terrain"));
	m_Tab.InsertItem(1, _T("Navi Mesh"));

	// 초기 터레인 탭으로 설정
	m_Tab.SetCurSel(0);		

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_pTerrainTab = new CTerrainTab;
	m_pTerrainTab->Create(IDD_TERRAIN_TAB, &m_Tab);
	m_pTerrainTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pTerrainTab->ShowWindow(SW_SHOW);

	m_pNaviMeshTab = new CNaviMeshTab;
	m_pNaviMeshTab->Create(IDD_NAVI_MESH_TAB, &m_Tab);
	m_pNaviMeshTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_pNaviMeshTab->ShowWindow(SW_HIDE);
}


void CTabForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Safe_Delete(m_pTerrainTab);
	Safe_Delete(m_pNaviMeshTab);
}


void CTabForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelIndex = m_Tab.GetCurSel();
	CEditScene* pEditScene = g_pTool3D_Kernel->GetEditScene();
	pEditScene->GetNaviMeshVtxCtrl()->SetActive(false);
	auto pInputModeMgr = pEditScene->GetInputModeMgr();
	
	switch (iSelIndex)
	{
	case 0: {
		if(pInputModeMgr)
			pInputModeMgr->SetNextInputMode(new CInputMode_Terrain(pInputModeMgr));
		m_pTerrainTab->ShowWindow(SW_SHOW);
		m_pNaviMeshTab->ShowWindow(SW_HIDE);
	}
		
		break;
	case 1: {
		if (pInputModeMgr)
			pInputModeMgr->SetNextInputMode(new CInputMode_Navi(pInputModeMgr));
		m_pTerrainTab->ShowWindow(SW_HIDE);
		m_pNaviMeshTab->ShowWindow(SW_SHOW);
		//pEditScene->GetNaviMeshVtxCtrl()->SetActive(true);
	}
		break;
	default:
		break;
	}

	*pResult = 0;
}
