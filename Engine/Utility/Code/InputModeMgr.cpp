#include "InputModeMgr.h"
#include "InputMode.h"

USING(Engine)

Engine::CInputModeMgr::CInputModeMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

Engine::CInputModeMgr::~CInputModeMgr()
{
	//Release();
	//Free();
}

//LRESULT CInputModeMgr::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	if (!m_pCurInputMode) return 0;
//	return m_pCurInputMode->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

bool Engine::CInputModeMgr::ConfirmValidInputMode(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		Safe_Release(m_pCurInputMode);		// ���� ���¸� �����. 
		m_pCurInputMode = m_pNextInputMode;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		m_pNextInputMode = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurInputMode) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ

	return true;						// ��� ���μ����� ����
}

void Engine::CInputModeMgr::SetNextInputMode(CInputMode * _pNextInputMode)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextInputMode);	// �ش� ��û�� �����Ѵ�.
		return;	// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextInputMode = _pNextInputMode;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

_int Engine::CInputModeMgr::ProcessInput(const _float& _fDeltaTime)
{
	if (!ConfirmValidInputMode())
		return -1;

	return m_pCurInputMode->ProcessInput(_fDeltaTime);
}


CInputModeMgr * CInputModeMgr::Create(void)
{
	return new CInputModeMgr();
}

void Engine::CInputModeMgr::Free(void)
{
	Safe_Release(m_pCurInputMode);
	Safe_Release(m_pNextInputMode);
	m_bIsConfirmed = true;
}