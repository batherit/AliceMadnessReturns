#pragma once
#include "Component.h"

BEGIN(Engine)
class CRenderer;
class ENGINE_DLL CSphereCollider : public CComponent {
protected:
	explicit CSphereCollider();
	explicit CSphereCollider(const CSphereCollider& collider);
	virtual ~CSphereCollider(void);
public:
	virtual HRESULT Ready() { return S_OK; }
	virtual _int	Update(const _float& fTimeDelta) { return 0; }

public:
	void SetSphereColliderInfo(const _float& _fRadius, const _vec3& _vOffset = _vec3(0.f, 0.f, 0.f)) {
		m_fRadius = _fRadius;
		if (m_fRadius < 0.f) m_fRadius = 0.f;
		m_vOffset = _vOffset;
	}
	_vec3 GetColliderPos();
	_float GetRadius() const { return m_fRadius; }

public:
	static const _tchar* GetComponentTag() { return L"SphereComponent"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }
	static CComponent* Create();
	virtual CComponent* Clone(void) override;
	virtual void Free(void) override;

private:
	_float m_fRadius = 1.f;
	_vec3 m_vOffset{ 0.f, 0.f, 0.f };
};
END