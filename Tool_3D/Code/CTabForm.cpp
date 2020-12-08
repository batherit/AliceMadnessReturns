// CTabForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool_3D.h"
#include "CTabForm.h"
#include "CTerrainTab.h"
#include "CNaviMeshTab.h"
#include "CMapTab.h"
#include "CColliderTab.h"
#include "NaviMesh.h"
#include "NaviMeshVtxCtrl.h"
#include "EditScene.h"
#include "InputProcessor_Terrain.h"
#include "InputProcessor_Navi.h"
#include "InputProcessor_Map.h"
#include "InputProcessor_Collider.h"


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

void CTabForm::ActivateTab(const E_TAB_TYPE & _eTabType)
{
	for (auto& pTab : m_vecTabs) {
		pTab->ShowWindow(SW_HIDE);
	}

	m_Tab.SetCurSel(_eTabType);
	m_vecTabs[_eTabType]->ShowWindow(SW_SHOW);
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
	m_Tab.InsertItem(2, _T("Map"));
	m_Tab.InsertItem(3, _T("Collider"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_pTerrainTab = new CTerrainTab;
	m_pTerrainTab->Create(IDD_TERRAIN_TAB, &m_Tab);
	m_pTerrainTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pTerrainTab);

	m_pNaviMeshTab = new CNaviMeshTab;
	m_pNaviMeshTab->Create(IDD_NAVI_MESH_TAB, &m_Tab);
	m_pNaviMeshTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pNaviMeshTab);

	m_pMapTab = new CMapTab;
	m_pMapTab->Create(IDD_MAP_TAB, &m_Tab);
	m_pMapTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pMapTab);

	m_pColliderTab = new CColliderTab;
	m_pColliderTab->Create(IDD_COLLIDER_TAB, &m_Tab);
	m_pColliderTab->MoveWindow(0, 25, rect.Width(), rect.Height());
	m_vecTabs.emplace_back(m_pColliderTab);
	
	// 초기 터레인 탭으로 설정
	ActivateTab(TYPE_TERRAIN);
}


void CTabForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	for (auto& pTab : m_vecTabs) {
		Safe_Delete(pTab);
	}
	m_vecTabs.clear();
	m_vecTabs.shrink_to_fit();
}


void CTabForm::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelIndex = m_Tab.GetCurSel();
	CEditScene* pEditScene = g_pTool3D_Kernel->GetEditScene();
	//pEditScene->GetNaviMeshVtxCtrl()->SetActive(false);
	auto pInputProcessorMgr = pEditScene->GetInputProcessorMgr();
	
	switch (iSelIndex)
	{
	case 0: {
		if(pInputProcessorMgr)
			pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Terrain(pInputProcessorMgr));
		ActivateTab(TYPE_TERRAIN);
		break;
	}
		
	case 1: {
		if (pInputProcessorMgr)
			pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Navi(pInputProcessorMgr));
		ActivateTab(TYPE_NAVI);
		break;
	}		
	case 2: {
		if (pInputProcessorMgr)
			pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Map(pInputProcessorMgr));
		ActivateTab(TYPE_MAP);
		break;
	}
	case 3: {
		if (pInputProcessorMgr)
			pInputProcessorMgr->SetNextInputProcessor(new CInputProcessor_Collider(pInputProcessorMgr));
		ActivateTab(TYPE_COLLIDER);
		break;
	}
	default:
		break;
	}

	*pResult = 0;
}
