#include "ColliderObject_OBB.h"
#include "PolygonRenderer.h"
#include "CubeCol.h"
#include "Export_Resouces.h"
#include "Transform.h"


USING(Engine)

CColliderObject_OBB::CColliderObject_OBB(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CColliderObject(pGraphicDev)
{
}

CColliderObject_OBB::CColliderObject_OBB(const CColliderObject_OBB & rhs)
	:
	CColliderObject(rhs)
{
}

CColliderObject_OBB::~CColliderObject_OBB()
{
}

HRESULT CColliderObject_OBB::Ready_Object(void)
{
	SetColliderTag(L"Col_OBB");
	SetColliderType(TYPE_OBB);

	m_pCubeBuffer = dynamic_cast<CCubeCol*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_CubeCol"));
	NULL_CHECK_RETURN(m_pCubeBuffer, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", m_pCubeBuffer);

	m_pRenderer = AddComponent<CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(RENDER_NONALPHA, m_pCubeBuffer);

	return S_OK;
}

_int CColliderObject_OBB::Update_Object(const _float & _fDeltaTime)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CColliderObject_OBB::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
}

_bool CColliderObject_OBB::SaveInfo(HANDLE& _hfOut)
{
	DWORD dwByte = 0;

	_vec3 vPos = GetTransform()->GetPos(Engine::CTransform::COORD_TYPE_LOCAL);
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	return true;
}

_bool CColliderObject_OBB::LoadInfo(HANDLE& _hfIn)
{
	DWORD dwByte = 0;

	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	return true;
}

CColliderObject_OBB * CColliderObject_OBB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderObject_OBB*	pInstance = new CColliderObject_OBB(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

// �θ� �����ϰ� SetRadius�� ȣ���ؾ� ���������� ���δ�.
void CColliderObject_OBB::SetRadius(_float _fRadius)
{
	if (_fRadius <= 0.f)
		return;

	m_fRadius = _fRadius;
	GetTransform()->SetScale(_vec3(_fRadius, _fRadius, _fRadius));
}

_float CColliderObject_OBB::GetRadiusW() const {
	_matrix matScale;

	D3DXMatrixIdentity(&matScale);
	if (GetParent()) {
		Engine::SeparateMatrix(&matScale, nullptr, nullptr, &GetTransform()->GetParentMatrix());
	}

	// xyz�� ���� �� ������ ������ ��տ� �������� ���Ѵ�.
	return m_fRadius * (matScale._11 + matScale._22 + matScale._33) / 3.f;
}

void CColliderObject_OBB::Free(void)
{
	CColliderObject::Free();
}

