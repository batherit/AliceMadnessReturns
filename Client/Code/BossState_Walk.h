#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Walk :
	public CState<CBoss>
{
public:
	enum E_DIR { DIR_FORWARD, DIR_RIGHT, DIR_BACK, DIR_LEFT, DIR_END };

public:
	CBossState_Walk(CBoss& _rOwner);
	virtual ~CBossState_Walk();

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
	E_DIR m_eDir = DIR_END;
	_float m_fWanderingTime = 0.f;
};

END
