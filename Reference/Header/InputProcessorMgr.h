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
	_bool ConfirmValidInputProcessor(void);				// ���� ���� �����Ѵٸ�, �ش������ �����Ѵ�.
	void SetNextInputProcessor(CInputProcessor* _pNextScene);		// nullptr�� �����ϰų�, �ʱ� ���� ���õ��� ������ ���α׷��� ����ȴ�.
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