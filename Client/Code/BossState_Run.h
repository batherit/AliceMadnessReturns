#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Run :
	public CState<CBoss>
{
#define ACC_TIME	0.2f
public:
	CBossState_Run(CBoss& _rOwner);
	virtual ~CBossState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_float m_fWanderingTime = 0.f;
	_float m_fAccTime = ACC_TIME;	// 0.2f초만에 최고속도 도달.
};

END
