#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBigGuyB;
class CBigGuyBState_Idle :
	public CState<CBigGuyB>
{
public:
	CBigGuyBState_Idle(CBigGuyB& _rOwner);
	virtual ~CBigGuyBState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_float m_fKeepTime = 0.f;
	_bool m_bIsNextGo = true;
};

END
