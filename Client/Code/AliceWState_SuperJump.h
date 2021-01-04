#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"

BEGIN(Engine)
class CNaviMesh;
END

BEGIN(Client)
class CAliceW;
class CAliceWState_SuperJump :
	public CState<CAliceW>
{
public:
	CAliceWState_SuperJump(CAliceW& _rOwner);
	virtual ~CAliceWState_SuperJump();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;
};

END
