#include "CSceneMgr.h"
#include "Export_Utility.h"

USING(Engine)

Engine::CSceneMgr::CSceneMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

Engine::CSceneMgr::~CSceneMgr()
{
	//Release();
	//Free();
}

//LRESULT CSceneMgr::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	if (!m_pCurScene) return 0;
//	return m_pCurScene->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

bool Engine::CSceneMgr::ConfirmValidScene(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		if (m_pCurScene) m_pCurScene->OnExited();		// ���ֱ� �� ���� ����		(�߰�)
		Safe_Release(m_pCurScene);		// ���� ���¸� �����. 
		m_pCurScene = m_pNextScene;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		if (m_pCurScene) m_pCurScene->OnLoaded();		// ���� �غ� (���� Ready��� �̰��� �� ���̴�.) (�߰�)
		m_pNextScene = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurScene) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ
	if (!m_bInit) {
		m_pCurScene->ResetScene();
		m_bInit = true;
	}
	
	return true;						// ��� ���μ����� ����
}

void Engine::CSceneMgr::SetNextScene(CScene * _pNextScene)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextScene);	// �ش� ��û�� �����Ѵ�.
		return;	// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextScene = _pNextScene;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

void Engine::CSceneMgr::Update(const _float& _fDeltaTime)
{
	m_pCurScene->Update(_fDeltaTime);
}

//void Engine::CSceneMgr::LateUpdate(void)
//{
//	m_pCurScene->LateUpdate();
//}

//void Engine::CSceneMgr::Render(CCamera * _pCamera)
//{
//	m_pCurScene->Render(_pCamera);
//}
//
//void Engine::CSceneMgr::Render(HDC & _hdc, CCamera * _pCamera)
//{
//	
//}

void Engine::CSceneMgr::Render(void)
{
	m_pCurScene->Render();
	m_pCurScene->CollectInvalidObjects();
}

//void Engine::CSceneMgr::Release(void)
//{
//	Safe_Delete(m_pCurScene);
//	Safe_Delete(m_pNextScene);
//	m_bIsConfirmed = true;
//}

CSceneMgr * CSceneMgr::Create(void)
{
	return new CSceneMgr();
}

void Engine::CSceneMgr::Free(void)
{
	Safe_Release(m_pCurScene);
	Safe_Release(m_pNextScene);
	m_bIsConfirmed = true;
}

//LRESULT Engine::CSceneMgr::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	if (!m_pCurScene) return 0;
//	return m_pCurScene->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}
