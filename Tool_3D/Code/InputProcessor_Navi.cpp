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
	// ����Ʈ�� �����͸� ���´�.
	m_pEditScene = m_pEditScene = g_pTool3D_Kernel->GetEditScene();

	// �׺�޽��� �����͸� ���´�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pNaviMeshTab = pTabForm->GetNaviMeshTab();

	// �׺�޽� ������ ���� ��Ʈ�������� ������ ���� ������ �����Ѵ�.
	m_vecTriangle.reserve(3);
}

CInputProcessor_Navi::~CInputProcessor_Navi(void)
{
	m_vecTriangle.clear();
	m_vecTriangle.shrink_to_fit();
}

_int CInputProcessor_Navi::ProcessInput(const _float & _fDeltaTime)
{
	// ȭ�� ���� Ŭ���� ���
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// TODO : �� ��ŷ�� ���� ó���� �����մϴ�.
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
		// TODO : �巡�� ���¿� ���ؼ� ó���մϴ�.
		switch (GetPickModeFromTab()) {
		case NAVIMESH_TAB::PICKMODE_NAVI:
			// �׺� ��忡�� �巡�׷� ó���Ǵ� ���� �����ϴ�.
			break;
		case NAVIMESH_TAB::PICKMODE_VERTEX:
			// ���� �巡�״� �׺�޽������̵��ڰ� ó���ϹǷ� ó���Ǵ� ���� �����ϴ�.
			break;
		case NAVIMESH_TAB::PICKMODE_TRIANGLE:
			// �ﰢ�� ��忡�� �巡�׷� ó���Ǵ� ���� �����ϴ�.
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
	// ��ŷ�� �ϱ� ���� �⺻ ������ ����.
	CTerrain* pTerrain = m_pEditScene->GetTerrain();
	auto& pVertices = pTerrain->GetComponent<Engine::CTerrainTex>()->GetVertices();
	auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
	_float fU, fV, fDist;
	_vec3 vV1, vV2, vV3;

	// ��ŷ �˻縦 �����Ѵ�.
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
			// ���ֺ��� �ִ� �ﰢ���� �ƴ϶��, �ǳʶڴ�.
			continue;
		}

		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
			// �浹�ߴٸ�, �浹 ������ ã�´�.
			vHitPos = Engine::GetHitPos(vV1, vV2, vV3, fU, fV);
			fLengthSq = D3DXVec3LengthSq(&(vHitPos - stPickingRayInfo.vRayPos));

			// ���� ����� ���� ã�� ���� ��Ȳ�̶��, �ʱ� ������ ���ش�.
			if (!bIsNearestPosFinded) {
				vNearestPos = vHitPos;
				fMinLengthSq = fLengthSq;
				bIsNearestPosFinded = true;
			}
			else {
				// ���� ����� ���� ã���� �������� ������ �����Ѵ�.
				if (fMinLengthSq > fLengthSq) {
					vNearestPos = vHitPos;
					fMinLengthSq = fLengthSq;
				}
			}
		}
	}

	if (bIsNearestPosFinded) {
		// ���� ����� ���� ã�Ҵٸ�, �̰��� �����صд�.

		// �׺� ���׳� ����� �����ִٸ� vNearestPos�� �����Ѵ�.
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
			// �׺� �޽� ����
			m_pEditScene->GetNaviMesh()->PushTriangleVertices(m_vecTriangle[0], m_vecTriangle[1], m_vecTriangle[2]);
			m_vecTriangle.clear();

			// �׺� Ʈ�� ����(UI ����ȭ)
			m_pNaviMeshTab->UpdateNaviTree(m_pEditScene->GetNaviMesh());
		}
	}
}

