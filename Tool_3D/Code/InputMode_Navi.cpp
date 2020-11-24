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
	// 화면 밖을 클릭한 경우
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;
	m_pEditScene = g_pTool3D_Kernel->GetEditScene();
	// 에디트씬이 없을 경우
	if (!m_pEditScene)
		return -1;

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
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
			m_vecTriangle.push_back(vNearestPos);

			if (m_vecTriangle.size() == 3) {
				// 네비 메쉬 갱신
				m_pEditScene->GetNaviMesh()->PushTriangleVertices(m_vecTriangle[0], m_vecTriangle[1], m_vecTriangle[2]);
				m_vecTriangle.clear();

				// 네비 트리 갱신
				CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
				CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
				pTabForm->GetNaviMeshTab()->UpdateNaviTree(m_pEditScene->GetNaviMesh());
			}
		}
	}
	return _int();
}
