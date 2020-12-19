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
class CAliceWState_Jump :
	public CState<CAliceW>
{
public:
	CAliceWState_Jump(CAliceW& _rOwner);
	virtual ~CAliceWState_Jump();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	enum E_JUMP_TYPE { TYPE_FORWARD, TYPE_RIGHT, TYPE_LEFT, TYPE_END };
	enum E_JUMP_STEP { STEP_START, STEP_RISE, STEP_FALL, STEP_LAND, STEP_END };

private:
	Engine::CNaviMesh* m_pNaviMesh = nullptr;
	_int m_iJumpNum = 0;
	_bool m_bIsJumping = true;
	E_JUMP_TYPE m_eJumpType = TYPE_FORWARD;
	E_JUMP_STEP m_eJumpStep = STEP_START;
};

END
