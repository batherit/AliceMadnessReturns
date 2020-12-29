#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMadCapA;
class CMadCapAState_Idle :
	public CState<CMadCapA>
{
public:
	CMadCapAState_Idle(CMadCapA& _rOwner);
	virtual ~CMadCapAState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_bool m_bIsAttackIdle = false;
	_float m_fElapsedTime = 0.f;
};

END
