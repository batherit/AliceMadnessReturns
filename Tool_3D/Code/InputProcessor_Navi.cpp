#include "pch.h"
#include "InputProcessor_Navi.h"
#include "EditScene.h"
#include "Terrain.h"
#include "NaviMesh.h"
//#include "NaviMeshVtxCtrl.h"
#include "NaviMeshVtxMover.h"
#include "NaviMeshInputProcessor.h"
#include "CNaviMeshTab.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CNaviMeshTab.h"


CInputProcessor_Navi::CInputProcessor_Navi(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
	// 에디트씬 포인터를 얻어온다.
	m_pEditScene = m_pEditScene = g_pTool3D_Kernel->GetEditScene();

	// 네비메쉬탭 포인터를 얻어온다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pNaviMeshTab = pTabForm->GetNaviMeshTab();

	// 네비메쉬 생성을 위한 히트포지션을 저장할 벡터 공간을 예약한다.
	m_vecTriangle.reserve(3);
}

CInputProcessor_Navi::~CInputProcessor_Navi(void)
{
	m_vecTriangle.clear();
	m_vecTriangle.shrink_to_fit();
}

_int CInputProcessor_Navi::ProcessInput(const _float & _fDeltaTime)
{
	// 화면 밖을 클릭한 경우
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : 뷰 픽킹에 대한 처리를 진행합니다.
		switch (GetPickModeFromTab()) {
		case NAVIMESH_TAB::PICKMODE_NAVI:
			PickTerrain();
			break;
		case NAVIMESH_TAB::PICKMODE_VERTEX:
			PickVertex();
			break;
		case NAVIMESH_TAB::PICKMODE_TRIANGLE:
			PickTriangle();
			break;
		}
	}

	/*
	else if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
		// TODO : 드래그 상태에 대해서 처리합니다.
		switch (GetPickModeFromTab()) {
		case NAVIMESH_TAB::PICKMODE_NAVI:
			// 네비 모드에선 드래그로 처리되는 것은 없습니다.
			break;
		case NAVIMESH_TAB::PICKMODE_VERTEX:
			// 정점 드래그는 네비메쉬정점이동자가 처리하므로 처리되는 것은 없습니다.
			break;
		case NAVIMESH_TAB::PICKMODE_TRIANGLE:
			// 삼각형 모드에선 드래그로 처리되는 것은 없습니다.
			break;
		}
	}
	*/

	return 0;
}

NAVIMESH_TAB::E_PICKMODE CInputProcessor_Navi::GetPickModeFromTab() const
{
	if (m_pNaviMeshTab->m_rbtnNavi.GetCheck())
		return NAVIMESH_TAB::PICKMODE_NAVI;
	else if (m_pNaviMeshTab->m_rbtnVertex.GetCheck())
		return NAVIMESH_TAB::PICKMODE_VERTEX;
	else if (m_pNaviMeshTab->m_rbtnTriangle.GetCheck())
		return NAVIMESH_TAB::PICKMODE_TRIANGLE;

	return NAVIMESH_TAB::PICKMODE_END;
}

