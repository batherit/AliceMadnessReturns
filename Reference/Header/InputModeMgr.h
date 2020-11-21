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
	void SetNextScene(CInputMode* _pNextScene);		// nullptr�� �����ϰų�, �ʱ� ���� ���õ��� ������ ���α׷��� ����ȴ�.
	void RequestSceneInit(void) { m_bInit = false; }
	void Update(const _float& _fDeltaTime);
	void Render(void);
	CInputMode* GetCurScene(void) const { return m_pCurScene; }

	//void Release(void);
	//void LateUpdate(void);
	//void Render(HDC & _hdc, CCamera * _pCamera);
	//void Render(CCamera* _pCamera); 

public:
	static CInputModeMgr* Create(void);
	virtual void Free(void);

private:
	//CGameWorld& m_rGameWorld;
	CInputMode* m_pCurScene = nullptr;
	CInputMode* m_pNextScene = nullptr;
	bool m_bIsConfirmed = true;
	bool m_bInit = true;		// �� �����ڿ��� �ʱ� �̴��� �����ϹǷ� ���ô� true
};

END