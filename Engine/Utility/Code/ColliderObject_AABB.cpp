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
	if (m_bIsAutoGenerating) {
		m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix(Engine::CTransform::COORD_TYPE_LOCAL));
	}
	else {
		_matrix matNRotWorld = GetTransform()->GetObjectMatrix();
		_matrix matScale, matTrans;
		Engine::SeparateMatrix(&matScale, nullptr, &matTrans, &matNRotWorld);
		matNRotWorld = matScale * matTrans;
		m_pRenderer->SetWorldMatrix(matNRotWorld);
	}
	
	m_pRenderer->Render();
}

_bool CColliderObject_AABB::SaveInfo(HANDLE& _hfOut)
{
	DWORD dwByte = 0;

	_vec3 vPos = GetTransform()->GetPos(Engine::CTransform::COORD_TYPE_LOCAL);
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);
	
	// 자동 생성 AABB인지?
	WriteFile(_hfOut, &m_bIsAutoGenerating, sizeof(m_bIsAutoGenerating), &dwByte, nullptr);

	return true;
}

_bool CColliderObject_AABB::LoadInfo(HANDLE& _hfIn)
{
	DWORD dwByte = 0;

	_vec3 vPos, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetScale(vScale);

	// 자동 생성 AABB인지?
	ReadFile(_hfIn, &m_bIsAutoGenerating, sizeof(m_bIsAutoGenerating), &dwByte, nullptr);

	return true;
}

CColliderObject_AABB * CColliderObject_AABB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderObject_AABB*	pInstance = new CColliderObject_AABB(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CColliderObject_AABB::GetCollisionInfo(_vec3 & _vMin, _vec3 & _vMax)
{
	if (m_bIsAutoGenerating) {
		_vMin = m_vMinW;
		_vMax = m_vMaxW;
	}
	else {
		_matrix matNRotWorld = GetTransform()->GetObjectMatrix();
		_matrix matScale, matTrans;
		Engine::SeparateMatrix(&matScale, nullptr, &matTrans, &matNRotWorld);
		matNRotWorld = matScale * matTrans;

		D3DXVec3TransformCoord(&_vMin, &_vec3(-0.5f, -0.5f, -0.5f), &matNRotWorld);
		D3DXVec3TransformCoord(&_vMax, &_vec3(0.5f, 0.5f, 0.5f), &matNRotWorld);
	}
}

BOXINFO CColliderObject_AABB::GetBoxInfo()
{
	static const _vec3 vPoint[8] = {
		_vec3(-0.5f, 0.5f, -0.5f), _vec3(0.5f, 0.5f, -0.5f), _vec3(0.5f, -0.5f, -0.5f), _vec3(-0.5f, -0.5f, -0.5f),
		_vec3(-0.5f, 0.5f, 0.5f), _vec3(0.5f , 0.5f, 0.5f), _vec3(0.5f , -0.5f, 0.5f), _vec3(-0.5f, -0.5f, 0.5f)

	};

	BOXINFO tBoxInfo;

	_matrix matWorld = GetTransform()->GetObjectMatrix();
	_matrix matScale, matTrans;
	Engine::SeparateMatrix(&matScale, nullptr, &matTrans, &matWorld);
	matWorld = matScale * matTrans;

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

_bool CColliderObject_AABB::GenerateAABB()
{
	// 메쉬 구해서 자동으로 AABB 만들어줌.
	if (!GetParent())
		return false;

	Engine::CStaticMesh* pStaticMesh = GetParent()->GetComponent<Engine::CStaticMesh>();

	if (!pStaticMesh)
		return false;

	const _vec3* pVtxPosL = pStaticMesh->Get_VtxPos();
	const _ulong dwNumVtxCnt = pStaticMesh->Get_NumVtx();
	const _ulong dwStride = pStaticMesh->Get_Stride();

	_vec3* pVtxPosW = new _vec3[dwNumVtxCnt];
	_matrix matWorld = GetTransform()->GetParentMatrix();
	for (_int i = 0; i < dwNumVtxCnt; ++i) {
		// 월드 정점으로 변환
		D3DXVec3TransformCoord(&pVtxPosW[i], &pVtxPosL[i], &matWorld);
	}
	D3DXComputeBoundingBox(pVtxPosW, dwNumVtxCnt, sizeof(_vec3), &m_vMinW, &m_vMaxW);
	delete[] pVtxPosW;

	_vec3 vCenter = (m_vMaxW + m_vMinW) * 0.5f;
	GetTransform()->SetPos(vCenter);
	GetTransform()->SetAngle(0.f, 0.f, 0.f);
	GetTransform()->SetScale(_vec3((m_vMaxW.x - vCenter.x) / 0.5f, 
		(m_vMaxW.y - vCenter.y) / 0.5f, 
		(m_vMaxW.z - vCenter.z) / 0.5f));

	return true;
}

void CColliderObject_AABB::Free(void)
{
	CColliderObject::Free();
}