void CInputProcessor_Navi::PickTerrain()
{
	// 픽킹을 하기 위한 기본 변수들 세팅.
	CTerrain* pTerrain = m_pEditScene->GetTerrain();
	auto& pVertices = pTerrain->GetComponent<Engine::CTerrainTex>()->GetVertices();
	auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
	_float fU, fV, fDist;
	_vec3 vV1, vV2, vV3;

	// 픽킹 검사를 진행한다.
	_matrix matTerrainWorld = pTerrain->GetTransform()->GetObjectMatrix();
	_bool bIsNearestPosFinded = false;
	_vec3 vCross;
	_float fMinLengthSq = FLT_MAX;
	_float fLengthSq = FLT_MAX;
	_vec3 vHitPos;
	_vec3 vNearestPos;
	for (auto& pIndex : pTerrain->GetComponent<Engine::CTerrainTex>()->GetIndexes()) {
		vV1 = pVertices[pIndex._0];	/*From Local To World*/ D3DXVec3TransformCoord(&vV1, &vV1, &matTerrainWorld);
		vV2 = pVertices[pIndex._1];	/*From Local To World*/ D3DXVec3TransformCoord(&vV2, &vV2, &matTerrainWorld);
		vV3 = pVertices[pIndex._2];	/*From Local To World*/ D3DXVec3TransformCoord(&vV3, &vV3, &matTerrainWorld);

		vCross = Engine::GetCross(vV1, vV2, vV3);
		if (!Engine::IsFacing(vCross, stPickingRayInfo.vRayDir)) {
			// 마주보고 있는 삼각형이 아니라면, 건너뛴다.
			continue;
		}

		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
			// 충돌했다면, 충돌 지점을 찾는다.
			vHitPos = Engine::GetHitPos(vV1, vV2, vV3, fU, fV);
			fLengthSq = D3DXVec3LengthSq(&(vHitPos - stPickingRayInfo.vRayPos));

			// 가장 가까운 점을 찾지 못한 상황이라면, 초기 세팅을 해준다.
			if (!bIsNearestPosFinded) {
				vNearestPos = vHitPos;
				fMinLengthSq = fLengthSq;
				bIsNearestPosFinded = true;
			}
			else {
				// 가장 가까운 점을 찾으면 변수들을 새로이 갱신한다.
				if (fMinLengthSq > fLengthSq) {
					vNearestPos = vHitPos;
					fMinLengthSq = fLengthSq;
				}
			}
		}
	}

	if (bIsNearestPosFinded) {
		// 가장 가까운 점을 찾았다면, 이것을 저장해둔다.

		// 네비 마그넷 기능이 켜져있다면 vNearestPos를 보정한다.
		if (m_pNaviMeshTab->m_bIsNaviMagnet) {
			auto& rNaviVertices = m_pEditScene->GetNaviMesh()->GetNaviVertices();

			for (auto& rPos : rNaviVertices) {
				if (Engine::IsPointAndSphereCollided(vNearestPos, 5.f, rPos)) {
					vNearestPos = rPos;
					break;
				}
			}
		}

		m_vecTriangle.push_back(vNearestPos);

		if (m_vecTriangle.size() == 3) {
			// 네비 메쉬 갱신
			m_pEditScene->GetNaviMesh()->PushTriangleVertices(m_vecTriangle[0], m_vecTriangle[1], m_vecTriangle[2]);
			m_vecTriangle.clear();

			// 네비 트리 갱신(UI 동기화)
			m_pNaviMeshTab->UpdateNaviTree(m_pEditScene->GetNaviMesh());
		}
	}
}

