#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMiniGrunt;
class CMiniGruntState_Damage :
	public CState<CMiniGrunt>
{
public:
	CMiniGruntState_Damage(CMiniGrunt& _rOwner, _bool _bIsBigAttack = false);
	virtual ~CMiniGruntState_Damage();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	void PlayDamagedSound();

private:
	_bool m_bIsBigAttack = false;
};

END
