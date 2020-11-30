#include "pch.h"
#include "NaviMeshVtxMover.h"
#include "NaviMesh.h"
#include "CNaviMeshTab.h"
#include "MainFrm.h"
#include "CTabForm.h"

CNaviMeshVtxMover::CNaviMeshVtxMover(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CNaviMeshVtxMover::CNaviMeshVtxMover(const CNaviMeshVtxMover & rhs)
	: CGameObject(rhs)
{
}

CNaviMeshVtxMover::~CNaviMeshVtxMover(void)
{
}

HRESULT CNaviMeshVtxMover::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	GetTransform()->SetScale(_vec3(10.f, 10.f, 10.f));

	m_vecGroupList.reserve(100);
	return S_OK;
}

int CNaviMeshVtxMover::Update_Object(const _float & _fDeltaTime)
{
	if (!m_bIsMoverGizmoActivated)
		return 0;

	// �Ĺ��� ���°� �ƴҶ� ������Ʈ�˴ϴ�.

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// �浹�� ����� ã�´�.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);

			if (m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
			else {
				// �浹�� ����� ã�� ���� ���,
				// �浹 ������ ã�´�.
				if (m_pNaviMesh) {
					vector<_int> vecCollidedSphereIndex;
					vecCollidedSphereIndex.reserve(1);
					auto& vecNaviVertices = m_pNaviMesh->GetNaviVertices();
					_int iVecSize = vecNaviVertices.size();
					auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

					for (_int i = 0; i < iVecSize; ++i) {
						if (Engine::IsRayAndSphereCollided(stPickingRayInfo, 2.f, vecNaviVertices[i])) {
							if (vecCollidedSphereIndex.empty()) {
								vecCollidedSphereIndex.emplace_back(i);
							}
							else {
								if (D3DXVec3LengthSq(&(vecNaviVertices[vecCollidedSphereIndex[0]] - stPickingRayInfo.vRayPos))
									> D3DXVec3LengthSq(&(vecNaviVertices[i] - stPickingRayInfo.vRayPos))) {
									vecNaviVertices[0] = vecNaviVertices[i];
								}
							}
						}
					}

					CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
					CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
					auto pNaviMeshTab = pTabForm->GetNaviMeshTab();
					

					if (!vecCollidedSphereIndex.empty()) {
						// �浹 ������ ã�� ���,
						_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
						_int iVertexIndex = vecCollidedSphereIndex[0] % 3;

						SelectVertex(m_pNaviMesh, iTriangleIndex, iVertexIndex);

						pNaviMeshTab->m_chkGroup.EnableWindow(TRUE);

						// �ʱ� ���·� ���ư���.
						pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
						pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
						pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
						pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);

						if (pNaviMeshTab->m_chkGroup.GetCheck())
							FormGroup();
					}

					// UI ����ȭ
					pNaviMeshTab->UpdateVertexPos(GetVertexPos());
				}
			}
		}
	}
	else if (m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
			DragVertex();
		}
		else {
			// �巡�װ� �����ٸ�, ��ŷ ���°� �ƴ� ���̴�.
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CNaviMeshVtxMover::Render_Object(void)
{
	_matrix matWorld, matView, matProj;
	matWorld = m_pTransform->GetObjectMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTexture(0, NULL);

	// �� �׸���
	ID3DXLine *pLine;
	D3DXCreateLine(m_pGraphicDev, &pLine);
	pLine->SetWidth(2.f);
	pLine->SetAntialias(true);
	pLine->Begin();

	for (_int i = 0; i < AXIS_NUM; ++i) {
		if (m_ePlaneType == PLANE::TYPE_XY && i == 2) continue;
		else if (m_ePlaneType == PLANE::TYPE_XZ && i == 1) continue;
		else if (m_ePlaneType == PLANE::TYPE_YZ && i == 0) continue;

		pLine->DrawTransform(m_vAxisVertices[i], 2, &(matWorld * matView * matProj), m_clAxisColor[i]);
	}
	pLine->End();
	pLine->Release();

}

CNaviMeshVtxMover * CNaviMeshVtxMover::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMeshVtxMover*	pInstance = new CNaviMeshVtxMover(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMeshVtxMover::Free(void)
{
	CGameObject::Free();
}

void CNaviMeshVtxMover::SelectVertex(CNaviMesh * _pNaviMesh, _int _iTriangleIndex, _int _iVertexIndex)
{
	if (!_pNaviMesh || !_pNaviMesh->IsValidIndex(_iTriangleIndex, _iVertexIndex))
		return;

	m_pNaviMesh = _pNaviMesh;
	m_iTriangleIndex = _iTriangleIndex;
	m_iVertexIndex = _iVertexIndex;

	// ���� ��ġ�� �̵��ڸ� ��ġ��Ų��.(����ȭ)
	GetTransform()->SetPos(_pNaviMesh->GetTriangleVertexPosition(_iTriangleIndex, _iVertexIndex));
}

void CNaviMeshVtxMover::SetVertexPos(const _vec3 & _vPos)
{
	if (!m_pNaviMesh && m_pNaviMesh->IsValidIndex(m_iTriangleIndex, m_iVertexIndex))
		return;

	m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, _vPos);
	GetTransform()->SetPos(_vPos);
}

_vec3 CNaviMeshVtxMover::GetVertexPos() const
{
	return m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);
}

void CNaviMeshVtxMover::ReleaseVertexInfo()
{
	m_pNaviMesh = nullptr;
	m_iTriangleIndex = -1;
	m_iVertexIndex = -1;
}

