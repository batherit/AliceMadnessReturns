#include "pch.h"
#include "NaviMeshVtxCtrl.h"
#include "NaviMesh.h"
#include "CNaviMeshTab.h"
#include "MainFrm.h"
#include "CTabForm.h"

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
	
	GetTransform()->SetScale(_vec3(10.f, 10.f, 10.f));

	m_vecGroupList.reserve(100);
	return S_OK;
}

int CNaviMeshVtxCtrl::Update_Object(const _float & _fDeltaTime)
{
	if (!m_bIsActivated && m_ePickMode != NAVIMESH_TAB::PICKMODE_TRIANGLE)
		return 0;

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// �浹�� ����� ã�´�.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);
			if (m_ePickMode == NAVIMESH_TAB::PICKMODE_VERTEX && m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
			else if(m_ePickMode == NAVIMESH_TAB::PICKMODE_VERTEX){
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
					
					if (!vecCollidedSphereIndex.empty()) {
						// �浹 ������ ã�� ���,
						_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
						_int iVertexIndex = vecCollidedSphereIndex[0] % 3;
						SetVertexInfo(m_pNaviMesh, iTriangleIndex, iVertexIndex);
						CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
						CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
						auto pNaviMeshTab = pTabForm->GetNaviMeshTab();
						pNaviMeshTab->m_chkGroup.EnableWindow(TRUE);

						if (pNaviMeshTab->m_btnCombine.IsWindowVisible()) {
							// �ʱ� ���¿��� ������ �����Ͽ� �Ĺ��� ��ư�� Ȱ��ȭ�� ���·� ��ȯ
							pNaviMeshTab->m_pairLastPickedVertex.first = iTriangleIndex;
							pNaviMeshTab->m_pairLastPickedVertex.second = iVertexIndex;
							pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
						}
						else if (pNaviMeshTab->m_btnCancel.IsWindowVisible()) {
							if (pNaviMeshTab->m_pairLastPickedVertex.first != iTriangleIndex) {
								// �Ĺ����� �����Ѵ�.
								// �ٸ� ������ �ﰢ���̸� �Ĺ����� �����Ѵ�.
								//_vec3 vSrcPos = m_pNaviMesh->GetTriangleVertexPosition(pNaviMeshTab->m_pairLastPickedVertex.first, pNaviMeshTab->m_pairLastPickedVertex.second);
								//m_pNaviMesh->SetTriangleVertexPosition(iTriangleIndex, iVertexIndex, vSrcPos);
								_vec3 vDestPos = m_pNaviMesh->GetTriangleVertexPosition(iTriangleIndex, iVertexIndex);
								m_pNaviMesh->SetTriangleVertexPosition(pNaviMeshTab->m_pairLastPickedVertex.first, pNaviMeshTab->m_pairLastPickedVertex.second, vDestPos);
								
								// ���� ��Ʈ�ѷ� ��ġ �簻��
								SetVertexInfo(m_pNaviMesh, iTriangleIndex, iVertexIndex);
								MoveGroup();
							}

							// �ʱ� ���·� ���ư���.
							pNaviMeshTab->m_btnCombine.EnableWindow(FALSE);
							pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
							pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
							pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);
						}
					}
				}
			}
			else if (m_ePickMode == NAVIMESH_TAB::PICKMODE_TRIANGLE) {
				// TODO : �浹 �ﰢ���� ã�´�.
				vector<_int> vecCollidedTriangleIndex;
				vecCollidedTriangleIndex.reserve(1);

				auto& vecNaviVertices = m_pNaviMesh->GetNaviVertices();
				_int iVecSize = vecNaviVertices.size();
				auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

				_vec3 vV1, vV2, vV3;
				_float fU, fV, fDist;
				for (_int i = 0; i < iVecSize / 3; ++i) {
					vV1 = vecNaviVertices[3 * i];
					vV2 = vecNaviVertices[3 * i + 1];
					vV3 = vecNaviVertices[3 * i + 2];
					if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
						// �浹�ߴٸ�, �ش� �ε����� �ﰢ���� Ȱ��ȭ��Ų��.
						m_pNaviMesh->MarkTriangle(i);

						CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
						CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
						auto pNaviMeshTab = pTabForm->GetNaviMeshTab();

						pNaviMeshTab->m_btnDelete.EnableWindow(TRUE);
						// Position Edit Ctrl ��Ȱ��ȭ
						pNaviMeshTab->m_editPosX.EnableWindow(FALSE);
						pNaviMeshTab->m_editPosY.EnableWindow(FALSE);
						pNaviMeshTab->m_editPosZ.EnableWindow(FALSE);
						pNaviMeshTab->m_chkGroup.EnableWindow(FALSE);
						pNaviMeshTab->m_editGroupRange.EnableWindow(FALSE);

						//pNaviMeshVtxCtrl->SetPickMode(NAVIMESH_TAB::PICKMODE_TRIANGLE);
						pNaviMeshTab->m_rbtnNavi.SetCheck(FALSE);
						pNaviMeshTab->m_rbtnVertex.SetCheck(FALSE);
						pNaviMeshTab->m_rbtnTriangle.SetCheck(TRUE);

						// �ʱ� ���·� ���ư���.
						pNaviMeshTab->m_btnCombine.EnableWindow(FALSE);
						pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
						pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
						pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);
						break;
					}
				}
			}
		}
	}
	else if (m_bIsPicking) {
		if(Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
			DragVertex();
		}
		else {
			// �巡�װ� �����ٸ�, ��ŷ ���°� �ƴ� ���̴�.
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}

	if(m_ePickMode != NAVIMESH_TAB::PICKMODE_TRIANGLE)
		m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CNaviMeshVtxCtrl::Render_Object(void)
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

_vec3 CNaviMeshVtxCtrl::GetVertexPos() const
{
	return m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);
}

