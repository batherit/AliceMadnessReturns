#include "pch.h"
#include "InputProcessor_Map.h"
#include "InputProcessor_Navi.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CMapTab.h"
#include "EditScene.h"
#include "StaticObject.h"
#include "Gizmo.h"

CInputProcessor_Map::CInputProcessor_Map(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
	// ����Ʈ�� �����͸� ���´�.
	m_pEditScene = m_pEditScene = g_pTool3D_Kernel->GetEditScene();

	// �� �� �����͸� ���´�.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pMapTab = pTabForm->GetMapTab();
}

CInputProcessor_Map::~CInputProcessor_Map(void)
{
	g_pTool3D_Kernel->GetEditScene()->GetGizmo()->ActivateGizmo(false);
}

_int CInputProcessor_Map::ProcessInput(const _float & _fDeltaTime)
{
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
		return -1;

	CGizmo* pGizmo = g_pTool3D_Kernel->GetEditScene()->GetGizmo();
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
		// ���� ������Ʈ�� �浹�ߴ��� Ȯ���Ѵ�.
		vector<_int> vecCollidedSphereIndex;
		vecCollidedSphereIndex.reserve(1);

		auto& rStaticObjectList = g_pTool3D_Kernel->GetEditScene()->GetStaticObjectList();
		_int iVecSize = rStaticObjectList.size();
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		for (_int i = 0; i < iVecSize; ++i) {
			if (Engine::IsRayAndSphereCollided(stPickingRayInfo, 2.f, rStaticObjectList[i]->GetTransform()->GetPos())) {
				if (vecCollidedSphereIndex.empty()) {
					vecCollidedSphereIndex.emplace_back(i);
				}
				else {
					if (D3DXVec3LengthSq(&(rStaticObjectList[vecCollidedSphereIndex[0]]->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))
		> D3DXVec3LengthSq(&(rStaticObjectList[i]->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))) {
						//vecNaviVertices[0] = vecNaviVertices[i];
						vecCollidedSphereIndex[0] = i;
					}
				}
			}
		}

		if (!pGizmo->IsGizmoActivated()) {
			if (!vecCollidedSphereIndex.empty()) {
				// �浹�� ���� ������Ʈ�� ã�� ���, ����� �ش� ������Ʈ�� �����ϰ� Ȱ��ȭ�Ѵ�.
				m_pMapTab->SetSelectedObject(rStaticObjectList[vecCollidedSphereIndex[0]]);
				pGizmo->SetObject(rStaticObjectList[vecCollidedSphereIndex[0]]);
				pGizmo->ActivateGizmo(true);
			}
		}


	}
	return 0;
}
