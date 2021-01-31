#include "pch.h"
#include "Ship.h"
#include "Map.h"
#include "StaticObject.h"
#include "EndScene.h"
#include "Attribute.h"
#include "CannonBall.h"
#include "UI_HPBar.h"
#include "StaticCamera.h"

CShip::CShip(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CShip::CShip(const CShip & rhs)
	:
	CGameObject(rhs)
{
}

CShip::~CShip(void)
{
}

HRESULT CShip::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Ship"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);

	// Load Colliders
	LoadColliders(L"Ship.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Dissolve"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(0.f);

	//GetTransform()->SetPos(0.f, -15.f, 0.f);
	GetTransform()->RotateByUp(-D3DX_PI * 0.5f);

	m_pAttribute = AddComponent<CAttribute>();
	m_pAttribute->SetHP(10.f, 10.f);

	// Dissolve
	m_pTexture = static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Dissolve"));

	return S_OK;
}

int CShip::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (m_bIsAttacked) {
		if ((m_fRecoveryTime -= _fDeltaTime) > 0.f) {
			if (IsVisible()) {
				SetVisible(false);
			}
			else {
				SetVisible(true);
			}
		}
		else {
			SetVisible(true);
			m_bIsAttacked = false;
		}
	}

	if (!m_pAttribute->IsDead()) {
		_vec2 vDir = _vec2(0.f, 0.f);
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_W) || Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_W)) {
			vDir.y += 1.f;
		}
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_A) || Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_A)) {
			vDir.x -= 1.f;
		}
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_S) || Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_S)) {
			vDir.y -= 1.f;
		}
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(DIK_D) || Engine::CDirectInputMgr::GetInstance()->IsKeyPressing(DIK_D)) {
			vDir.x += 1.f;
		}
		if (Engine::CDirectInputMgr::GetInstance()->IsKeyDown(Engine::DIM_LB)) {
			Bang();
		}

		if (D3DXVec2LengthSq(&vDir) > 0.f) {
			D3DXVec2Normalize(&vDir, &vDir);
			m_pPhysics->SetDirection(_vec3(vDir.x, vDir.y, 0.f));
			m_pPhysics->SetSpeed(20.f);
			m_pPhysics->SetResistanceCoefficientXZ(1.f);
			m_pPhysics->SetResistanceCoefficientY(1.f);
		}
		else {
			m_pPhysics->SetResistanceCoefficientXZ(0.9f);
			m_pPhysics->SetResistanceCoefficientY(0.9f);
		}
	}
	else {
		if (!m_bIsDeathInit) {
			m_pPhysics->SetSpeed(0.f);
			m_pPhysics->SetGravity(-3.f);
			m_bIsDeathInit = true;
		}

		if ((m_fDeathAnimTime -= _fDeltaTime) > 0.f) {
			m_fDissolveAmount = 1.f - m_fDeathAnimTime / 2.f;
			GetTransform()->RotateByLook(D3DX_PI * _fDeltaTime * 0.2f);
		}
		else {
			m_pHPBar->Off();
			//SetValid(false);
			return 1;
		}
	}
	

	GetTransform()->SetPos(m_pPhysics->GetUpdatedPos(_fDeltaTime));
	_vec3 vPos = GetTransform()->GetPos();
	if (vPos.y >= SEA_HALF_HEIGHT)
		GetTransform()->SetPosY(SEA_HALF_HEIGHT);
	else if (vPos.y <= -SEA_HALF_HEIGHT)
		GetTransform()->SetPosY(-SEA_HALF_HEIGHT);

	if (vPos.x >= SEA_HALF_WIDTH)
		GetTransform()->SetPosX(SEA_HALF_WIDTH);
	else if(vPos.x <= -SEA_HALF_WIDTH)
		GetTransform()->SetPosX(-SEA_HALF_WIDTH);

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CShip::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

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

_bool CShip::LoadColliders(const _tchar* _pFileName)
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

void CShip::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

void CShip::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CShip * CShip::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShip*	pInstance = new CShip(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CShip::Free(void)
{
	CGameObject::Free();
}

void CShip::SetHPBar(CUI_HPBar * _pHPBar)
{
	m_pHPBar = _pHPBar;
	m_pHPBar->On(this);
}

void CShip::Bang()
{
	CCannonBall* pCannonBall = CCannonBall::Create(m_pGraphicDev);
	pCannonBall->SetCannonBallInfo(CCannonBall::TYPE_PLAYER, GetTransform()->GetPos() + _vec3(1.f, 0.f, 0.f), _vec3(40.f, 0.f, 0.f));
	Engine::GetLayer(L"Environment")->Add_GameObject(pCannonBall);
}

void CShip::Attacked()
{
	m_bIsAttacked = true;
	m_fRecoveryTime = 1.f;
	dynamic_cast<CStaticCamera*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Camera").begin())->Shake(0.5f, 0.3f, 10);
}
