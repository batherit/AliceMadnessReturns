#pragma once
#include "Component.h"
#include "Typedef.h"

BEGIN(Client)

class CAttribute : public Engine::CComponent {
protected:
	explicit CAttribute();
	explicit CAttribute(const CAttribute& Physics);
	virtual ~CAttribute(void);
public:
	virtual HRESULT Ready() override;
	virtual _int	Update(const _float& fTimeDelta) override;

public:
	static const _tchar* GetComponentTag() { return L"Attribute"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
	static CComponent* Create();
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

public:
	void SetHP(_float _fHP, _float _fMaxHP) {
		SetMaxHP(_fMaxHP);
		SetHP(_fHP);
	}
	void SetHP(_float _fHP) { m_fHP = _fHP; Engine::Clamp(&m_fHP, 0.f, m_fMaxHP); }
	void SetMaxHP(_float _fMaxHP) {
		m_fMaxHP = _fMaxHP;
		if (m_fMaxHP < 0.f)
			m_fMaxHP = 0.f;
		Engine::Clamp(&m_fHP, 0.f, m_fMaxHP);
	}

	void SetDamaged(_bool _bIsDamaged) { m_bIsDamaged = _bIsDamaged; }

	_float GetHP() const { return m_fHP; }
	_float GetMaxHP() const { return m_fMaxHP; }

	_bool IsDead() { return m_fHP <= 0.f; }
	_bool IsDamaged() const { return m_bIsDamaged; }

	// Update
	void IncreaseHP(_float _fAmount) { SetHP(GetHP() + _fAmount); }
	void Recover(_float _fAmount) { IncreaseHP(_fAmount); }
	void DecreaseHP(_float _fAmount) { SetHP(GetHP() - _fAmount); }
	void Damaged(_float _fAmount) { DecreaseHP(_fAmount); if (_fAmount > 0.f) m_bIsDamaged = true; }

	_bool RegisterAttacker(Engine::CGameObject* _pAttacker);
	_bool ReleaseAttacker(Engine::CGameObject* _pAttacker);
	void ClearAttackerList();

private:
	_float m_fMaxHP = 100.f;
	_float m_fHP = 100.f;
	_bool m_bIsDamaged = false;
	list<Engine::CGameObject*> m_AttackerList;
};
END