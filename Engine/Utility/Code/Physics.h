#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CPhysics : public CComponent {
protected:
	explicit CPhysics();
	explicit CPhysics(const CPhysics& Physics);
	virtual ~CPhysics(void);
public:
	virtual HRESULT Ready();
	virtual _int	Update(const _float& fTimeDelta);

public:
	static const _tchar* GetComponentTag() { return L"Physics"; }
	static CComponent* Create();
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

public:
	// Set
	void SetSpeed(_float _fSpeed, _float _fMaxSpeed) {
		SetMaxSpeed(_fMaxSpeed);
		SetSpeed(_fSpeed);
	}
	void SetSpeed(_float _fSpeed) { m_fSpeed = _fSpeed; Clamp(&m_fSpeed, 0.f, m_fMaxSpeed); }
	void SetMaxSpeed(_float _fMaxSpeed) {
		m_fMaxSpeed = _fMaxSpeed;
		if (m_fMaxSpeed < 0.f)
			m_fMaxSpeed = 0.f;
		Clamp(&m_fSpeed, 0.f, m_fMaxSpeed);
	}
	void SetDirection(_vec3 _vDir) { m_vDir = _vDir; D3DXVec3Normalize(&m_vDir, &m_vDir); }
	void SetVelocity(_vec3 _vVelocity) { m_fSpeed = D3DXVec3Length(&_vVelocity); D3DXVec3Normalize(&m_vDir, &_vVelocity); }
	void SetMaxTurnRateInRadian(_float _fMaxTurnRate) { m_fMaxTurnRate = _fMaxTurnRate; }
	void SetMaxTurnRateInDegree(_float _fMaxTurnRate) { m_fMaxTurnRate = D3DXToRadian(_fMaxTurnRate); }

	// Get
	_float GetSpeed() const { return m_fSpeed; }
	_float GetMaxSpeed() const { return m_fMaxSpeed; }
	_vec3 GetDirection() const { return m_vDir; }
	_vec3 GetVelocity() const { return m_fSpeed * m_vDir; }
	_float GetMaxTurnRateInRadian() const { return m_fMaxTurnRate; }
	_float GetMaxTurnRateInDegree() const { return D3DXToDegree(m_fMaxTurnRate); }

	// Util
	void MoveByDelta(const float& _fDeltaTime) {
		m_pOwner->GetTransform()->Translate(GetVelocity() * _fDeltaTime);
	}

private:
	//_float m_fMass = 0.f;
	_float m_fMaxSpeed = 9876543210.f;
	_float m_fSpeed = 0.f;	// 스칼라값으로 양수만 취급한다.
	//_float m_fMaxForce = 0.f;
	_float m_fMaxTurnRate = 2.f * D3DX_PI;	// (rad/s)
	_float m_fTurnRate = 2.f * D3DX_PI;
	_vec3 m_vDir = WORLD_X_AXIS;

};
END