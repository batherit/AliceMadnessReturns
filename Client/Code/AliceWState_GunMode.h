#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CDynamicObject;
class CAliceW;
class CUI_GunGauge;
class CAliceWState_GunMode :
	public CState<CAliceW>
{
#define FIRE_DELAY_TIME  0.2f
public:
	enum E_STATE_TYPE { TYPE_FIRE, TYPE_NOAMMO, TYPE_RELEASE, TYPE_IDLE_OR_RUN, TYPE_END};
public:
	CAliceWState_GunMode(CAliceW& _rOwner);
	virtual ~CAliceWState_GunMode();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	E_STATE_TYPE m_eStateType = TYPE_FIRE;
	CUI_GunGauge* m_pGunGauge = nullptr;
	CDynamicObject* m_pGun = nullptr;
	_float m_fFireTickTime = 0.f;
};

END
