#include "pch.h"
#include "NaviMeshInputProcessor.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CNaviMeshTab.h"

CNaviMeshInputProcessor::CNaviMeshInputProcessor(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CNaviMeshInputProcessor::CNaviMeshInputProcessor(const CNaviMeshInputProcessor & rhs)
	:
	CGameObject(rhs)
{
}

CNaviMeshInputProcessor::~CNaviMeshInputProcessor(void)
{
}

HRESULT CNaviMeshInputProcessor::Ready_Object(void)
{
	// 네비메쉬탭 포인터를 얻어온다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pNaviMeshTab = pTabForm->GetNaviMeshTab();

	return S_OK;
}

int CNaviMeshInputProcessor::Update_Object(const _float & _fDeltaTime)
{
	// 여기에서 입력을 처리하진 않는다.
	// 입력 모드에서 특정 키에 대한 if문이 존재하고, 해당 if문에서
	// Processor의 함수를 호출하는 식이다.
	return 0;
}

void CNaviMeshInputProcessor::Click()
{
	switch (GetPickMode()) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		PickTerrain();
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		// TODO : 정점에 대한 처리(이동, 결합, 그룹)를 진행합니다.
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		// TODO : 삼각형 픽킹/삭제에 대한 처리를 진행합니다.
		break;
	}
}

void CNaviMeshInputProcessor::Drag(const _float _fDeltaTime)
{
}

void CNaviMeshInputProcessor::Idle(const _float _fDeltaTime)
{
}

void CNaviMeshInputProcessor::SetPickMode(NAVIMESH_TAB::E_PICKMODE _ePickMode)
{
	if (NAVIMESH_TAB::PICKMODE_END <= _ePickMode)
		return;

	// 현재 픽모드에 대한 전처리를 진행합니다.
	switch (GetPickMode()) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		break;
	}

	// 세팅할 픽모드로의 처리를 진행합니다.
	switch (_ePickMode) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		break;
	}
}

NAVIMESH_TAB::E_PICKMODE CNaviMeshInputProcessor::GetPickMode() const
{
	if (m_pNaviMeshTab->m_rbtnNavi.GetCheck())
		return NAVIMESH_TAB::PICKMODE_NAVI;
	else if (m_pNaviMeshTab->m_rbtnVertex.GetCheck())
		return NAVIMESH_TAB::PICKMODE_VERTEX;
	else if (m_pNaviMeshTab->m_rbtnTriangle.GetCheck())
		return NAVIMESH_TAB::PICKMODE_TRIANGLE;

	return NAVIMESH_TAB::PICKMODE_END;
}

void CNaviMeshInputProcessor::PickTerrain()
{
}

CNaviMeshInputProcessor * CNaviMeshInputProcessor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMeshInputProcessor*	pInstance = new CNaviMeshInputProcessor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMeshInputProcessor::Free(void)
{
	CGameObject::Free();
}
