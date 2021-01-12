#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBigGuyB;
class CBigGuyBState_Walk :
	public CState<CBigGuyB>
{
public:
	CBigGuyBState_Walk(CBigGuyB& _rOwner);
	virtual ~CBigGuyBState_Walk();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_float m_fKeepTime = 0.f;
	_vec3 m_vTargetPos;
	_vec3 m_vDir;
};

END
