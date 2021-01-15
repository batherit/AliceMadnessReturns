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
class CAliceWState_JumpDamage :
	public CState<CAliceW>
{
public:
	CAliceWState_JumpDamage(CAliceW& _rOwner);
	virtual ~CAliceWState_JumpDamage();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	enum E_JumpDamage_STEP { STEP_FALL, STEP_LAND, STEP_END };

private:
	Engine::CNaviMesh* m_pNaviMesh = nullptr;
	E_JumpDamage_STEP m_eJumpStep = STEP_FALL;
	_bool m_bIsAnimChanging = false;
};

END
