#include "pch.h"
#include "Shark.h"
#include "Map.h"
#include "StaticObject.h"
#include "EndScene.h"
#include "Attribute.h"
#include "CannonBall.h"
#include "Ship.h"
#include "ToothShip.h"

CShark::CShark(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CShark::CShark(const CShark & rhs)
	:
	CGameObject(rhs)
{
}

CShark::~CShark(void)
{
}

HRESULT CShark::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Shark"));
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	m_pMesh->Set_AnimationSet(ANIM::Shark_Swim_Fast);

	// Load Colliders
	LoadColliders(L"Shark.col");
	m_pCollider = GetColliderFromTag(L"Monster");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Dissolve"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetDirection(_vec3(-1.f, 0.f, 0.f));
	m_pPhysics->SetSpeed(Engine::GetNumberBetweenMinMax(8.f, 11.f));

	//GetTransform()->SetPos(0.f, -15.f, 0.f);
	GetTransform()->RotateByUp(-D3DX_PI * 0.5f);

	// Attribute
	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(3.f, 5.f);

	// Dissolve Texture
	m_pTexture = static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Dissolve"));
	m_fAttackDelayTime = Engine::GetNumberBetweenMinMax(3.f, 5.f);

	return S_OK;
}

int CShark::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_pAttribute->IsDead()) {
		if (GetTransform()->GetPos().x < -SEA_HALF_WIDTH - 10.f) {
			SetValid(false);
			return 1;
		}

		if ((m_fAttackDelayTime -= _fDeltaTime) <= 0.f) {
			Bang();
			m_fAttackDelayTime = Engine::GetNumberBetweenMinMax(2.f, 4.f);
		}
	}
	else {
		if (!m_bIsDeathInit) {
			m_pPhysics->SetGravity(-3.f);
			m_pPhysics->SetResistanceCoefficientXZ(0.95f);
			m_pMesh->Set_AnimationSet(ANIM::Shark_swim);
			CToothShip* pToothShip = nullptr;
			_vec3 vPos = GetTransform()->GetPos();
			auto pLayer = Engine::GetLayer(L"Environment");
			for (_int i = 0; i < 2; ++i) {
				pToothShip = CToothShip::Create(m_pGraphicDev);
				pToothShip->SetPopTooth(vPos);
				pLayer->Add_GameObject(pToothShip);
			}
			m_pCollider->SetActivated(false);
			m_bIsDeathInit = true;
		}

		if ((m_fDeathAnimTime -= _fDeltaTime) > 0.f) {
			m_fDissolveAmount = 1.f - m_fDeathAnimTime / 2.f;
			GetTransform()->RotateByLook(D3DX_PI * _fDeltaTime * 0.2f);
		}
		else {
			SetValid(false);
			return 1;
		}
	}
	

	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));

	m_pMesh->Play_Animation(_fDeltaTime);
	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CShark::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pMesh->UpdateAnimation();

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	if (!m_bIsVisible)
		return;

	auto pEffect = m_pShader->Get_EffectHandle();
	m_pTexture->Set_Texture(pEffect, "g_DissolveTexture");
	pEffect->SetFloat("g_fDissolveAmount", m_fDissolveAmount);

	m_pRenderer->Render(pEffect);

	//_vec3 vPos = GetTransform()->GetPos();
	//_tchar tcBuf[50];
	//wsprintf(tcBuf, L"%f %f", vPos.x, vPos.y);
	//Engine::Render_Font(L"Font_Jinji", tcBuf, &_vec2(WINCX >> 1, WINCY >> 1), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

_bool CShark::LoadColliders(const _tchar* _pFileName)
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

void CShark::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (m_pAttribute->IsDead())
		return;

	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		CShip* pShip = dynamic_cast<CShip*>(_tCollisionInfo.pCollidedObject);
		if (pShip && !pShip->IsAttacked()) {
			_tCollisionInfo.pCollidedObject->GetComponent<CAttribute>()->Damaged(2.f);
			pShip->Attacked();
		}
	}
}

void CShark::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CShark * CShark::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShark*	pInstance = new CShark(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CShark::Free(void)
{
	CGameObject::Free();
}

void CShark::Bang()
{
	CCannonBall* pCannonBall = CCannonBall::Create(m_pGraphicDev);
	pCannonBall->SetCannonBallInfo(CCannonBall::TYPE_MONSTER, GetTransform()->GetPos() + _vec3(-1.f, 0.f, 0.f), _vec3(-20.f, 0.f, 0.f));
	Engine::GetLayer(L"Environment")->Add_GameObject(pCannonBall);
	CSoundMgr::Get_Instance()->PlaySound(L"Shark_Attack.ogg", CSoundMgr::MONSTER);
}
