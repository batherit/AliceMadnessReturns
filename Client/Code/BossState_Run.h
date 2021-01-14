#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CBoss;
class CBossState_Run :
	public CState<CBoss>
{
#define ACC_TIME	0.2f
public:
	CBossState_Run(CBoss& _rOwner);
	virtual ~CBossState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_float m_fWanderingTime = 0.f;
	_float m_fAccTime = ACC_TIME;	// 0.2f�ʸ��� �ְ�ӵ� ����.
};

END
