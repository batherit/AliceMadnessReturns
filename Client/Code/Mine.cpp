#include "pch.h"
#include "Mine.h"
#include "Map.h"
#include "StaticObject.h"
#include "EndScene.h"
#include "Attribute.h"
#include "CannonBall.h"
#include "Ship.h"
#include "ToothShip.h"
//#include "SplashAttack.h"
#include "MineBombAttack.h"

CMine::CMine(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMine::CMine(const CMine & rhs)
	:
	CGameObject(rhs)
{
}

CMine::~CMine(void)
{
}

HRESULT CMine::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Mine"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);

	// Load Colliders
	LoadColliders(L"Mine.col");
	m_pCollider = GetColliderFromTag(L"Monster");
	m_pDetect = GetColliderFromTag(L"Detect");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetDirection(_vec3(-1.f, 0.f, 0.f));
	m_pPhysics->SetSpeed(Engine::GetNumberBetweenMinMax(5.f, 6.f));

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(50.f, 50.f);

	return S_OK;
}

int CMine::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_bIsBang) {
		GetTransform()->RotateByUp(_fDeltaTime * D3DX_PI * 3.f);

		if ((m_fElapsedTime += _fDeltaTime) >= 1.f) {
			CMineBombAttack* pAttack = CMineBombAttack::Create(m_pGraphicDev);
			pAttack->SetMineBombAttackInfo(GetTransform()->GetPos());
			Engine::GetLayer(L"Environment")->Add_GameObject(pAttack);
			switch (Engine::GetNumberBetweenMinMax(0, 1)) {
			case 0:
				CSoundMgr::Get_Instance()->PlaySound(L"Mine_Bomb0.ogg", CSoundMgr::MONSTER);
				break;
			case 1:
				CSoundMgr::Get_Instance()->PlaySound(L"Mine_Bomb1.ogg", CSoundMgr::MONSTER);
				break;
			}
			
			SetValid(false);
		}
	}
	else {
		GetTransform()->RotateByUp(_fDeltaTime * D3DX_PI * 0.5f);
	}

	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CMine::Render_Object(void)
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

_bool CMine::LoadColliders(const _tchar* _pFileName)
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

void CMine::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (m_bIsBang)
		return;

	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		if (_tCollisionInfo.pCollidedMyCollider == m_pCollider) {
			CShip* pShip = dynamic_cast<CShip*>(_tCollisionInfo.pCollidedObject);
			if (pShip && !pShip->IsAttacked()) {
				_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(2.f);
				pShip->Attacked();
				
			}
		}
		Bang();
	}
	else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Ball") == 0) {
		Bang();
	}
}

void CMine::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CMine * CMine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMine*	pInstance = new CMine(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMine::Free(void)
{
	CGameObject::Free();
}

void CMine::Bang()
{
	m_bIsBang = true;
}