void CNaviMeshVtxCtrl::SetGrouping(_bool _bIsGrouping)
{
	m_bIsGrouping = _bIsGrouping;
	
	if (!m_pNaviMesh)
		return;

	if (m_bIsGrouping)
		FormGroup();
	else
		ReleaseGroup();
}

void CNaviMeshVtxCtrl::SetPickMode(NAVIMESH_TAB::E_PICKMODE _ePickMode)
{
	m_ePickMode = _ePickMode;

	switch (_ePickMode) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		SetActive(false);
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:
		if (m_pNaviMesh && !m_pNaviMesh->GetNaviVertices().empty()) {
			if (!m_pNaviMesh->IsValidIndex(m_iTriangleIndex, m_iVertexIndex)) {
				m_iTriangleIndex = 0;
				m_iVertexIndex = 0;
				GetTransform()->SetPos(m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex));
			}
			SetActive(true);
		}
		else {
			SetActive(false);
		}
		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		SetActive(false);
		break;
	}
}

void CNaviMeshVtxCtrl::ProcessPickInput()
{
	switch (m_ePickMode) {
	case NAVIMESH_TAB::PICKMODE_NAVI:
		// InputMode�� ó���ϰ� �ֽ��ϴ�.
		break;
	case NAVIMESH_TAB::PICKMODE_VERTEX:

		break;
	case NAVIMESH_TAB::PICKMODE_TRIANGLE:
		break;
	}
}

void CNaviMeshVtxCtrl::SetNaviMesh(CNaviMesh * _pNaviMesh)
{
	if (!_pNaviMesh)
		return;

	m_pNaviMesh = _pNaviMesh;
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

	if (m_bIsGrouping)
		FormGroup();
}

void CNaviMeshVtxCtrl::ReleaseVertexInfo()
{
	m_pNaviMesh = nullptr;
	m_iTriangleIndex = -1;
	m_iVertexIndex = -1;
}

PLANE::E_TYPE CNaviMeshVtxCtrl::GetPlaneTypeByRay(Engine::PICKINGRAYINFO & _rRayInfo)
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

void CNaviMeshVtxCtrl::DragVertex()
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
			pTabForm->GetNaviMeshTab()->UpdateVertexPos(vHitPos);

			if (m_bIsGrouping) {
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
			pTabForm->GetNaviMeshTab()->UpdateVertexPos(vHitPos);

			if (m_bIsGrouping) {
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
			pTabForm->GetNaviMeshTab()->UpdateVertexPos(vHitPos);

			if (m_bIsGrouping) {
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

void CNaviMeshVtxCtrl::FormGroup()
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

void CNaviMeshVtxCtrl::ReleaseGroup()
{
	m_vecGroupList.clear();
}

void CNaviMeshVtxCtrl::MoveGroup()
{
	_int iGroupSize = static_cast<_int>(m_vecGroupList.size());
	_vec3 vVtxPos = m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);

	for (_int i = 0; i < iGroupSize; ++i) {
		m_pNaviMesh->SetTriangleVertexPosition(m_vecGroupList[i].first, m_vecGroupList[i].second, vVtxPos);
	}
}
