#include "pch.h"
#include "InputMode_Navi.h"
#include "EditScene.h"
#include "Terrain.h"
#include "NaviMesh.h"
#include "CNaviMeshTab.h"
#include "MainFrm.h"
#include "CTabForm.h"

CInputMode_Navi::CInputMode_Navi(Engine::CInputModeMgr * _pInputModeMgr)
	:
	CInputMode(_pInputModeMgr)
{
	m_vecTriangle.reserve(3);
	m_pEditScene = g_pTool3D_Kernel->GetEditScene();
}

CInputMode_Navi::~CInputMode_Navi(void)
{
	m_vecTriangle.clear();
	m_vecTriangle.shrink_to_fit();
}

_int CInputMode_Navi::ProcessInput(const _float & _fDeltaTime)
{
	// ȭ�� ���� Ŭ���� ���
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;
	m_pEditScene = g_pTool3D_Kernel->GetEditScene();
	// ����Ʈ���� ���� ���
	if (!m_pEditScene)
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
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
			m_vecTriangle.push_back(vNearestPos);

			if (m_vecTriangle.size() == 3) {
				// �׺� �޽� ����
				m_pEditScene->GetNaviMesh()->PushTriangleVertices(m_vecTriangle[0], m_vecTriangle[1], m_vecTriangle[2]);
				m_vecTriangle.clear();

				// �׺� Ʈ�� ����
				CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
				CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
				pTabForm->GetNaviMeshTab()->UpdateNaviTree(m_pEditScene->GetNaviMesh());
			}
		}
	}
	return _int();
}
