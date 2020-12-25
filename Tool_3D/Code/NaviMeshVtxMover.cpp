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

	// 컴바인 상태가 아닐때 업데이트됩니다.

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// 충돌된 평면을 찾는다.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);

			if (m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
			else {
				// 충돌된 평면을 찾지 못한 경우,
				// 충돌 정점을 찾는다.
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
									vecCollidedSphereIndex[0] = i;
								}
							}
						}
					}

					CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
					CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
					auto pNaviMeshTab = pTabForm->GetNaviMeshTab();
					

					if (!vecCollidedSphereIndex.empty()) {
						// 충돌 정점을 찾은 경우,
						_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
						_int iVertexIndex = vecCollidedSphereIndex[0] % 3;

						SelectVertex(m_pNaviMesh, iTriangleIndex, iVertexIndex);

						pNaviMeshTab->m_chkGroup.EnableWindow(TRUE);

						// 초기 상태로 돌아간다.
						pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
						pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
						pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
						pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);

						if (pNaviMeshTab->m_chkGroup.GetCheck())
							FormGroup();
					}

					// UI 동기화
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
			// 드래그가 끝났다면, 픽킹 상태가 아닌 것이다.
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_UI, this);
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

	// 축 그리기
	ID3DXLine *pLine;
	D3DXCreateLine(m_pGraphicDev, &pLine);
	pLine->SetWidth(2.f);
	pLine->SetAntialias(true);
	pLine->Begin();

	DWORD bIsZBufferEnabled;
	m_pGraphicDev->GetRenderState(D3DRS_ZENABLE, &bIsZBufferEnabled);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	for (_int i = 0; i < AXIS_NUM; ++i) {
		if (m_ePlaneType == PLANE::TYPE_XY && i == 2) continue;
		else if (m_ePlaneType == PLANE::TYPE_XZ && i == 1) continue;
		else if (m_ePlaneType == PLANE::TYPE_YZ && i == 0) continue;

		pLine->DrawTransform(m_vAxisVertices[i], 2, &(matWorld * matView * matProj), m_clAxisColor[i]);
	}
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, bIsZBufferEnabled);

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

	// 정점 위치에 이동자를 일치시킨다.(동기화)
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

	// 1) XY 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XYTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vMinHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);
			ePlaneType = PLANE::TYPE_XY;
			break;
		}
	}
	// 2) XZ 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &XZTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// 기존 히트 포스와 거리를 비교하여 가까우면 해당 평면 타입으로 갱신한다.
			if (D3DXVec3LengthSq(&(vMinHitPos - _rRayInfo.vRayPos)) > D3DXVec3LengthSq(&(vTempHitPos - _rRayInfo.vRayPos))) {
				vMinHitPos = vTempHitPos;
				ePlaneType = PLANE::TYPE_XZ;
				break;
			}
		}
	}
	// 3) YZ 평면
	for (_int i = 0; i < TRIANGLE_NUM; ++i) {
		// 삼각형 로컬 정점 좌표를 월드 좌표로 변환한다.
		for (_int j = 0; j < TRI_VERTEX_NUM; ++j) {
			D3DXVec3TransformCoord(&vWorldTriVtx[j], &YZTriLocal[i][j], &matWorld);
		}
		// 충돌했는지를 체크한다.
		if (D3DXIntersectTri(&vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2],
			&_rRayInfo.vRayPos, &_rRayInfo.vRayDir, &fU, &fV, &fDist)) {

			// 충돌했다면, 충돌 평면 타입과 광선 충돌 지점을 저장한다.
			vTempHitPos = Engine::GetHitPos(vWorldTriVtx[0], vWorldTriVtx[1], vWorldTriVtx[2], fU, fV);

			// 기존 히트 포스와 거리를 비교하여 가까우면 해당 평면 타입으로 갱신한다.
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
		// TODO : XY 평면에 대한 이동 갱신
	{
		// XY 평면을 구한다.
		D3DXPLANE planeXY;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XYTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeXY, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXY, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// 그루핑이 설정되어있다면, 그룹 이동을 진행한다.
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
		// TODO : XZ 평면에 대한 이동 갱신
	{
		D3DXPLANE planeXZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XZTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeXZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// 그루핑이 설정되어있다면, 그룹 이동을 진행한다.
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
		// TODO : YZ 평면에 대한 이동 갱신
	{
		D3DXPLANE planeYZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &YZTriLocal[0][i], &matWorld);
		}

		// 월드 XY 평면을 구한다
		D3DXPlaneFromPoints(&planeYZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeYZ, &fT)) {
			vHitPos = stPickingRayInfo.vRayPos + fT * stPickingRayInfo.vRayDir;
			// 충돌 지점으로 삼각형 정점과 컨트롤러를 이동시킨다.
			m_pNaviMesh->SetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex, vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);

			if (pNaviMeshTab->m_chkGroup) {
				// 그루핑이 설정되어있다면, 그룹 이동을 진행한다.
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
	// 기존 그룹을 해제한다.
	ReleaseGroup();

	if (!m_pNaviMesh)
		return;

	auto& rVertices = m_pNaviMesh->GetNaviVertices();
	_int iVerticesSize = static_cast<_int>(rVertices.size());
	_int iTriangleSize = iVerticesSize / 3;

	_vec3 vVtxPos = m_pNaviMesh->GetTriangleVertexPosition(m_iTriangleIndex, m_iVertexIndex);
	_vec3 vExtractedVtxPos;

	for (_int i = 0; i < iTriangleSize; ++i) {
		// 같은 삼각형 내 정점은 취급하지 않는다.
		if (i == m_iTriangleIndex) continue;

		for (_int j = 0; j < 3; ++j) {
			vExtractedVtxPos = m_pNaviMesh->GetTriangleVertexPosition(i, j);
			if (D3DXVec3Length(&(vVtxPos - vExtractedVtxPos)) < m_fGroupRange) {
				// 그룹 리스트에 추가
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
