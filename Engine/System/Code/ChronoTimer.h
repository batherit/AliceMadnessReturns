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
	void Reset();						// Ÿ�̸� �Ӽ��� �ʱ�ȭ
	void RunTick();						// �� ���� ƽ (�� ����� �ð��� �����Ѵ�.)
	void RunToPause();					// �Ͻ� ����
	void PauseToRun();					// Ÿ�̸� �۵� (���� ȣ�� �� ù ȣ�� �������� �ð� ����)

	_float GetElapsedTimePerFrame();		// ������ �� ����� �ð�(sec)
	_float GetCurTime();					// ���� �ð�(sec)
	_float GetTotalTimeFromRun();		// ���� RunTick ���� �� ����� �ð�

private:
	_bool m_bIsPaused;					// �Ͻ������Ǿ�����?
	TIME_POINT m_timeBase;					// ������ �⺻ �ð� 
	TIME_POINT m_timeBefore;				// ���� �ð� 
	TIME_POINT m_timeCurrent;				// ���� �ð� 
	TIME_POINT m_timePausePoint;  		// �Ͻ������� ���� 

	_float m_fPauseTotalTime;		// ������ �Ͻ����� �� �ð� 
	_float m_fElapsedTimePerFrame; 		// ������ �� ��� �ð� (sec)

public:
	static   CChronoTimer*      Create(void);
	virtual void      Free(void);
};

END
#endif // Timer_h__