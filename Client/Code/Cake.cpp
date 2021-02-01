#include "pch.h"
#include "Cake.h"
#include "Map.h"
#include "StaticObject.h"
#include "EndScene.h"

CCake::CCake(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CCake::CCake(const CCake & rhs)
	:
	CGameObject(rhs)
{
}

CCake::~CCake(void)
{
}

HRESULT CCake::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Cake"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);

	// Load Colliders
	LoadColliders(L"Cake.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	GetTransform()->SetPos(0.f, 1.f, 0.f);
	GetTransform()->SetScaleXYZ(2.f, 2.f, 2.f);

	return S_OK;
}

int CCake::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	GetTransform()->RotateByUp(_fDeltaTime * D3DXToRadian(360.f));

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CCake::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	m_pRenderer->Render(m_pShader->Get_EffectHandle());
}

_bool CCake::LoadColliders(const _tchar* _pFileName)
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

void CCake::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		Engine::CManagement::GetInstance()->SetNextScene(CEndScene::Create(m_pGraphicDev));
		SetValid(false);
	}
}

void CCake::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CCake * CCake::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCake*	pInstance = new CCake(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CCake::Free(void)
{
	CGameObject::Free();
}