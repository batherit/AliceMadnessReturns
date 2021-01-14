#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBolterFly;
class CBolterFlyState_Attack :
	public CState<CBolterFly>
{
public:
	CBolterFlyState_Attack(CBolterFly& _rOwner);
	virtual ~CBolterFlyState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_bool m_bIsAttackEnd = false;
	_float m_fElapsedTime = 0.f;
};

END
