#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Attack_Mixer :
	public CState<CBoss>
{
public:
	CBossState_Attack_Mixer(CBoss& _rOwner);
	virtual ~CBossState_Attack_Mixer();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_float m_fElapsedTime = 0.f;
	_bool m_bIsStopping = false;
	_float m_fLoopSpinTime = 0.f;
};

END
