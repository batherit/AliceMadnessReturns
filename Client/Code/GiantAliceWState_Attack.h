#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CGiantAliceW;
class CGiantAliceWState_Attack :
	public CState<CGiantAliceW>
{
public:
	CGiantAliceWState_Attack(CGiantAliceW& _rOwner);
	virtual ~CGiantAliceWState_Attack();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	_bool m_bIsSwingSoundOn = false;
};

END
