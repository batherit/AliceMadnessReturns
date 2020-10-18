#ifndef ChronoTimer_h__
#define ChronoTimer_h__

#include "Engine_Define.h"
#include "Base.h"
using namespace chrono;

BEGIN(Engine)


class ENGINE_DLL CChronoTimer : public CBase
{
private:
	typedef steady_clock::time_point TIME_POINT;
private:
	explicit CChronoTimer(void);
	virtual ~CChronoTimer(void);
	HRESULT      Ready_ChronoTimer(void);

public:
	void Reset();						// 타이머 속성을 초기화
	void RunTick();						// 한 번의 틱 (곧 경과된 시간을 측정한다.)
	void RunToPause();					// 일시 정지
	void PauseToRun();					// 타이머 작동 (연속 호출 시 첫 호출 기준으로 시간 측정)

	_float GetElapsedTimePerFrame();		// 프레임 당 경과된 시간(sec)
	_float GetCurTime();					// 현재 시간(sec)
	_float GetTotalTimeFromRun();		// 최초 RunTick 실행 후 경과된 시간

private:
	_bool m_bIsPaused;					// 일시정지되었는지?
	TIME_POINT m_timeBase;					// 최초의 기본 시간 
	TIME_POINT m_timeBefore;				// 이전 시간 
	TIME_POINT m_timeCurrent;				// 현재 시간 
	TIME_POINT m_timePausePoint;  		// 일시정지된 시점 

	_float m_fPauseTotalTime;		// 누적된 일시정지 총 시간 
	_float m_fElapsedTimePerFrame; 		// 프레임 당 경과 시간 (sec)

public:
	static   CChronoTimer*      Create(void);
	virtual void      Free(void);
};

END
#endif // Timer_h__