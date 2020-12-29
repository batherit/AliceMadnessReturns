#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMadCapA;
class CMap;
class CMadCapAState_Alert :
	public CState<CMadCapA>
{
public:
	CMadCapAState_Alert(CMadCapA& _rOwner);
	virtual ~CMadCapAState_Alert();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

	
};

END
