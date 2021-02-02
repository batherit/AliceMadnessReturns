#include "pch.h"
#include "GiantAliceW.h"
#include "Map.h"
#include "StateMgr.h"
#include "GiantAliceWState_Scale.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "Trigger.h"
#include "Attribute.h"
#include "BunnyBomb.h"
#include "Platform.h"
#include "UI_InGame.h"
#include "UI_WeaponLock.h"
#include "UI_LockedWeapon.h"
#include "CreditScene.h"
//#include "UI_SpeechBubble.h"
#include "Cat.h"
#include "Trigger.h"

CGiantAliceW::CGiantAliceW(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CGiantAliceW::CGiantAliceW(const CGiantAliceW & rhs)
	:
	CGameObject(rhs)
{
}

CGiantAliceW::~CGiantAliceW(void)
{
}

HRESULT CGiantAliceW::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"AliceW"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"GiantAliceW.col");
	m_pHead = ExtractColliderFromTag(L"Head");
	m_pLeftFoot = ExtractColliderFromTag(L"Foot_L");
	m_pAttackCollider = GetColliderFromTag(L"Foot_R");
	m_pAttackCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(ALICE_RUN_SPEED);

	// StateMgr
	m_pStateMgr = new CStateMgr<CGiantAliceW>(*this);
	m_pStateMgr->SetNextState(new CGiantAliceWState_Scale(*this));

	// SpeechBubble;
	//m_pSpeechBubble = CUI_SpeechBubble::Create(m_pGraphicDev);
	//AddChild(m_pSpeechBubble);

	return S_OK;
}

int CGiantAliceW::Update_Object(const _float & _fDeltaTime)
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

void CGiantAliceW::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

_bool CGiantAliceW::LoadColliders(const _tchar* _pFileName)
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

void CGiantAliceW::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
		Engine::CManagement::GetInstance()->SetNextScene(CCreditScene::Create(m_pGraphicDev));
	}
}

void CGiantAliceW::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CGiantAliceW * CGiantAliceW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGiantAliceW*	pInstance = new CGiantAliceW(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CGiantAliceW::Free(void)
{
	Engine::Safe_Release(m_pHead);
	Engine::Safe_Release(m_pLeftFoot);
	Engine::Safe_Release(m_pStateMgr);
	CGameObject::Free();
}

_bool CGiantAliceW::IsAttackOn(const _float & _fDeltaTime)
{
	return Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB);
}

_bool CGiantAliceW::IsAttacking(const _float & _fDeltaTime)
{
	return Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(Engine::DIM_LB);
}

_bool CGiantAliceW::IsMoving(const _float & _fDeltaTime, _vec3 * _pDir)
{
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