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
	_bool ConfirmValidScene(void);				// ���� ���� �����Ѵٸ�, �ش������ �����Ѵ�.
	void SetNextScene(CScene* _pNextScene);		// nullptr�� �����ϰų�, �ʱ� ���� ���õ��� ������ ���α׷��� ����ȴ�.
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
	bool m_bInit = true;		// �� �����ڿ��� �ʱ� �̴��� �����ϹǷ� ���ô� true
};

END