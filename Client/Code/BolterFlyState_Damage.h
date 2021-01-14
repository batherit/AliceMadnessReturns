#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBolterFly;
class CBolterFlyState_Damage :
	public CState<CBolterFly>
{
public:
	CBolterFlyState_Damage(CBolterFly& _rOwner, _bool _bIsBigAttack = false);
	virtual ~CBolterFlyState_Damage();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_bool m_bIsBigAttack = false;
};

END
