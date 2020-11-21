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
	_bool ConfirmValidInputMode(void);				// ���� ���� �����Ѵٸ�, �ش������ �����Ѵ�.
	void SetNextInputMode(CInputMode* _pNextScene);		// nullptr�� �����ϰų�, �ʱ� ���� ���õ��� ������ ���α׷��� ����ȴ�.
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