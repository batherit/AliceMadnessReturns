#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMadCapA;
class CMadCapAState_Attack :
	public CState<CMadCapA>
{
public:
	CMadCapAState_Attack(CMadCapA& _rOwner);
	virtual ~CMadCapAState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_bool m_bIsAttacking = false;
};

END
