#include "InputModeMgr.h"
#include "InputMode.h"

USING(Engine)

Engine::CInputModeMgr::CInputModeMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// 반.드.시. 초기에는 true로 설정해주어야 함.
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
	if (!m_bIsConfirmed) {				// 상태 변경 요청이 접수된 경우
		Safe_Release(m_pCurInputMode);		// 기존 상태를 지운다. 
		m_pCurInputMode = m_pNextInputMode;		// 기존 상태를 새로운 상태로 교체한다.
		m_pNextInputMode = nullptr;			// 새로운 상태 변수 무효화
		m_bIsConfirmed = true;			// 상태 변경 완료!
	}
	if (!m_pCurInputMode) return false;		// 그럼에도 현재 유효한 씬이 없다면 false를 반환

	return true;						// 계속 프로세스를 진행
}

void Engine::CInputModeMgr::SetNextInputMode(CInputMode * _pNextInputMode)
{
	// 다음 씬이 이미 세팅된 경우, 해당 함수를 무시한다.
	// => 다음 상태 변경까지 새로운 상태로 세팅할 수 없다.
	// for 상태를 동기화해주기 위함.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextInputMode);	// 해당 요청은 삭제한다.
		return;	// 이전에 상태 변경이 요청이 들어왔지만 아직 변경되지 않은 경우
	}

	m_pNextInputMode = _pNextInputMode;		// 새로이 상태 변경 요청이 들어온 경우.
	m_bIsConfirmed = false;			// 변경 요청이 들어옴.
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