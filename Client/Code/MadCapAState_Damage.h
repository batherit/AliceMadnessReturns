#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMadCapA;
class CMadCapAState_Damage :
	public CState<CMadCapA>
{
public:
	CMadCapAState_Damage(CMadCapA& _rOwner, _bool _bIsBigAttack = false);
	virtual ~CMadCapAState_Damage();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	void PlayDamagedSound();

private:
	_bool m_bIsBigAttack = false;
};

END
