#include "ChronoTimer.h"

USING(Engine)

Engine::CChronoTimer::CChronoTimer(void)
{

}

Engine::CChronoTimer::~CChronoTimer(void)
{

}

HRESULT CChronoTimer::Ready_ChronoTimer(void)
{
	Reset();

	return S_OK;
}

void CChronoTimer::Reset()
{
	m_bIsPaused = false;
	m_fElapsedTimePerFrame = 0.f;
	m_fPauseTotalTime = 0.f;

	m_timeBase = steady_clock::now();
	m_timePausePoint = m_timeBase;
	m_timeCurrent = m_timeBase;
	m_timeBefore = m_timeBase;
}

void CChronoTimer::RunTick()
{
	if (m_bIsPaused) {
		m_fElapsedTimePerFrame = 0.f;
		return;
	}

	m_timeCurrent = steady_clock::now();
	m_fElapsedTimePerFrame = duration<_float>(m_timeCurrent - m_timeBefore).count();
	m_timeBefore = m_timeCurrent;

	// 프로세서가 절전 모드로 들어가거나 다른 프로세서와 엉키는 경우
	// elapsed_time_per_frame_은 음수가 될 수 있다.
	if (m_fElapsedTimePerFrame < 0.f) {
		m_fElapsedTimePerFrame = 0.f;
	}
}

void CChronoTimer::RunToPause()
{
	if (!m_bIsPaused) {
		m_timePausePoint = steady_clock::now();
		m_bIsPaused = true;
	}
}

void CChronoTimer::PauseToRun()
{
	if (m_bIsPaused) {
		TIME_POINT timeRunPoint = steady_clock::now();
		m_fPauseTotalTime += duration<_float>(timeRunPoint - m_timePausePoint).count();
		m_timeBefore = timeRunPoint;
		m_bIsPaused = false;
	}
}

float CChronoTimer::GetElapsedTimePerFrame()
{
	return m_fElapsedTimePerFrame * m_fDeltaMultiple;
}

float CChronoTimer::GetCurTime()
{
	return duration<_float>(m_timeCurrent - m_timeBase).count();
}

float CChronoTimer::GetTotalTimeFromRun()
{
	_float fTotalTime = 0.f;

	if (m_bIsPaused) {
		fTotalTime = duration<_float>(m_timePausePoint - m_timeBase).count() - m_fPauseTotalTime;
	}
	else {
		m_timeCurrent = steady_clock::now();
		fTotalTime = duration<_float>(m_timeCurrent - m_timeBase).count() - m_fPauseTotalTime;
	}
	return fTotalTime;
}


Engine::CChronoTimer* Engine::CChronoTimer::Create(void)
{
	CChronoTimer*   pInstance = new CChronoTimer;

	if (FAILED(pInstance->Ready_ChronoTimer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CChronoTimer::Free(void)
{

}


