#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBolterFly;
class CBolterFlyState_Idle :
	public CState<CBolterFly>
{
public:
	CBolterFlyState_Idle(CBolterFly& _rOwner);
	virtual ~CBolterFlyState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_bool m_bIsAttackIdle = false;
	_float m_fElapsedTime = 0.f;
};

END
