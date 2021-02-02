#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMiniGrunt;
class CMiniGruntState_Attack :
	public CState<CMiniGrunt>
{
public:
	CMiniGruntState_Attack(CMiniGrunt& _rOwner);
	virtual ~CMiniGruntState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	void PlayAttackSound();

private:
	_bool m_bIsAttackEnd = false;
};

END
