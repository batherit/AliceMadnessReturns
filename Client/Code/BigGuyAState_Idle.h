#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBigGuyA;
class CBigGuyAState_Idle :
	public CState<CBigGuyA>
{
public:
	CBigGuyAState_Idle(CBigGuyA& _rOwner);
	virtual ~CBigGuyAState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_float m_fKeepTime = 0.f;
	_bool m_bIsNextGo = true;
};

END
