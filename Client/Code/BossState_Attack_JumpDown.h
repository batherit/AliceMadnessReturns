#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Attack_JumpDown :
	public CState<CBoss>
{
public:
	CBossState_Attack_JumpDown(CBoss& _rOwner);
	virtual ~CBossState_Attack_JumpDown();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_bool m_bIsJumpStarting = true;
	_bool m_bIsTargeting = false;
	_bool m_bIsAttackEnd = false;
	_bool m_bIsBack = false;
	_vec3 m_vStartPos;
	_vec3 m_vTargetPos;
	_float m_fElapsedTime = 0.f;
};

END
