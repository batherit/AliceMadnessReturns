#pragma once

#include "Engine_Define.h"
#include "Base.h"
//#include "InputProcessor.h"

BEGIN(Engine)
class CInputProcessor;
class ENGINE_DLL CInputProcessorMgr final : public CBase
{
private:
	explicit CInputProcessorMgr();
	virtual ~CInputProcessorMgr();

public:
	//LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	_bool ConfirmValidInputProcessor(void);				// 다음 씬이 존재한다면, 해당씬으로 변경한다.
	void SetNextInputProcessor(CInputProcessor* _pNextScene);		// nullptr을 전달하거나, 초기 씬이 세팅되지 않으면 프로그램이 종료된다.
	_int ProcessInput(const _float& _fDeltaTime);
	CInputProcessor* GetCurInputProcessor(void) const { return m_pCurInputProcessor; }

public:
	static CInputProcessorMgr* Create(void);
	virtual void Free(void);

private:
	CInputProcessor* m_pCurInputProcessor = nullptr;
	CInputProcessor* m_pNextInputProcessor = nullptr;
	bool m_bIsConfirmed = true;
};

END