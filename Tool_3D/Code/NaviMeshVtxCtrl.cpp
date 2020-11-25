#include "pch.h"
#include "NaviMeshVtxCtrl.h"
#include "NaviMesh.h"

CNaviMeshVtxCtrl::CNaviMeshVtxCtrl(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CNaviMeshVtxCtrl::CNaviMeshVtxCtrl(const CNaviMeshVtxCtrl & rhs)
	: CGameObject(rhs)
{
}

CNaviMeshVtxCtrl::~CNaviMeshVtxCtrl(void)
{
}

HRESULT CNaviMeshVtxCtrl::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	return E_NOTIMPL;
}

int CNaviMeshVtxCtrl::Update_Object(const _float & _fDeltaTime)
{
	if (!m_bIsActivated)
		return 0;

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// �浹�� ����� ã�´�.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);
			if (m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
		}
	}
	else if (m_bIsPicking) {
		if(Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
			switch (m_ePlaneType) {
			case PLANE::TYPE_XY:
				// TODO : XY ��鿡 ���� �̵� ����
			{int a = 10; }
				break;
			case PLANE::TYPE_XZ:
				// TODO : XZ ��鿡 ���� �̵� ����
			{int a = 10; }
				break;
			case PLANE::TYPE_YZ:
				// TODO : YZ ��鿡 ���� �̵� ����
			{int a = 10; }
				break;
			}
		}
		else {
			// �巡�װ� �����ٸ�, ��ŷ ���°� �ƴ� ���̴�.
			m_bIsPicking = false;
		}
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CNaviMeshVtxCtrl::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
}

CNaviMeshVtxCtrl * CNaviMeshVtxCtrl::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMeshVtxCtrl*	pInstance = new CNaviMeshVtxCtrl(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMeshVtxCtrl::Free(void)
{
	CGameObject::Free();
}

void CNaviMeshVtxCtrl::SetVertexInfo(CNaviMesh * _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex)
{
	if (!_pNaviMesh || !_pNaviMesh->IsValidIndex(_iTriangleIndex, _iVertexIndex))
		return;

	m_pNaviMesh = _pNaviMesh;
	m_iTriangleIndex = _iTriangleIndex;
	m_iVertexIndex = _iVertexIndex;

	// ���� ��ġ�� ��Ʈ���� ��ġ��Ų��.(����ȭ)
	GetTransform()->SetPos(_pNaviMesh->GetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex));
}

PLANE::E_TYPE CNaviMeshVtxCtrl::GetPlaneTypeByRay(Engine::PICKINGRAYINFO & _rRayInfo)
{
	PLANE::E_TYPE ePlaneType = PLANE::TYPE_END;
	_float fU, fV, fDist;
	_vec3 vMinHitPos;
	_vec3 vTempHitPos;
	_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
	_matrix matWorld = GetTransform()->GetObjectMatrix();

	auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
	// 1) XY ���
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// �ﰢ�� ���� ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XYTriLocal[i][j], &matWorld);
		}
		// �浹�ߴ����� üũ�Ѵ�.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// �浹�ߴٸ�, �浹 ��� Ÿ�԰� ���� �浹 ������ �����Ѵ�.
			vMinHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);
			ePlaneType = PLANE::TYPE_XY;
			break;
		}
	}
	// 2) XZ ���
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// �ﰢ�� ���� ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XZTriLocal[i][j], &matWorld);
		}
		// �浹�ߴ����� üũ�Ѵ�.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// �浹�ߴٸ�, �浹 ��� Ÿ�԰� ���� �浹 ������ �����Ѵ�.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// ���� ��Ʈ ������ �Ÿ��� ���Ͽ� ������ �ش� ��� Ÿ������ �����Ѵ�.
			if (D3DXVec3LengthSq(&(vMinHitPos - stPickingRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - stPickingRayInfo.vRayPos))) {
				vMinHitPos = vTempHitPos;
				ePlaneType = PLANE::TYPE_XZ;
				break;
			}
		}
	}
	// 3) YZ ���
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// �ﰢ�� ���� ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &YZTriLocal[i][j], &matWorld);
		}
		// �浹�ߴ����� üũ�Ѵ�.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// �浹�ߴٸ�, �浹 ��� Ÿ�԰� ���� �浹 ������ �����Ѵ�.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// ���� ��Ʈ ������ �Ÿ��� ���Ͽ� ������ �ش� ��� Ÿ������ �����Ѵ�.
			if (D3DXVec3LengthSq(&(vMinHitPos - stPickingRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - stPickingRayInfo.vRayPos))) {
				ePlaneType = PLANE::TYPE_YZ;
				break;
			}
		}
	}

	return ePlaneType;
}
