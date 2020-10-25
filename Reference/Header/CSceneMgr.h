#pragma once

#include "Engine_Define.h"
#include "Base.h"

#include "CScene.h"

BEGIN(Engine)

class ENGINE_DLL CSceneMgr final : public CBase
{
private:
	explicit CSceneMgr();
	virtual ~CSceneMgr();

public:
	//LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	_bool ConfirmValidScene(void);				// 다음 씬이 존재한다면, 해당씬으로 변경한다.
	void SetNextScene(CScene* _pNextScene);		// nullptr을 전달하거나, 초기 씬이 세팅되지 않으면 프로그램이 종료된다.
	void RequestSceneInit(void) { m_bInit = false; }
	void Update(const _float& _fDeltaTime);
	void Render(void);
	CScene* GetCurScene(void) const { return m_pCurScene; }

	//void Release(void);
	//void LateUpdate(void);
	//void Render(HDC & _hdc, CCamera * _pCamera);
	//void Render(CCamera* _pCamera); 

public:
	static CSceneMgr* Create(void);
	virtual void Free(void);

private:
	//CGameWorld& m_rGameWorld;
	CScene* m_pCurScene = nullptr;
	CScene* m_pNextScene = nullptr;
	bool m_bIsConfirmed = true;
	bool m_bInit = true;		// 씬 생성자에서 초기 이닛을 진행하므로 평상시는 true
};

END