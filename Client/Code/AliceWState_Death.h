#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CUI_InGame;
class CAliceWState_Death :
	public CState<CAliceW>
{
public:
	CAliceWState_Death(CAliceW& _rOwner);
	virtual ~CAliceWState_Death();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState��(��) ���� ��ӵ�
	virtual void Free(void) override;

private:
	CUI_InGame* m_pInGameUI = nullptr;
	_float m_fElapsedTime = 0.f;
};

END
