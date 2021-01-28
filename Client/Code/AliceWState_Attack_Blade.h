#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Typedef.h"
#include "State.h"


BEGIN(Client)
class CAliceW;
class CAliceWState_Attack_Blade :
	public CState<CAliceW>
{
public:
	CAliceWState_Attack_Blade(CAliceW& _rOwner);
	virtual ~CAliceWState_Attack_Blade();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(const _float& _fDeltaTime) override;
	//virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

	// CState을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	const _float m_fEntryTime[5] = { 0.f, 0.3f, 0.5f, 0.6f, 1.f };
	const _float m_fEndTime[5] = { 0.f, 0.5f, 0.6f, 0.6f, 0.6f };
	_int m_iAttackNum = 0;
	_bool m_bIsAttacking = true;
	Engine::CGameObject* m_pWeaponCollider = nullptr;
};

END
