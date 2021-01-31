#include "pch.h"
#include "CannonBall.h"
#include "Map.h"
#include "StaticObject.h"
#include "EndScene.h"
#include "Attribute.h"
#include "Ship.h"

CCannonBall::CCannonBall(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CCannonBall::CCannonBall(const CCannonBall & rhs)
	:
	CGameObject(rhs)
{
}

CCannonBall::~CCannonBall(void)
{
}

HRESULT CCannonBall::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"CannonBall"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);

	// Load Colliders
	LoadColliders(L"CannonBall.col");
	m_pAttackCollider = GetColliderFromTag(L"Ball");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();

	return S_OK;
}

int CCannonBall::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	_vec3 vPos = GetTransform()->GetPos();
	if (vPos.x > SEA_HALF_WIDTH + 10.f) {
		SetValid(false);
		return 1;
	}
	else if (vPos.x > SEA_HALF_WIDTH) {
		if (m_pAttackCollider->IsActivated())
			m_pAttackCollider->SetActivated(false);
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CCannonBall::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;


	m_pRenderer->Render(m_pShader->Get_EffectHandle());

	//_vec3 vPos = GetTransform()->GetPos();
	//_tchar tcBuf[50];
	//wsprintf(tcBuf, L"%f %f", vPos.x, vPos.y);
	//Engine::Render_Font(L"Font_Jinji", tcBuf, &_vec2(WINCX >> 1, WINCY >> 1), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

_bool CCannonBall::LoadColliders(const _tchar* _pFileName)
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

void CCannonBall::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	switch (m_eType) {
	case TYPE_PLAYER: {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Monster") == 0) {
			_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(1.f);
			// TODO : Æø¹ß ÀÌÆåÆ®¸¦ ³Ö´Â´Ù.
			SetValid(false);
		}
	}
		break;
	case TYPE_MONSTER: {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			CShip* pShip = dynamic_cast<CShip*>(_tCollisionInfo.pCollidedObject);
			if (pShip && !pShip->IsAttacked()) {
				_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(1.f);
				pShip->Attacked();
				// TODO : Æø¹ß ÀÌÆåÆ®¸¦ ³Ö´Â´Ù.
				SetValid(false);
			}
		}
	}
		break;
	}


	
}

void CCannonBall::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CCannonBall * CCannonBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCannonBall*	pInstance = new CCannonBall(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CCannonBall::Free(void)
{
	CGameObject::Free();
}

void CCannonBall::SetCannonBallInfo(E_TYPE _eType, const _vec3 & _vPos, const _vec3 & _vVelocity)
{
	m_eType = _eType;
	GetTransform()->SetPos(_vPos);
	m_pPhysics->SetVelocity(_vVelocity);
}