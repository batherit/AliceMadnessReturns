#include "pch.h"
#include "NaviMeshVtxMover.h"
#include "NaviMesh.h"
#include "CNaviMeshTab.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CMapTab.h"
#include "Gizmo.h"
#include "StaticObject.h"
#include "EditScene.h"

CGizmo::CGizmo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CGizmo::CGizmo(const CGizmo & rhs)
	: CGameObject(rhs)
{
}

CGizmo::~CGizmo(void)
{
}

HRESULT CGizmo::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();

	GetTransform()->SetScale(_vec3(10.f, 10.f, 10.f));

	// 맵 탭 포인터를 얻어온다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pMapTab = pTabForm->GetMapTab();

	return S_OK;
}

int CGizmo::Update_Object(const _float & _fDeltaTime)
{
	if (!m_bIsGizmoActivated)
		return 0;

	// C버튼을 누르면 복제가 된다.
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_C)) {
		CString pMeshTag = dynamic_cast<CStaticObject*>(m_pGameObject)->GetMeshTag();
		if (m_pMapTab->AddStaticObject(pMeshTag)) {
			auto& rStaticObjectList = g_pTool3D_Kernel->GetEditScene()->GetStaticObjectList();
			CStaticObject* pNewStaticObject = rStaticObjectList[rStaticObjectList.size() - 1];
			_vec3 vPos = m_pGameObject->GetTransform()->GetPos();
			vPos.x += 4;
			_vec3 vAngle = m_pGameObject->GetTransform()->GetAngle();
			_vec3 vScale = m_pGameObject->GetTransform()->GetScale();
			pNewStaticObject->GetTransform()->SetPos(vPos);
			pNewStaticObject->GetTransform()->SetAngle(vAngle);
			pNewStaticObject->GetTransform()->SetScale(vScale);
			SetObject(pNewStaticObject);

			m_pMapTab->SetSelectedObject(pNewStaticObject);
			m_pMapTab->UpdatePos(vPos);
			m_pMapTab->UpdateAngle(vAngle);
			m_pMapTab->UpdateScale(vScale);
			return 0;
		}
	}

	// 컴바인 상태가 아닐때 업데이트됩니다.
	auto ptCurrentCursor = Engine::GetClientCursorPoint(g_hWnd);
	if (!Engine::IsPointInClient(g_hWnd, ptCurrentCursor))
	{
		m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
		return 0;
	}

	if (!m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			// 충돌된 평면을 찾는다.
			auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));
			m_ePlaneType = GetPlaneTypeByRay(stPickingRayInfo);

			if (m_ePlaneType != PLANE::TYPE_END) {
				m_bIsPicking = true;
			}
			else {
				SetObject(nullptr);
				ActivateGizmo(false);
			}
		}
	}
	else if (m_bIsPicking) {
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB)) {
			switch (m_eGizmoMode) {
				case MAP_TAB::MODE_POSITION :
					DragObject();
					break;
				case MAP_TAB::MODE_ROTATION :
					RotateObject();
					break;
				case MAP_TAB::MODE_SCALE:
					ScaleObject();
					break;
			}
			
		}
		else {
			// 드래그가 끝났다면, 픽킹 상태가 아닌 것이다.
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
			GetTransform()->SetAngle(0.f, 0.f, 0.f);
		}
	}

	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return 0;
}

void CGizmo::Render_Object(void)
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

	//DWORD bIsZBufferEnabled;
	//m_pGraphicDev->GetRenderState(D3DRS_ZENABLE, &bIsZBufferEnabled);
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	for (_int i = 0; i < AXIS_NUM; ++i) {
		if (m_ePlaneType == PLANE::TYPE_XY && i == 2) continue;
		else if (m_ePlaneType == PLANE::TYPE_XZ && i == 1) continue;
		else if (m_ePlaneType == PLANE::TYPE_YZ && i == 0) continue;

		pLine->DrawTransform(m_vAxisVertices[i], 2, &(matWorld * matView * matProj), m_clAxisColor[i]);
	}
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, bIsZBufferEnabled);

	pLine->End();
	pLine->Release();
	
}

