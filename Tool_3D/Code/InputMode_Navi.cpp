#include "pch.h"
#include "InputMode_Navi.h"
#include "EditScene.h"
#include "Terrain.h"
#include "NaviMesh.h"
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
		for (auto& pIndex : pTerrain->GetComponent<Engine::CTerrainTex>()->GetIndexes()) {
			vV1 = pVertices[pIndex._0];
			vV2 = pVertices[pIndex._1];
			vV3 = pVertices[pIndex._2];
			if (D3DXIntersectTri(&vV1, &vV2, &vV3, &stPickingRayInfo.vRayPos, &stPickingRayInfo.vRayDir, &fU, &fV, &fDist)) {
				// 픽킹이 성공했다면, 픽킹 히트 지점을 플레이어 이동 지점으로 세팅
				//m_pPlayer->SetTartgetPos(stPickingRayInfo.vRayPos + stPickingRayInfo.vRayDir * fDist);
				m_vecTriangle.push_back(Engine::GetHitPos(vV1, vV2, vV3, fU, fV));

				if (m_vecTriangle.size() == 3) {
					m_pEditScene->GetNaviMesh()->PushTriangleVertices(m_vecTriangle[0], m_vecTriangle[1], m_vecTriangle[2]);
					m_vecTriangle.clear();

					CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
					CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
					pTabForm->UpdateNaviTree(m_pEditScene->GetNaviMesh());
				}
				break;
			}
		}
	}
	return _int();
}
