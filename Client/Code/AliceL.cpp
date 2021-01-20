#include "pch.h"
#include "AliceL.h"
#include "Map.h"
#include "StateMgr.h"
#include "AliceLState_Idle.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Attribute.h"
#include "BunnyBomb.h"
#include "Platform.h"
#include "UI_InGame.h"
#include "UI_WeaponLock.h"
#include "UI_LockedWeapon.h"
#include "UI_SpeechBubble.h"
#include "Cat.h"
#include "Trigger.h"

CAliceL::CAliceL(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CAliceL::CAliceL(const CAliceL & rhs)
	:
	CGameObject(rhs)
{
}

CAliceL::~CAliceL(void)
{
}

HRESULT CAliceL::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"AliceL"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"AliceL.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED);

	// StateMgr
	m_pStateMgr = new CStateMgr<CAliceL>(*this);
	m_pStateMgr->SetNextState(new CAliceLState_Idle(*this));

	// SpeechBubble;
	m_pSpeechBubble = CUI_SpeechBubble::Create(m_pGraphicDev);
	AddChild(m_pSpeechBubble);

	return S_OK;
}

int CAliceL::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_pMap) {
		// 맵 세팅
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
		_vec3 vPos = GetTransform()->GetPos();
		m_iCellIndex = m_pMap->GetNaviMesh()->GetNaviIndexByPos(vPos);
		if (m_iCellIndex != -1) {
			_float fHeight = m_pMap->GetNaviMesh()->GetCell(m_iCellIndex)->GetHeight(vPos);
			GetTransform()->SetPosY(fHeight);
		}
		// 고양이 찾기
		auto& rMapObject = Engine::GetLayer(L"Environment")->GetLayerList(L"MapObjects");
		for (auto pObj : rMapObject) {
			if (m_pCat = dynamic_cast<CCat*>(pObj))
				break;
		}
	}
	if (!m_pStateMgr->ConfirmValidState())
		return 1;

	// 부모 먼저 렌더러에 들어가야 올바르게 자식도 transform 됨.
	m_pRenderer->Update(_fDeltaTime);

	CGameObject::Update_Object(_fDeltaTime);

	m_pStateMgr->Update(_fDeltaTime);
	m_pMesh->Play_Animation(_fDeltaTime);

	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();

	_vec3 vCurrentPos = GetTransform()->GetPos();
	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	// 이동 확정
	GetTransform()->SetPos(pNaviMesh->MoveOnNaviMesh_Adhesion(m_iCellIndex, &vCurrentPos, &vTargetPos));

	return 0;
}

void CAliceL::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	m_pRenderer->Render();
}

_bool CAliceL::LoadColliders(const _tchar* _pFileName)
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

void CAliceL::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if(lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Trigger") == 0) {
		if (dynamic_cast<CTrigger*>(_tCollisionInfo.pCollidedObject)->GetTriggerType()
			== TRIGGER::TYPE_CHECKPOINT) {

			_int iIndex = 0;
			for (auto& rObj : m_pMap->GetCheckPoint()) {
				if (rObj->IsActivated()) {
					if (iIndex == 2)
						m_pSpeechBubble->SetText(WINCX / 6.f, WINCY - WINCY / 4.f, L"귀여운 고양이네! 일로 와보렴.");
					else if (iIndex == 3)
						m_pSpeechBubble->SetText(WINCX / 6.f, WINCY - WINCY / 4.f, L"어디까지 가는 거니?? 고양아~");
					break;
				}
				++iIndex;
			}
			if (iIndex != 1 && m_pMap->GetCheckPoint(iIndex)) {
				m_pCat->SetGoOn(true, m_pMap->GetCheckPoint(iIndex)->GetTransform()->GetPos());
			}
		}
	}
	
}

void CAliceL::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CAliceL * CAliceL::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAliceL*	pInstance = new CAliceL(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CAliceL::Free(void)
{
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

_bool CAliceL::IsMoving(const _float & _fDeltaTime, _vec3 * _pDir)
{
	if (!Engine::CDirectInputMgr::GetInstance()->IsMouseFixed())
		return false;

	_matrix matView;
	GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCamRight = _vec3(matView._11, matView._12, matView._13);
	vCamRight.y = 0.f;
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	_vec3 vCamLook = _vec3(matView._31, matView._32, matView._33);
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	_vec3 vDir = _vec3(0.f, 0.f, 0.f);

	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
		vDir -= vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
		vDir += vCamRight;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
		vDir += vCamLook;
	}
	if (Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
		vDir -= vCamLook;
	}

	if (D3DXVec3LengthSq(&vDir) != 0.f) {
		if (_pDir) {
			*_pDir = *D3DXVec3Normalize(&vDir, &vDir);
		}
		return true;
	}

	return false;
}