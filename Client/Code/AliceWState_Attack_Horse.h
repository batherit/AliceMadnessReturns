#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CAliceWState_Attack_Horse :
	public CState<CAliceW>
{
public:
	CAliceWState_Attack_Horse(CAliceW& _rOwner);
	virtual ~CAliceWState_Attack_Horse();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	const _float m_fEntryTime[5] = { 0.f, 0.4f, 0.7f, 0.6f };
	const _float m_fStartTime[5] = { 0.1f, 0.5f, 0.4f, 0.4f, 0.4f };
	const _float m_fEndTime[5] = { 0.f, 0.8f, 0.7f, 0.8f, 0.8f };

	_int m_iAttackNum = 0;
	_bool m_bIsAttacking = true;
	Engine::CGameObject* m_pWeaponCollider = nullptr;
};

END
