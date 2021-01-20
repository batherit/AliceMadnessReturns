#include "pch.h"
#include "JumpPad.h"
#include "AliceW.h"
#include "Attribute.h"

CJumpPad::CJumpPad(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CJumpPad::CJumpPad(const CJumpPad & rhs)
	:
	CGameObject(rhs)
{
}

CJumpPad::~CJumpPad(void)
{
}

HRESULT CJumpPad::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"JumpPad"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"JumpPad.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	m_pMesh->Set_AnimationSet(ANIM::GlowingJumpPad_ready);

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();

	return S_OK;
}

int CJumpPad::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_bIsLaunched && m_pMesh->GetAnimationProgress() >= 0.95f) {
		m_pMesh->Set_AnimationSet(ANIM::GlowingJumpPad_ready);
		m_pAttribute->ClearAttackerList();
		m_bIsLaunched = false;
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	m_pMesh->Play_Animation(_fDeltaTime);


	return 0;
}

void CJumpPad::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CJumpPad::LoadColliders(const _tchar* _pFileName)
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

void CJumpPad::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			if (m_pAttribute->RegisterAttacker(_tCollisionInfo.pCollidedCollider)) {
				CAliceW* pAliceW = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
				if (pAliceW) {
					pAliceW->SetSuperJumped(true);
					m_bIsLaunched = true;
				}
				m_pMesh->Set_AnimationSet(ANIM::GrowingJumpPad_Launch);
			}
		}
	}
}

void CJumpPad::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			// 충돌하지 않았다면 어태커에서 제거한다.
			m_pAttribute->ReleaseAttacker(_tCollisionInfo.pCollidedCollider);
		}
	}
}

CJumpPad * CJumpPad::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJumpPad*	pInstance = new CJumpPad(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CJumpPad::Free(void)
{
	CGameObject::Free();
}
