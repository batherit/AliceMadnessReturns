#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CMiniGrunt;
class CMiniGruntState_Death :
	public CState<CMiniGrunt>
{
public:
	CMiniGruntState_Death(CMiniGrunt& _rOwner);
	virtual ~CMiniGruntState_Death();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	void PlayDieSound();

private:
	_float m_fElapsedTime = 0.f;
};

END
