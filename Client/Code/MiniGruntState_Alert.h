#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMiniGrunt;
class CMap;
class CMiniGruntState_Alert :
	public CState<CMiniGrunt>
{
public:
	enum E_STATE_TYPE { TYPE_PURSUE, TYPE_ALERT, TYPE_EVADE, TYPE_END };
	enum E_DIR { DIR_FORWARD, DIR_RIGHT, DIR_BACK, DIR_LEFT, DIR_END };

public:
	CMiniGruntState_Alert(CMiniGrunt& _rOwner);
	virtual ~CMiniGruntState_Alert();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	E_DIR GetVelDirXZType(const _vec2& _vVelDirXZ);

private:
	E_STATE_TYPE m_eStateType = TYPE_PURSUE;
	E_DIR m_eDir = DIR_END;
	_bool m_bIsRightMoving = true;
	_float m_fTickTime = 0.f;
	_float m_fElapsedTime = 0.f;
};

END