CGizmo * CGizmo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGizmo*	pInstance = new CGizmo(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CGizmo::Free(void)
{
	CGameObject::Free();
}

void CGizmo::SetObject(Engine::CGameObject * _pGameObject)
{
	m_pGameObject = _pGameObject;

	if(_pGameObject)
		GetTransform()->SetPos(_pGameObject->GetTransform()->GetPos());
}

PLANE::E_TYPE CGizmo::GetPlaneTypeByRay(Engine::PICKINGRAYINFO & _rRayInfo)
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

void CGizmo::DragObject()
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
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			m_pGameObject->GetTransform()->SetPos(vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);
			m_pMapTab->UpdatePos(m_pGameObject->GetTransform()->GetPos());
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
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			m_pGameObject->GetTransform()->SetPos(vHitPos);
			GetTransform()->SetPos(vHitPos);
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CTabForm* pTabForm = dynamic_cast<CTabForm*>(pMain->m_MainSplitter.GetPane(0, 0));
			CNaviMeshTab* pNaviMeshTab = pTabForm->GetNaviMeshTab();
			pNaviMeshTab->UpdateVertexPos(vHitPos);
			m_pMapTab->UpdatePos(m_pGameObject->GetTransform()->GetPos());
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
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			m_pGameObject->GetTransform()->SetPos(vHitPos);
			GetTransform()->SetPos(vHitPos);
			m_pMapTab->UpdatePos(m_pGameObject->GetTransform()->GetPos());
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

void CGizmo::RotateObject()
{
	switch (m_ePlaneType) {
	case PLANE::TYPE_XY:
		// TODO : XY 평면에 대한 회전 갱신
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
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			_float fDeltaMousePosX = static_cast<_float>(ptDeltaMousePos.x);
			m_pGameObject->GetTransform()->RotateByAxis(-D3DXToRadian(fDeltaMousePosX / WINCX * 360.f), WORLD_Z_AXIS);
			m_pMapTab->UpdateAngle(m_pGameObject->GetTransform()->GetAngle());
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_XZ:
		// TODO : XZ 평면에 대한 회전 갱신
	{
		D3DXPLANE planeXZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XZTriLocal[0][i], &matWorld);
		}

		// 월드 XZ 평면을 구한다
		D3DXPlaneFromPoints(&planeXZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXZ, &fT)) {
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			_float fDeltaMousePosX = static_cast<_float>(ptDeltaMousePos.x);
			m_pGameObject->GetTransform()->RotateByAxis(-D3DXToRadian(fDeltaMousePosX / WINCX * 360.f), WORLD_Y_AXIS);
			m_pMapTab->UpdateAngle(m_pGameObject->GetTransform()->GetAngle());
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_YZ:
		// TODO : YZ 평면에 대한 회전 갱신
	{
		D3DXPLANE planeYZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &YZTriLocal[0][i], &matWorld);
		}

		// 월드 YZ 평면을 구한다
		D3DXPlaneFromPoints(&planeYZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeYZ, &fT)) {
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			_float fDeltaMousePosX = static_cast<_float>(ptDeltaMousePos.x);
			m_pGameObject->GetTransform()->RotateByAxis(D3DXToRadian(fDeltaMousePosX / WINCX * 360.f), WORLD_X_AXIS);
			m_pMapTab->UpdateAngle(m_pGameObject->GetTransform()->GetAngle());
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

void CGizmo::ScaleObject()
{
	switch (m_ePlaneType) {
	case PLANE::TYPE_XY:
		// TODO : XY 평면에 대한 스케일 갱신
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
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			GetTransform()->SetAngle(m_pGameObject->GetTransform()->GetAngle());
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			m_pGameObject->GetTransform()->Scaling(
				static_cast<_float>(ptDeltaMousePos.x) / WINCX * 10.f, 
				-static_cast<_float>(ptDeltaMousePos.y) / WINCY * 10.f, 0.f);
			m_pMapTab->UpdateScale(m_pGameObject->GetTransform()->GetScale());
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_XZ:
		// TODO : XZ 평면에 대한 스케일 갱신
	{
		D3DXPLANE planeXZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &XZTriLocal[0][i], &matWorld);
		}

		// 월드 XZ 평면을 구한다
		D3DXPlaneFromPoints(&planeXZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeXZ, &fT)) {
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			GetTransform()->SetAngle(m_pGameObject->GetTransform()->GetAngle());
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			m_pGameObject->GetTransform()->Scaling(
				static_cast<_float>(ptDeltaMousePos.x) / WINCX * 10.f,
				0.f, static_cast<_float>(ptDeltaMousePos.y) / WINCY * 10.f);
			m_pMapTab->UpdateScale(m_pGameObject->GetTransform()->GetScale());
		}
		else {
			m_ePlaneType = PLANE::TYPE_END;
			m_bIsPicking = false;
		}
	}
	break;
	case PLANE::TYPE_YZ:
		// TODO : YZ 평면에 대한 스케일 갱신
	{
		D3DXPLANE planeYZ;
		_vec3 vWorldTriVtx[TRI_VERTEX_NUM];
		_matrix matWorld = GetTransform()->GetObjectMatrix();
		_vec3 vHitPos;
		// 로컬 삼각형 정점들을 월드 좌표로 변환한다.
		for (_int i = 0; i < TRI_VERTEX_NUM; ++i) {
			D3DXVec3TransformCoord(&vWorldTriVtx[i], &YZTriLocal[0][i], &matWorld);
		}

		// 월드 YZ 평면을 구한다
		D3DXPlaneFromPoints(&planeYZ, &vWorldTriVtx[0], &vWorldTriVtx[1], &vWorldTriVtx[2]);
		// 광선을 구한다.
		auto stPickingRayInfo = Engine::GetPickingRayInfo(g_pTool3D_Kernel->GetGraphicDev(), Engine::GetClientCursorPoint(g_hWnd));

		// 광선과 평면이 충돌했는지 확인한다.
		float fT;
		if (Engine::IsRayAndPlaneCollided(stPickingRayInfo, planeYZ, &fT)) {
			// 충돌 지점으로 오브젝트와 기즈모를 이동시킨다.
			GetTransform()->SetAngle(m_pGameObject->GetTransform()->GetAngle());
			POINT ptDeltaMousePos = Engine::CDirectInputMgr::GetInstance()->GetDeltaMousePos();
			m_pGameObject->GetTransform()->Scaling(
				0.f,
				-static_cast<_float>(ptDeltaMousePos.y) / WINCY * 10.f
				, -static_cast<_float>(ptDeltaMousePos.x) / WINCX * 10.f);
			m_pMapTab->UpdateScale(m_pGameObject->GetTransform()->GetScale());
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
