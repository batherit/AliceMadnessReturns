#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "framework.h"
#include "State.h"

BEGIN(Client)

template<typename T>
class CStateMgr final : public CBase
{
public:
	CStateMgr(T& _rOwner);
	~CStateMgr();

public:
	bool ConfirmValidState(void);				// 다음 상태가 존재한다면, 그 상태으로 변경한다.
	void SetNextState(CState<T>* _pNextScene, bool _bIsForced = false);
	void Update(float _fDeltaTime);
	//void LateUpdate(void);
	//void Release(void);

	// CBase을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	T& m_rOwner;
	CState<T>* m_pCurState = nullptr;
	CState<T>* m_pNextState = nullptr;
	bool m_bIsConfirmed = true;

	
};

template<typename T>
CStateMgr<T>::CStateMgr(T& _rOwner)
	:
	m_rOwner(_rOwner),
	m_bIsConfirmed(true)		// 반.드.시. 초기에는 true로 설정해주어야 함.
{
}

template<typename T>
inline CStateMgr<T>::~CStateMgr()
{
}

template<typename T>
bool CStateMgr<T>::ConfirmValidState(void)
{
	if (!m_bIsConfirmed) {				// 상태 변경 요청이 접수된 경우
		if (m_pCurState) m_pCurState->OnExited();		// 없애기 전 상태를 정리한다.
		Client::Safe_Release(m_pCurState);		// 기존 상태를 지운다. 
		m_pCurState = m_pNextState;		// 기존 상태를 새로운 상태로 교체한다.
		if (m_pCurState) m_pCurState->OnLoaded();		// 상태 준비
		m_pNextState = nullptr;			// 새로운 상태 변수 무효화
		m_bIsConfirmed = true;			// 상태 변경 완료!
	}
	if (!m_pCurState) return false;		// 그럼에도 현재 유효한 씬이 없다면 false를 반환
	return true;						// 계속 프로세스를 진행
}

template<typename T>
void CStateMgr<T>::SetNextState(CState<T> * _pNextScene, bool _bIsForced)
{
	// 다음 씬이 이미 세팅된 경우, 해당 함수를 무시한다.
	// => 다음 상태 변경까지 새로운 상태로 세팅할 수 없다.
	// for 상태를 동기화해주기 위함.
	if (!m_bIsConfirmed && !_bIsForced) {
		Client::Safe_Release(_pNextScene);	// 들어온 상태는 제거한다.
		return;						// 이전에 상태 변경이 요청이 들어왔지만 아직 변경되지 않은 경우
	}

	if (m_pNextState)
		Client::Safe_Release(m_pNextState);

	m_pNextState = _pNextScene;		// 새로이 상태 변경 요청이 들어온 경우.
	m_bIsConfirmed = false;			// 변경 요청이 들어옴.
}

template<typename T>
void CStateMgr<T>::Update(float _fDeltaTime)
{
	if (m_pCurState) m_pCurState->Update(_fDeltaTime);
}

//template<typename T>
//void CStateMgr<T>::LateUpdate(void)
//{
//	if (m_pCurState) m_pCurState->LateUpdate();
//}

//template<typename T>
//void CStateMgr<T>::Release(void)
//{
//	SafelyDeleteObj(m_pCurState);
//	SafelyDeleteObj(m_pNextState);
//	m_bIsConfirmed = true;
//}

template<typename T>
inline void CStateMgr<T>::Free(void)
{
	Client::Safe_Release(m_pCurState);
	Client::Safe_Release(m_pNextState);
	m_bIsConfirmed = true;
}

END