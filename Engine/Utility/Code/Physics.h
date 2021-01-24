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
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
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
	void SetDirection(const _vec3& _vDir) { m_vDir = _vDir; D3DXVec3Normalize(&m_vDir, &m_vDir); }
	void SetVelocity(const _vec3& _vVelocity) { 
		SetSpeed(D3DXVec3Length(&_vVelocity)); // 기존 m_fSpeed = D3DXVec3Length(&_vVelocity);
		if(m_fSpeed > 0.f)
			D3DXVec3Normalize(&m_vDir, &_vVelocity); 
	}
	void AddVelocity(const _vec3& _vDeltaVelocity) {
		SetVelocity(GetVelocity() + _vDeltaVelocity);
	}
	void SetMaxTurnRateInRadian(_float _fMaxTurnRate) { m_fMaxTurnRate = _fMaxTurnRate; }
	void SetMaxTurnRateInDegree(_float _fMaxTurnRate) { m_fMaxTurnRate = D3DXToRadian(_fMaxTurnRate); }
	void SetGravity(_float _fGravity) { m_fGravity = _fGravity; }

	// Get
	_float GetSpeed() const { return m_fSpeed; }
	_float GetSpeedY() const { return GetVelocity().y; }
	_float GetSpeedXZ() const { _vec3 vVel = GetVelocity();  return D3DXVec2Length(&_vec2(vVel.x, vVel.y)); }
	_float GetMaxSpeed() const { return m_fMaxSpeed; }
	_vec3 GetDirection() const { return m_vDir; }
	_vec3 GetVelocity() const { return m_fSpeed * m_vDir; }
	_float GetMaxTurnRateInRadian() const { return m_fMaxTurnRate; }
	_float GetMaxTurnRateInDegree() const { return D3DXToDegree(m_fMaxTurnRate); }
	_float GetGravity() const { return m_fGravity; }

	void SetResistanceCoefficientXZ(const _float& _fResistanceCoefficient) { m_fResistanceCoefficientXZ = _fResistanceCoefficient; }
	void SetResistanceCoefficientY(const _float& _fResistanceCoefficient) { m_fResistanceCoefficientY = _fResistanceCoefficient; }

	void SetVelocityXZ(const _vec2& _vVelocityXZ) {
		_float fVelocityY = GetVelocity().y;
		SetVelocity(_vec3(_vVelocityXZ.x, fVelocityY, _vVelocityXZ.y));
	}
	void AddVelocityXZ(const _vec2& _vDeltaVelocityXZ) {
		_vec3 vVelocity = GetVelocity();
		SetVelocity(_vec3(vVelocity.x + _vDeltaVelocityXZ.x, vVelocity.y, vVelocity.z + _vDeltaVelocityXZ.y));
	}
	void SetVelocityY(const _float& _fVelocityY) {
		_vec3 vVelocity = GetVelocity();
		SetVelocity(_vec3(vVelocity.x, _fVelocityY, vVelocity.z));
	}
	void AddVelocityY(const _float& _fDeltaVelocityY) {
		_vec3 vVelocity = GetVelocity();
		SetVelocity(_vec3(vVelocity.x, vVelocity.y + _fDeltaVelocityY, vVelocity.z));
	}

	// Update
	_vec3 GetUpdatedVelocity(const _float& _fDeltaTime) {
		_vec3 vCurrentVelocity = GetVelocity();
		SetVelocity(_vec3(vCurrentVelocity.x * m_fResistanceCoefficientXZ, (vCurrentVelocity.y - m_fGravity * _fDeltaTime) * m_fResistanceCoefficientY, vCurrentVelocity.z * m_fResistanceCoefficientXZ));
		return GetVelocity();
	}

	_vec3 GetUpdatedPos(const _float& _fDeltaTime, CTransform::E_COORD_TYPE _eCoordType = CTransform::COORD_TYPE_WORLD) {
		return m_pOwner->GetTransform()->GetPos(_eCoordType) + GetUpdatedVelocity(_fDeltaTime) * _fDeltaTime;
	}

	void MoveByDelta(const _float& _fDeltaTime) {
		m_pOwner->GetTransform()->Translate(GetUpdatedVelocity(_fDeltaTime) * _fDeltaTime);
	}



private:
	//_float m_fMass = 0.f;
	_float m_fMaxSpeed = 9876543210.f;
	_float m_fSpeed = 0.f;	// 스칼라값으로 양수만 취급한다.
	//_float m_fMaxForce = 0.f;
	_float m_fMaxTurnRate = 2.f * D3DX_PI;	// (rad/s)
	_float m_fTurnRate = 2.f * D3DX_PI;
	_vec3 m_vDir = WORLD_X_AXIS;
	_float m_fGravity = 0.f;
	_float m_fResistanceCoefficientXZ = 1.f;
	_float m_fResistanceCoefficientY = 1.f;
};
END