#include "CSceneMgr.h"
#include "Export_Utility.h"

USING(Engine)

Engine::CSceneMgr::CSceneMgr()
	:
	//m_rGameWorld(_rGameWorld),
	m_bIsConfirmed(true)		// 반.드.시. 초기에는 true로 설정해주어야 함.
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
	if (!m_bIsConfirmed) {				// 상태 변경 요청이 접수된 경우
		if (m_pCurScene) m_pCurScene->OnExited();		// 없애기 전 상태 정리		(추가)
		Safe_Release(m_pCurScene);		// 기존 상태를 지운다. 
		m_pCurScene = m_pNextScene;		// 기존 상태를 새로운 상태로 교체한다.
		if (m_pCurScene) m_pCurScene->OnLoaded();		// 상태 준비 (이제 Ready대신 이것을 쓸 것이다.) (추가)
		m_pNextScene = nullptr;			// 새로운 상태 변수 무효화
		m_bIsConfirmed = true;			// 상태 변경 완료!
	}
	if (!m_pCurScene) return false;		// 그럼에도 현재 유효한 씬이 없다면 false를 반환
	if (!m_bInit) {
		m_pCurScene->ResetScene();
		m_bInit = true;
	}
	
	return true;						// 계속 프로세스를 진행
}

void Engine::CSceneMgr::SetNextScene(CScene * _pNextScene)
{
	// 다음 씬이 이미 세팅된 경우, 해당 함수를 무시한다.
	// => 다음 상태 변경까지 새로운 상태로 세팅할 수 없다.
	// for 상태를 동기화해주기 위함.
	if (!m_bIsConfirmed) {
		Safe_Release(_pNextScene);	// 해당 요청은 삭제한다.
		return;	// 이전에 상태 변경이 요청이 들어왔지만 아직 변경되지 않은 경우
	}

	m_pNextScene = _pNextScene;		// 새로이 상태 변경 요청이 들어온 경우.
	m_bIsConfirmed = false;			// 변경 요청이 들어옴.
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
