#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "InputMode.h"

BEGIN(Engine)

class ENGINE_DLL CInputModeMgr final : public CBase
{
private:
	explicit CInputModeMgr();
	virtual ~CInputModeMgr();

public:
	//LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	_bool ConfirmValidInputMode(void);				// 다음 씬이 존재한다면, 해당씬으로 변경한다.
	void SetNextInputMode(CInputMode* _pNextScene);		// nullptr을 전달하거나, 초기 씬이 세팅되지 않으면 프로그램이 종료된다.
	_int ProcessInput(const _float& _fDeltaTime);
	CInputMode* GetCurInputMode(void) const { return m_pCurInputMode; }

public:
	static CInputModeMgr* Create(void);
	virtual void Free(void);

private:
	CInputMode* m_pCurInputMode = nullptr;
	CInputMode* m_pNextInputMode = nullptr;
	bool m_bIsConfirmed = true;
};

END