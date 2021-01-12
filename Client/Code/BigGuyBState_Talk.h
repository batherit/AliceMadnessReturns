#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBigGuyB;
class CBigGuyBState_Talk :
	public CState<CBigGuyB>
{
public:
	CBigGuyBState_Talk(CBigGuyB& _rOwner);
	virtual ~CBigGuyBState_Talk();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_float m_fElapsedTime = 0.f;
	_float m_fKeepTime = 0.f;
	wstring m_wstrTalkText[3];
	_bool m_bIsNextGo = true;
};

END
