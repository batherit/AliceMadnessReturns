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

BOXINFO CColliderObject_OBB::GetBoxInfo()
{
	static const _vec3 vPoint[8] = {
		_vec3(-0.5f, 0.5f, -0.5f), _vec3(0.5f, 0.5f, -0.5f), _vec3(0.5f, -0.5f, -0.5f), _vec3(-0.5f, -0.5f, -0.5f),
		_vec3(-0.5f, 0.5f, 0.5f), _vec3(0.5f , 0.5f, 0.5f), _vec3(0.5f , -0.5f, 0.5f), _vec3(-0.5f, -0.5f, 0.5f)

	};

	BOXINFO tBoxInfo;

	_matrix matWorld = GetTransform()->GetObjectMatrix();
	// 월드 정점 세팅하기
	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tBoxInfo.vPoint[i], &vPoint[i], &matWorld);
	}
	// 센터 포지션 구하기
	D3DXVec3TransformCoord(&tBoxInfo.vCenter, &_vec3(0.f, 0.f, 0.f), &matWorld);

	// 모서리 구하기
	tBoxInfo.vProjAxis[0] = (tBoxInfo.vPoint[2] + tBoxInfo.vPoint[5]) * 0.5f - tBoxInfo.vCenter;
	tBoxInfo.vProjAxis[1] = (tBoxInfo.vPoint[0] + tBoxInfo.vPoint[5]) * 0.5f - tBoxInfo.vCenter;
	tBoxInfo.vProjAxis[2] = (tBoxInfo.vPoint[7] + tBoxInfo.vPoint[5]) * 0.5f - tBoxInfo.vCenter;

	// 축 구하기
	tBoxInfo.vAxis[0] = tBoxInfo.vPoint[2] - tBoxInfo.vPoint[3];
	tBoxInfo.vAxis[1] = tBoxInfo.vPoint[0] - tBoxInfo.vPoint[3];
	tBoxInfo.vAxis[2] = tBoxInfo.vPoint[7] - tBoxInfo.vPoint[3];

	for (_uint i = 0; i < 3; ++i)
		D3DXVec3Normalize(&tBoxInfo.vAxis[i], &tBoxInfo.vAxis[i]);
	
	return tBoxInfo;
}

void CColliderObject_OBB::Free(void)
{
	CColliderObject::Free();
}

