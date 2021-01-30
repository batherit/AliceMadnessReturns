#include "pch.h"
#include "HPFlower.h"
#include "AliceW.h"
#include "Attribute.h"

CHPFlower::CHPFlower(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CHPFlower::CHPFlower(const CHPFlower & rhs)
	:
	CGameObject(rhs)
{
}

CHPFlower::~CHPFlower(void)
{
}

HRESULT CHPFlower::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"HPFlower"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"HPFlower.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	m_pMesh->Set_AnimationSet(ANIM::ShrinkFlower_Idle);

	return S_OK;
}

int CHPFlower::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_pAlice) {
		if (!m_bIsEating && m_pAlice->IsSmalling()) {
			m_pAlice->GetTransform()->SetPos(GetTransform()->GetPos());
			m_pMesh->Set_AnimationSet(ANIM::ShrinkFlower_Eat);
			m_bIsEating = true;
			// TODO : 플라워 카메라 컨트롤러로 변경
		}
		else if (m_bIsEating && !m_pAlice->IsSmalling()) {
			// TODO : 플레이어 카메라 컨트롤러로 변경
			SetValid(false);
			return 1;
		}
		else if(m_bIsEating && m_pAlice->IsSmalling()){
			m_pAlice->GetTransform()->SetPos(GetTransform()->GetPos());
			m_pAlice->GetAttribute()->IncreaseHP(HP_RECOVERY_SPEED * _fDeltaTime);
		}
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	m_pMesh->Play_Animation(_fDeltaTime);


	return 0;
}

void CHPFlower::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	m_pMesh->UpdateAnimation();
	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CHPFlower::LoadColliders(const _tchar* _pFileName)
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

void CHPFlower::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0)
			m_pAlice = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
	}
}

void CHPFlower::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			m_pAlice = nullptr;
		}

		
	}
}

CHPFlower * CHPFlower::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHPFlower*	pInstance = new CHPFlower(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CHPFlower::Free(void)
{
	CGameObject::Free();
}