void CInputProcessor_Navi::PickVertex()
{
	// �Ĺ��� ��ư�� ������ ���� ���¿����� �׺�޽������̵��ڿ� �Է�ó���� �ñ�ϴ�.
	// ������ �Ĺ��� ���°� �ƴѵ��� �׺�޽������̵��ڰ� Ȱ��ȭ�Ǿ����� �ʴٸ�,
	// ������ �浹�� ������ ã�� �װ��� �̵��ڿ� �����ϰ� Ȱ��ȭ��ŵ�ϴ�.
	CNaviMeshVtxMover* pNaviMeshVtxMover = g_pTool3D_Kernel->GetEditScene()->GetNaviMeshVtxMover();

	if (!pNaviMeshVtxMover->IsMoverGizmoActivated() && !m_pNaviMeshTab->m_btnCancel.IsWindowEnabled()) {
		// ������ �浹�� ������ ã���ϴ�.
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
		// �Ĺ��� ��ư�� ���� ����
		
		pNaviMeshVtxMover->ActivateMoverGizmo(false);

		// ������ �浹�� ������ ã���ϴ�.
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
			// Position Edit Ctrl Ȱ��ȭ
			m_pNaviMeshTab->m_editPosX.EnableWindow(TRUE);
			m_pNaviMeshTab->m_editPosY.EnableWindow(TRUE);
			m_pNaviMeshTab->m_editPosZ.EnableWindow(TRUE);

			// �׷� üũ �ڽ� Ȱ��ȭ
			m_pNaviMeshTab->m_chkGroup.EnableWindow(TRUE);


			// �ﰢ�� ���� ��ư ��Ȱ��ȭ
			m_pNaviMeshTab->m_btnDelete.EnableWindow(FALSE);

			// �ȸ�带 �����Ѵ�.
			ClearPickedVertices();
			m_pNaviMeshTab->m_rbtnNavi.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnVertex.SetCheck(TRUE);
			m_pNaviMeshTab->m_rbtnTriangle.SetCheck(FALSE);

			// �浹 ������ ã�� ���,
			_int iTriangleIndex = vecCollidedSphereIndex[0] / 3;
			_int iVertexIndex = vecCollidedSphereIndex[0] % 3;
			CNaviMesh* pNaviMesh = g_pTool3D_Kernel->GetEditScene()->GetNaviMesh();
			

			if (m_pNaviMeshTab->m_btnCombine.IsWindowVisible()) {
				// �ʱ� ���¿��� ������ �����Ͽ� �Ĺ��� ��ư�� Ȱ��ȭ�� ���·� ��ȯ
				pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
				pNaviMeshVtxMover->ActivateMoverGizmo(true);
				m_pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
			}
			else if (m_pNaviMeshTab->m_btnCancel.IsWindowVisible()) {
				if (pNaviMeshVtxMover->GetTriangleIndex() != iTriangleIndex) {
					// �Ĺ����� �����Ѵ�.
					// �̵��� ������ ��ġ�� ���´�.
					_vec3 vDestPos = pNaviMesh->GetTriangleVertexPosition(iTriangleIndex, iVertexIndex);
					pNaviMeshVtxMover->SetVertexPos(vDestPos);
					if (m_pNaviMeshTab->m_chkGroup.GetCheck())
						pNaviMeshVtxMover->MoveGroup();
					else
						pNaviMeshVtxMover->ReleaseGroup();
					// �̵��� �Ϸ��Ͽ��ٸ�, Ŭ���� ������ �����Ѵ�.
					pNaviMeshVtxMover->SelectVertex(pNaviMesh, iTriangleIndex, iVertexIndex);
				}
				// �̵� ����� Ȱ��ȭ�Ѵ�.
				pNaviMeshVtxMover->ActivateMoverGizmo(true);

				// �ʱ� ���·� ���ư���.
				m_pNaviMeshTab->m_btnCombine.EnableWindow(TRUE);
				m_pNaviMeshTab->m_btnCombine.ShowWindow(TRUE);
				m_pNaviMeshTab->m_btnCancel.EnableWindow(FALSE);
				m_pNaviMeshTab->m_btnCancel.ShowWindow(FALSE);
			}

			if (m_pNaviMeshTab->m_chkGroup.GetCheck())
				pNaviMeshVtxMover->FormGroup();

			// UI ����ȭ
			m_pNaviMeshTab->UpdateVertexPos(pNaviMeshVtxMover->GetVertexPos());
		}
		// �浹 ������ ã�� ���ߴٸ� �ƹ��͵� ���� �ʽ��ϴ�.
	}
}

void CInputProcessor_Navi::ClearPickedVertices()
{
	m_vecTriangle.clear();
}

void CInputProcessor_Navi::PickTriangle()
{
	// �浹 �ﰢ���� ã�´�.
	vector<_int> vecCollidedTriangleIndex;
	vecCollidedTriangleIndex.reserve(1);

	CNaviMesh* pNaviMesh = m_pEditScene->GetNaviMesh();
	auto& vecNaviVertices = pNaviMesh->GetNaviVertices();
	_int iVecSize = vecNaviVertices.size();

	// ������ ���Ѵ�.
	auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

	_vec3 vV1, vV2, vV3;
	_float fU, fV, fDist;
	for (_int i = 0; i < iVecSize / 3; ++i) {
		vV1 = vecNaviVertices[3 * i];
		vV2 = vecNaviVertices[3 * i + 1];
		vV3 = vecNaviVertices[3 * i + 2];
		if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
			// �浹�ߴٸ�, �ش� �ε����� �ﰢ���� Ȱ��ȭ��Ų��.
			pNaviMesh->MarkTriangle(i);

			
			// Position Edit Ctrl ��Ȱ��ȭ
			m_pNaviMeshTab->m_editPosX.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editPosY.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editPosZ.EnableWindow(FALSE);
			m_pNaviMeshTab->m_chkGroup.EnableWindow(FALSE);
			m_pNaviMeshTab->m_editGroupRange.EnableWindow(FALSE);

			// �ﰢ�� ���� ��ư�� Ȱ��ȭ�Ѵ�.
			m_pNaviMeshTab->m_btnDelete.EnableWindow(TRUE);

			// �ﰢ�� �ȸ��� �����Ѵ�.
			m_pNaviMeshTab->m_rbtnNavi.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnVertex.SetCheck(FALSE);
			m_pNaviMeshTab->m_rbtnTriangle.SetCheck(TRUE);

			// �ʱ� ���·� ���ư���.
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
	// �׺�޽������̵��ڰ� �� �Լ��� ����� ��ü�մϴ�.
}