PLANE::E_TYPE CNaviMeshVtxMover::GetPlaneTypeByRay(Engine::PICKINGRAYINFO & _rRayInfo)
{
	PLANE::E_TYPE ePlaneType = PLANE::TYPE_END;
	_float fU, fV, fDist;
	_vec3 vMinHitPos;
	_vec3 vTempHitPos;
	_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
	_matrix matWorld = GetTransform()->GetObjectMatrix();

	// 1) XY ���
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// �ﰢ�� ���� ���� ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XYTriLocal[i][j], &matWorld);
		}
		// �浹�ߴ����� üũ�Ѵ�.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

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
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// �浹�ߴٸ�, �浹 ��� Ÿ�԰� ���� �浹 ������ �����Ѵ�.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// ���� ��Ʈ ������ �Ÿ��� ���Ͽ� ������ �ش� ��� Ÿ������ �����Ѵ�.
			if (D3DXVec3LengthSq(&(vMinHitPos - _rRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - _rRayInfo.vRayPos))) {
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
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// �浹�ߴٸ�, �浹 ��� Ÿ�԰� ���� �浹 ������ �����Ѵ�.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// ���� ��Ʈ ������ �Ÿ��� ���Ͽ� ������ �ش� ��� Ÿ������ �����Ѵ�.
			if (D3DXVec3LengthSq(&(vMinHitPos - _rRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - _rRayInfo.vRayPos))) {
				ePlaneType = PLANE::TYPE_YZ;
				break;
			}
		}
	}

	return ePlaneType;
}

void CNaviMeshVtxMover::DragVertex()
{
	switch (m_ePlaneType) {
	case PLANE::TYPE_XY:
		// TODO : XY ��鿡 ���� �̵� ����
	{
		// XY ����� ���Ѵ�.
		D3DXPLANE planeXY;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// ���� �ﰢ�� �������� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XYTriLocal[0][i], &matWorld);
		}

		// ���� XY ����� ���Ѵ�
		D3DXPlaneFromPoints(&planeXY, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// ������ ���Ѵ�.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// ������ ����� �浹�ߴ��� Ȯ���Ѵ�.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXY, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// �浹 �������� �ﰢ�� ������ ��Ʈ�ѷ��� �̵���Ų��.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// �׷����� �����Ǿ��ִٸ�, �׷� �̵��� �����Ѵ�.
				MoveGroup();
			}
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_XZ:
		// TODO : XZ ��鿡 ���� �̵� ����
	{
		D3DXPLANE planeXZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// ���� �ﰢ�� �������� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XZTriLocal[0][i], &matWorld);
		}

		// ���� XY ����� ���Ѵ�
		D3DXPlaneFromPoints(&planeXZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// ������ ���Ѵ�.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// ������ ����� �浹�ߴ��� Ȯ���Ѵ�.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// �浹 �������� �ﰢ�� ������ ��Ʈ�ѷ��� �̵���Ų��.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// �׷����� �����Ǿ��ִٸ�, �׷� �̵��� �����Ѵ�.
				MoveGroup();
			}
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_YZ:
		// TODO : YZ ��鿡 ���� �̵� ����
	{
		D3DXPLANE planeYZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// ���� �ﰢ�� �������� ���� ��ǥ�� ��ȯ�Ѵ�.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &YZTriLocal[0][i], &matWorld);
		}

		// ���� XY ����� ���Ѵ�
		D3DXPlaneFromPoints(&planeYZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// ������ ���Ѵ�.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// ������ ����� �浹�ߴ��� Ȯ���Ѵ�.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeYZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// �浹 �������� �ﰢ�� ������ ��Ʈ�ѷ��� �̵���Ų��.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// �׷����� �����Ǿ��ִٸ�, �׷� �̵��� �����Ѵ�.
				MoveGroup();
			}
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	default:
		m_ePlaneType = PLANE::TYPE_END;
		m_bIsPicking = false;
		break;
	}
}

void CNaviMeshVtxMover::FormGroup()
{
	// ���� �׷��� �����Ѵ�.
	ReleaseGroup();

	auto& rVertices = m_pNaviMesh->GetNaviVertices();

	_int iVerticesSize = static_cast<_int>(rVertices.size());
	_int iTriangleSize = iVerticesSize / 3;
	_vec3 vVtxPos = m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);
	_vec3 vExtractedVtxPos;

	for (_int i = 0; i < iTriangleSize; ++i) {
		// ���� �ﰢ�� �� ������ ������� �ʴ´�.
		if (i == m_iTriangleIndex) continue;

		for (_int j = 0; j < 3; ++j) {
			vExtractedVtxPos = m_pNaviMesh->GetTriangleVertexPosition(i, j);
			if (D3DXVec3Length(&(vVtxPos - vExtractedVtxPos)) < m_fGroupRange) {
				// �׷� ����Ʈ�� �߰�
				m_vecGroupList.emplace_back(make_pair(i, j));
				break;
			}
		}
	}
}

void CNaviMeshVtxMover::ReleaseGroup()
{
	m_vecGroupList.clear();
}

void CNaviMeshVtxMover::MoveGroup()
{
	_int iGroupSize = static_cast<_int>(m_vecGroupList.size());
	_vec3 vVtxPos = m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);

	for (_int i = 0; i < iGroupSize; ++i) {
		m_pNaviMesh->SetTriangleVertexPosition(m_vecGroupList[i].first, m_vecGroupList[i].second, vVtxPos);
	}
}
