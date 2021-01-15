#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CAliceWState_Dash :
	public CState<CAliceW>
{
#define DASH_TIME 0.6f
public:
	CAliceWState_Dash(CAliceW& _rOwner, const _vec3& _vDir);
	virtual ~CAliceWState_Dash();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	_vec3 m_vDir;
	_float m_fElapsedTime = 0.f;
};

END
