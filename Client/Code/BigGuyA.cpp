#include "pch.h"
#include "BigGuyA.h"
#include "Map.h"
#include "StateMgr.h"
#include "BigGuyAState_Idle.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Route.h"
#include "UI_SpeechBubble.h"

CBigGuyA::CBigGuyA(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBigGuyA::CBigGuyA(const CBigGuyA & rhs)
	:
	CGameObject(rhs)
{
}

CBigGuyA::~CBigGuyA(void)
{
}

HRESULT CBigGuyA::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"BigGuyA"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"BigGuyA.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED);

	// StateMgr
	m_pStateMgr = new CStateMgr<CBigGuyA>(*this);
	m_pStateMgr->SetNextState(new CBigGuyAState_Idle(*this));

	// Route
	m_pRoute = AddComponent<CRoute>();

	// SpeechBubble
	m_pSpeechBubble = CUI_SpeechBubble::Create(m_pGraphicDev);
	AddChild(m_pSpeechBubble);

	return S_OK;
}

int CBigGuyA::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	/*if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
		m_iCellIndex = m_pMap->GetNaviMesh()->GetNaviIndexByPos(GetTransform()->GetPos());
	}*/
	if (!m_pStateMgr->ConfirmValidState())
		return 1;

	// 부모 먼저 렌더러에 들어가야 올바르게 자식도 transform 됨.
	m_pRenderer->Update(_fDeltaTime);

	CGameObject::Update_Object(_fDeltaTime);

	m_pStateMgr->Update(_fDeltaTime);
	m_pMesh->Play_Animation(_fDeltaTime);

//	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
//	_vec3 vCurrentPos = GetTransform()->GetPos();
//	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	// 이동 확정
//	GetTransform()->SetPos(pNaviMesh->MoveOnNaviMesh_Adhesion(m_iCellIndex, &vCurrentPos, &vTargetPos));

	return 0;
}

void CBigGuyA::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;
	if (!m_bIsVisible)
		return;

	m_pRenderer->Render();
}

_bool CBigGuyA::LoadColliders(const _tchar* _pFileName)
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, _pFileName);

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	LoadCollidersInfo(hFile);

	CloseHandle(hFile);

	return true;
}

void CBigGuyA::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (!m_bIsTalking) {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Talk") == 0) {
			m_bIsTalking = true;
		}
	}
}

void CBigGuyA::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (m_bIsTalking) {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Talk") == 0) {
		//	ShoutOut(WINCX* 0.5f, WINCY - WINCY / 6.f, L"왜 일을 안하고 있는 거지,,,??");

			m_bIsTalking = false;
		}
	}
}

CBigGuyA * CBigGuyA::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigGuyA*	pInstance = new CBigGuyA(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CBigGuyA::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

_bool CBigGuyA::LoadRoutePosList(const _tchar * _pRouteFilePath)
{
	FILE* fpIn;
	errno_t err = 0;

	TCHAR tmp[256] = L"";
	lstrcpy(tmp, _pRouteFilePath);

	_int len = 256;
	char ctemp[256];

	WideCharToMultiByte(CP_ACP, 0, tmp, len, ctemp, len, NULL, NULL);

	err = fopen_s(&fpIn, ctemp, "rt");
	if (!err) {
		vector<_vec3> vecPosList;
		vecPosList.reserve(300);

		_int iSize = 0;
		fscanf_s(fpIn, "%d", &iSize);
		
		_float fX, fY, fZ;
		for (_int i = 0; i < iSize; ++i) {
			fscanf_s(fpIn, "%f %f %f", &fX, &fY, &fZ);
			vecPosList.emplace_back(_vec3(fX, fY, fZ));
		}
		m_pRoute->SetPosList(vecPosList);
	}
	else abort();

	if (fpIn)
		fclose(fpIn);

	return true;
}

void CBigGuyA::ShoutOut(_float _fOffsetX, _float _fOffsetY, const wstring & _wstrText)
{
	m_pSpeechBubble->SetText(_fOffsetX, _fOffsetY, _wstrText);
}

//_bool CBigGuyA::IsMoving(const _float & _fDeltaTime, _vec3 * _pDir)
//{
//	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
//		return false;
//
//	_matrix matView;
//	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, nullptr, &matView);
//
//	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
//	vCamRight.y = 0.f;
//	D3DXVec3Normalize(&vCamRight, &vCamRight);
//	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
//	vCamLook.y = 0.f;
//	D3DXVec3Normalize(&vCamLook, &vCamLook);
//	_vec3 vDir = _vec3(0.f, 0.f, 0.f);
//
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
//		vDir -= vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
//		vDir += vCamRight;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
//		vDir += vCamLook;
//	}
//	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
//		vDir -= vCamLook;
//	}
//
//	if (D3DXVec3LengthSq(&vDir) != 0.f) {
//		if (_pDir) {
//			*_pDir = *D3DXVec3Normalize(&vDir, &vDir);
//		}
//		return true;
//	}
//
//	return false;
//}