void CInputProcessor_Navi::PickVertex()
{
	// 컴바인 버튼을 누르지 않은 상태에서는 네비메쉬정점이동자에 입력처리를 맡깁니다.
	// 하지만 컴바인 상태가 아닌데도 네비메쉬정점이동자가 활성화되어있지 않다면,
	// 광선과 충돌한 정점을 찾아 그것을 이동자에 세팅하고 활성화시킵니다.
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	if (!pNaviMeshVtxMover->IsMoverGizmoActivated() && !m_pNaviMeshTab->m_btnCancel.IsWindowEnabled()) {
		// 광선과 충돌한 정점을 찾습니다.
		vector<_int> vecCollidedSphereIndex;
		vecCollidedSphereIndex.reserve(1);

		CNaviMesh* pNaviMesh = m_pEditScene->GetNaviMesh();
		auto& vecNaviVertices = pNaviMesh->GetNaviVertices();
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
			_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
			_int iVertexIndex = vecCollidedSphereIndex[0] % 3;
			CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();

			pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
			pNaviMeshVtxMover->ActivateMoverGizmo(true);
			m_pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
		}
	}


	if(m_pNaviMeshTab->m_btnCancel.IsWindowEnabled()) {
		// 컴바인 버튼을 누른 상태
		
		pNaviMeshVtxMover->ActivateMoverGizmo(false);

		// 광선과 충돌한 정점을 찾습니다.
		vector<_int> vecCollidedSphereIndex;
		vecCollidedSphereIndex.reserve(1);

		CNaviMesh* pNaviMesh = m_pEditScene->GetNaviMesh();
		auto& vecNaviVertices = pNaviMesh->GetNaviVertices();
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
			// Position Edit Ctrl 활성화
			m_pNaviMeshTab->m_editPosX.EnableWindow(TRUE);
			m_pNaviMeshTab->m_editPosY.EnableWindow(TRUE);
			m_pNaviMeshTab->m_editPosZ.EnableWindow(TRUE);

			// 그룹 체크 박스 활성화
			m_pNaviMeshTab->m_chkGroup.EnableWindow(TRUE);


			// 삼각형 삭제 버튼 비활성화
			m_pNaviMeshTab->m_btnDelete.EnableWindow(FALSE);

			// 픽모드를 변경한다.
			ClearPickedVertices();
			m_pNaviMeshTab->m_rbtnNavi.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnVertex.SetCheck(TRUE);
			m_pNaviMeshTab->m_rbtnTriangle.SetCheck(FALSE);

			// 충돌 정점을 찾은 경우,
			_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
			_int iVertexIndex = vecCollidedSphereIndex[0] % 3;
			CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
			

			if (m_pNaviMeshTab->m_btnCombine.IsWindowVisible()) {
				// 초기 상태에서 정점을 선택하여 컴바인 버튼이 활성화된 상태로 전환
				pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
				pNaviMeshVtxMover->ActivateMoverGizmo(true);
				m_pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
			}
			else if (m_pNaviMeshTab->m_btnCancel.IsWindowVisible()) {
				if (pNaviMeshVtxMover->GetTriangleIndex() != iTriangleIndex) {
					// 컴바인을 수행한다.
					// 이동할 지점의 위치를 얻어온다.
					_vec3 vDestPos = pNaviMesh->GetTriangleVertexPosition(iTriangleIndex, iVertexIndex);
					pNaviMeshVtxMover->SetVertexPos(vDestPos);
					if (m_pNaviMeshTab->m_chkGroup.GetCheck())
						pNaviMeshVtxMover->MoveGroup();
					else
						pNaviMeshVtxMover->ReleaseGroup();
					// 이동을 완료하였다면, 클릭한 정점을 선택한다.
					pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
				}
				// 이동 기즈모를 활성화한다.
				pNaviMeshVtxMover->ActivateMoverGizmo(true);

				// 초기 상태로 돌아간다.
				m_pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
				m_pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
				m_pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
				m_pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);
			}

			if (m_pNaviMeshTab->m_chkGroup.GetCheck())
				pNaviMeshVtxMover->FormGroup();

			// UI 동기화
			m_pNaviMeshTab->UpdateVertexPos(pNaviMeshVtxMover->GetVertexPos());
		}
		// 충돌 정점을 찾지 못했다면 아무것도 하지 않습니다.
	}
}

void CInputProcessor_Navi::ClearPickedVertices()
{
	m_vecTriangle.clear();
}

void CInputProcessor_Navi::PickTriangle()
{
	// 충돌 삼각형을 찾는다.
	vector<_int> vecCollidedTriangleIndex;
	vecCollidedTriangleIndex.reserve(1);

	CNaviMesh* pNaviMesh = m_pEditScene->GetNaviMesh();
	auto& vecNaviVertices = pNaviMesh->GetNaviVertices();
	_int iVecSize = vecNaviVertices.size();

	// 광선을 구한다.
	auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

	_vec3 vV1, vV2, vV3;
	_float fU, fV, fDist;
	for (_int i = 0; i < iVecSize / 3; ++i) {
		vV1 = vecNaviVertices[3 * i];
		vV2 = vecNaviVertices[3 * i + 1];
		vV3 = vecNaviVertices[3 * i + 2];
		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
			// 충돌했다면, 해당 인덱스의 삼각형을 활성화시킨다.
			pNaviMesh->MarkTriangle(i);

			
			// Position Edit Ctrl 비활성화
			m_pNaviMeshTab->m_editPosX.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editPosY.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editPosZ.EnableWindow(FALSE);
			m_pNaviMeshTab->m_chkGroup.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editGroupRange.EnableWindow(FALSE);

			// 삼각형 삭제 버튼을 활성화한다.
			m_pNaviMeshTab->m_btnDelete.EnableWindow(TRUE);

			// 삼각형 픽모드로 세팅한다.
			m_pNaviMeshTab->m_rbtnNavi.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnVertex.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnTriangle.SetCheck(TRUE);

			// 초기 상태로 돌아간다.
			m_pNaviMeshTab->m_btnCombine.EnableWindow(FALSE);
			m_pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
			m_pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
			m_pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);
			break;
		}
	}
}

void CInputProcessor_Navi::DragVertex()
{
	// 네비메쉬정점이동자가 이 함수의 기능을 대체합니다.
}
