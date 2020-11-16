#include "Component.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"

BEGIN(Engine)

CSphereCollider::CSphereCollider()
	:
	CComponent()
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& collider)
	:
	CComponent(collider)
{
}

CSphereCollider::~CSphereCollider() {

}

_vec3 CSphereCollider::GetColliderPos()
{
	_vec3 vPos(0.f, 0.f, 0.f);

	if (m_pOwner) {
		vPos = m_pOwner->GetTransform()->GetPos();
	}
	return vPos + m_vOffset;
}

CComponent* CSphereCollider::Create() {
	CSphereCollider* pInstance = new CSphereCollider;

	if (FAILED(pInstance->Ready()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CSphereCollider::Clone(void) {
	CSphereCollider* pInstance = new CSphereCollider(*this);
	return pInstance;
}

void CSphereCollider::Free(void) {
}

END
