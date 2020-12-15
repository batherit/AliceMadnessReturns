#include "pch.h"
#include "InputProcessor_Collider.h"
#include "InputProcessor_Navi.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CColliderTab.h"
#include "EditScene.h"
#include "StaticObject.h"
#include "Gizmo.h"

CInputProcessor_Collider::CInputProcessor_Collider(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
	// 에디트씬 포인터를 얻어온다.
	m_pEditScene = m_pEditScene = g_pTool3D_Kernel->GetEditScene();

	// 맵 탭 포인터를 얻어온다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pColliderTab = pTabForm->GetColliderTab();
}

CInputProcessor_Collider::~CInputProcessor_Collider(void)
{
}

_int CInputProcessor_Collider::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	return 0;
}
