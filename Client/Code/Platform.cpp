#include "pch.h"
#include "Platform.h"
#include "AliceW.h"
#include "Attribute.h"

CPlatform::CPlatform(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPlatform::CPlatform(const CPlatform & rhs)
	:
	CGameObject(rhs)
{
}

CPlatform::~CPlatform(void)
{
}

HRESULT CPlatform::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Platform"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Platform.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	return S_OK;
}

int CPlatform::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated()) {
		//m_bIsOn = false;
		return 1;
	}

	/*if (m_bIsOn) {
		_vec3 vPos = GetTransform()->GetPos();
		vPos.y -= 0.4f / PRESS_TIME * _fDeltaTime;
		if (vPos.y < m_vEndPos.y)
			vPos.y = m_vEndPos.y;
		GetTransform()->SetPos(vPos);
	}
	else {
		_vec3 vPos = GetTransform()->GetPos();
		vPos.y += 0.4f / PRESS_TIME * _fDeltaTime;
		if (vPos.y > m_vInitPos.y)
			vPos.y = m_vInitPos.y;
		GetTransform()->SetPos(vPos);
	}*/

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);


	//m_bIsOn = false;
	return 0;
}

void CPlatform::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CPlatform::LoadColliders(const _tchar* _pFileName)
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

void CPlatform::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	/*if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0 ||
		lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"BunnyBomb") == 0) {
		m_bIsOn = true;
	}*/
}

void CPlatform::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CPlatform * CPlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlatform*	pInstance = new CPlatform(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPlatform::Free(void)
{
	CGameObject::Free();
}

void CPlatform::On(const _float& _fDeltaTime)
{
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vPostPos = vPos + m_vVelocity * _fDeltaTime;

	if (D3DXVec3Dot(&m_vDir, &(m_vEndPos - vPostPos)) < 0.f) {
		vPostPos = m_vEndPos;
	}

	m_vDeltaPos = vPostPos - vPos;
	GetTransform()->SetPos(vPostPos);
}

void CPlatform::Off(const _float& _fDeltaTime)
{
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vPostPos = vPos - m_vVelocity * _fDeltaTime;

	if (D3DXVec3Dot(&(-m_vDir), &(m_vInitPos - vPostPos)) < 0.f) {
		vPostPos = m_vInitPos;
	}

	m_vDeltaPos = vPostPos - vPos;
	GetTransform()->SetPos(vPostPos);
}
