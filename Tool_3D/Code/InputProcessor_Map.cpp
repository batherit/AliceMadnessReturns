#include "pch.h"
#include "InputProcessor_Map.h"
#include "InputProcessor_Navi.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CMapTab.h"
#include "EditScene.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Gizmo.h"

CInputProcessor_Map::CInputProcessor_Map(Engine::CInputProcessorMgr * _pInputProcessorMgr)
	:
	CInputProcessor(_pInputProcessorMgr)
{
	// 에디트씬 포인터를 얻어온다.
	m_pEditScene = m_pEditScene = g_pTool3D_Kernel->GetEditScene();

	// 맵 탭 포인터를 얻어온다.
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
		// 오브젝트와 충돌했는지 확인한다.
		//vector<_int> vecCollidedSphereIndex;
		//vecCollidedSphereIndex.reserve(1);

		_bool bIsStaticObject = true;
		Engine::CGameObject* pSelectedObject = nullptr;
		auto& rStaticObjectList = g_pTool3D_Kernel->GetEditScene()->GetStaticObjectList();
		auto& rDynamicObjectList = g_pTool3D_Kernel->GetEditScene()->GetDynamicObjectList();
		_int iVecSize = rStaticObjectList.size();
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		for (_int i = 0; i < iVecSize; ++i) {
			if (Engine::IsRayAndSphereCollided(stPickingRayInfo, 2.f, rStaticObjectList[i]->GetTransform()->GetPos())) {
				if (!pSelectedObject) {
					//vecCollidedSphereIndex.emplace_back(i);
					pSelectedObject = rStaticObjectList[i];
				}
				else {
					if (D3DXVec3LengthSq(&(pSelectedObject->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))
		> D3DXVec3LengthSq(&(rStaticObjectList[i]->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))) {
						//vecNaviVertices[0] = vecNaviVertices[i];
						bIsStaticObject = true;
						//vecCollidedSphereIndex[0] = i;
						pSelectedObject = rStaticObjectList[i];
					}
				}
			}
		}

		iVecSize = rDynamicObjectList.size();
		for (_int i = 0; i < iVecSize; ++i) {
			if (Engine::IsRayAndSphereCollided(stPickingRayInfo, 2.f, rDynamicObjectList[i]->GetTransform()->GetPos())) {
				if (!pSelectedObject) {
					//vecCollidedSphereIndex.emplace_back(i);
					pSelectedObject = rDynamicObjectList[i];
				}
				else {
					if (D3DXVec3LengthSq(&(pSelectedObject->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))
		> D3DXVec3LengthSq(&(rDynamicObjectList[i]->GetTransform()->GetPos() - stPickingRayInfo.vRayPos))) {
						//vecNaviVertices[0] = vecNaviVertices[i];
						bIsStaticObject = false;
						pSelectedObject = rDynamicObjectList[i];
						//vecCollidedSphereIndex[0] = i;
					}
				}
			}
		}

		if (!pGizmo->IsGizmoActivated()) {
			if (pSelectedObject) {
				if (bIsStaticObject){
					// 충돌한 정적 오브젝트를 찾은 경우, 기즈모에 해당 오브젝트를 세팅하고 활성화한다.
					m_pMapTab->SetSelectedObject(pSelectedObject);
					pGizmo->SetObject(pSelectedObject);
					pGizmo->ActivateGizmo(true);
				}
				else {
					m_pMapTab->SetSelectedObject(pSelectedObject);
					pGizmo->SetObject(pSelectedObject);
					pGizmo->ActivateGizmo(true);
				}
			}
		}


	}
	return 0;
}
