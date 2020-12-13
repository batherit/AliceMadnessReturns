#include "ColliderObject_AABB.h"
#include "PolygonRenderer.h"
#include "CubeCol.h"
#include "Export_Resouces.h"
#include "Transform.h"


USING(Engine)

CColliderObject_AABB::CColliderObject_AABB(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CColliderObject(pGraphicDev)
{
}

CColliderObject_AABB::CColliderObject_AABB(const CColliderObject_AABB & rhs)
	:
	CColliderObject(rhs)
{
}

CColliderObject_AABB::~CColliderObject_AABB()
{
}

HRESULT CColliderObject_AABB::Ready_Object(void)
{
	SetColliderTag(L"Col_AABB");
	SetColliderType(TYPE_AABB);

	m_pCubeBuffer = dynamic_cast<CCubeCol*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_CubeCol"));
	NULL_CHECK_RETURN(m_pCubeBuffer, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pCubeBuffer);

	m_pRenderer = AddComponent<CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(RENDER_NONALPHA, m_pCubeBuffer);

	return S_OK;
}

_int CColliderObject_AABB::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CColliderObject_AABB::Render_Object(void)
{
	_matrix matNRotWorld = GetTransform()->GetObjectMatrix();
	_matrix matScale, matTrans;
	Engine::SeparateMatrix(&matScale, nullptr, &matTrans, &matNRotWorld);
	matNRotWorld = matScale * matTrans;

	m_pRenderer->SetWorldMatrix(matNRotWorld);
	m_pRenderer->Render();
}

CColliderObject_AABB * CColliderObject_AABB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderObject_AABB*	pInstance = new CColliderObject_AABB(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

// 부모를 세팅하고 SetRadius를 호출해야 정상적으로 보인다.
void CColliderObject_AABB::SetRadius(_float _fRadius)
{
	if (_fRadius <= 0.f)
		return;

	m_fRadius = _fRadius;
	GetTransform()->SetScale(_vec3(_fRadius, _fRadius, _fRadius));
}

_float CColliderObject_AABB::GetRadiusW() const {
	_matrix matScale;

	D3DXMatrixIdentity(&matScale);
	if (GetParent()) {
		Engine::SeparateMatrix(&matScale, nullptr, nullptr, &GetTransform()->GetParentMatrix());
	}

	// xyz에 대한 각 스케일 값들의 평균에 반지름을 곱한다.
	return m_fRadius * (matScale._11 + matScale._22 + matScale._33) / 3.f;
}

void CColliderObject_AABB::Free(void)
{
	CColliderObject::Free();
}

