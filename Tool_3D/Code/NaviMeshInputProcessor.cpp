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
	// �׺�޽��� �����͸� ���´�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pNaviMeshTab = pTabForm->GetNaviMeshTab();

	return S_OK;
}

int CNaviMeshInputProcessor::Update_Object(const _float & _fDeltaTime)
{
	// ���⿡�� �Է��� ó������ �ʴ´�.
	// �Է� ��忡�� Ư�� Ű�� ���� if���� �����ϰ�, �ش� if������
	// Processor�� �Լ��� ȣ���ϴ� ���̴�.
	return 0;
}

void CNaviMeshInputProcessor::Click()
{
	switch (GetPickMode()) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		PickTerrain();
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		// TODO : ������ ���� ó��(�̵�, ����, �׷�)�� �����մϴ�.
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		// TODO : �ﰢ�� ��ŷ/������ ���� ó���� �����մϴ�.
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

	// ���� �ȸ�忡 ���� ��ó���� �����մϴ�.
	switch (GetPickMode()) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		break;
	}

	// ������ �ȸ����� ó���� �����մϴ�.
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
