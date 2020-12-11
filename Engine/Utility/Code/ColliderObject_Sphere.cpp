#include "ColliderObject_Sphere.h"
#include "PolygonRenderer.h"
#include "SphereCol.h"
#include "Export_Resouces.h"
#include "Transform.h"


USING(Engine)

CColliderObject_Sphere::CColliderObject_Sphere(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CColliderObject(pGraphicDev)
{
}

CColliderObject_Sphere::CColliderObject_Sphere(const CColliderObject_Sphere & rhs)
	:
	CColliderObject(rhs)
{
}

CColliderObject_Sphere::~CColliderObject_Sphere()
{
}

HRESULT CColliderObject_Sphere::Ready_Object(void)
{
	SetColliderTag(L"Col_Sphere");
	m_eColliderType = TYPE_SPHERE;

	m_pSphereBuffer = dynamic_cast<CSphereCol*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_SphereCol"));
	NULL_CHECK_RETURN(m_pSphereBuffer, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pSphereBuffer);

	m_pRenderer = AddComponent<CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(RENDER_NONALPHA, m_pSphereBuffer);

	return S_OK;
}

_int CColliderObject_Sphere::Update_Object(const _float & _fDeltaTime)
{
	
	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CColliderObject_Sphere::Render_Object(void)
{
	m_pRenderer->Render();
}

CColliderObject_Sphere * CColliderObject_Sphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderObject_Sphere*	pInstance = new CColliderObject_Sphere(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

// 부모를 세팅하고 SetRadius를 호출해야 정상적으로 보인다.
void CColliderObject_Sphere::SetRadius(_float _fRadius)
{
	if (_fRadius <= 0.f)
		return;

	m_fRadius = _fRadius;

	if (GetParent()) {
		_vec3 vParentScale = GetParent()->GetTransform()->GetScale();
		GetTransform()->SetScale(_vec3(_fRadius / vParentScale.x, _fRadius / vParentScale.y, _fRadius / vParentScale.z));
	}
	else 
		GetTransform()->SetScale(_vec3(_fRadius, _fRadius, _fRadius));
}

void CColliderObject_Sphere::Free(void)
{
	CColliderObject::Free();
}

