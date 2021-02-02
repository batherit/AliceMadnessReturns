#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CGiantAliceW;
class CGiantAliceWState_Walk :
	public CState<CGiantAliceW>
{
public:
	CGiantAliceWState_Walk(CGiantAliceW& _rOwner);
	virtual ~CGiantAliceWState_Walk();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	void PlayStepSound();

private:
	Engine::CCameraMgr* m_pCameraMgr = nullptr;
	_bool m_bIsLeftFootOnRightFoot = true; // 초기
	_float m_fElapsedTime = 0.f;
};

END
