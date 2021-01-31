#include "pch.h"
#include "ToothShip.h"
#include "Ship.h"

CToothShip::CToothShip(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CToothShip::CToothShip(const CToothShip & rhs)
	:
	CGameObject(rhs)
{
}

CToothShip::~CToothShip(void)
{
}

HRESULT CToothShip::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Tooth"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Tooth.col");
	m_pCollider = GetColliderFromTag(L"Tooth");
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(0.f);

	GetTransform()->SetScaleXYZ(5.f, 5.f, 5.f);

	return S_OK;
}

int CToothShip::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_bIsPoped) {
		if (m_pPhysics->GetSpeed() <= 0.05f) {
			m_pPhysics->SetVelocityXZ(_vec2(-5.f, 0.f));
			m_pPhysics->SetResistanceCoefficientXZ(1.f);
			m_pPhysics->SetResistanceCoefficientY(1.f);
			m_bIsPoped = false;
		}
	}
	else {
		if (m_pPhysics->GetVelocity().x >= -5.f) {
			m_pPhysics->AddVelocityXZ(_vec2(-2.5f * _fDeltaTime, 0.f));
		}
		else {
			m_pPhysics->SetVelocityXZ(_vec2(-5.f, 0.f));
		}
	}

	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CToothShip::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CToothShip::LoadColliders(const _tchar* _pFileName)
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

void CToothShip::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (!m_pShip && lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		m_pShip = dynamic_cast<CShip*>(_tCollisionInfo.pCollidedObject);
		if (m_pShip) {
			m_pShip->IncreaseToothNum(1);
			SetValid(false);
		}
	}
}

CToothShip * CToothShip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToothShip*	pInstance = new CToothShip(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CToothShip::Free(void)
{
	CGameObject::Free();
}

void CToothShip::SetPopTooth(const _vec3 & _vPos)
{
	GetTransform()->SetPos(_vPos);
	_vec3 vDir = WORLD_X_AXIS;
	_matrix matRot;
	D3DXVec3TransformNormal(&vDir, &vDir, D3DXMatrixRotationZ(&matRot, Engine::GetNumberBetweenMinMax(0.f, D3DX_PI * 2.f)));
	m_pPhysics->SetDirection(vDir);
	m_pPhysics->SetSpeed(15.f);
	m_pPhysics->SetResistanceCoefficientXZ(0.9f);
	m_pPhysics->SetResistanceCoefficientY(0.9f);
	m_bIsPoped = true;
}