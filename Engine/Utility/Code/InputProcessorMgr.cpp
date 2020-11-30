#include "InputProcessorMgr.h"
#include "InputProcessor.h"

USING(Engine)

Engine::CInputProcessorMgr::CInputProcessorMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// ��.��.��. �ʱ⿡�� true�� �������־�� ��.
{
}

Engine::CInputProcessorMgr::~CInputProcessorMgr()
{
	//Release();
	//Free();
}

//LRESULT CInputProcessorMgr::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	if (!m_pCurInputProcessor) return 0;
//	return m_pCurInputProcessor->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);
//}

bool Engine::CInputProcessorMgr::ConfirmValidInputProcessor(void)
{
	if (!m_bIsConfirmed) {				// ���� ���� ��û�� ������ ���
		Safe_Release(m_pCurInputProcessor);		// ���� ���¸� �����. 
		m_pCurInputProcessor = m_pNextInputProcessor;		// ���� ���¸� ���ο� ���·� ��ü�Ѵ�.
		m_pNextInputProcessor = nullptr;			// ���ο� ���� ���� ��ȿȭ
		m_bIsConfirmed = true;			// ���� ���� �Ϸ�!
	}
	if (!m_pCurInputProcessor) return false;		// �׷����� ���� ��ȿ�� ���� ���ٸ� false�� ��ȯ

	return true;						// ��� ���μ����� ����
}

void Engine::CInputProcessorMgr::SetNextInputProcessor(CInputProcessor * _pNextInputProcessor)
{
	// ���� ���� �̹� ���õ� ���, �ش� �Լ��� �����Ѵ�.
	// => ���� ���� ������� ���ο� ���·� ������ �� ����.
	// for ���¸� ����ȭ���ֱ� ����.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextInputProcessor);	// �ش� ��û�� �����Ѵ�.
		return;	// ������ ���� ������ ��û�� �������� ���� ������� ���� ���
	}

	m_pNextInputProcessor = _pNextInputProcessor;		// ������ ���� ���� ��û�� ���� ���.
	m_bIsConfirmed = false;			// ���� ��û�� ����.
}

_int Engine::CInputProcessorMgr::ProcessInput(const _float& _fDeltaTime)
{
	if (!ConfirmValidInputProcessor())
		return -1;

	return m_pCurInputProcessor->ProcessInput(_fDeltaTime);
}


CInputProcessorMgr * CInputProcessorMgr::Create(void)
{
	return new CInputProcessorMgr();
}

void Engine::CInputProcessorMgr::Free(void)
{
	Safe_Release(m_pCurInputProcessor);
	Safe_Release(m_pNextInputProcessor);
	m_bIsConfirmed = true;
}