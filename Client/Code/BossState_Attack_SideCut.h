#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Attack_SideCut :
	public CState<CBoss>
{
public:
	CBossState_Attack_SideCut(CBoss& _rOwner);
	virtual ~CBossState_Attack_SideCut();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;
};

END
