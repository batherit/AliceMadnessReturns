#include "pch.h"
#include "Valve.h"
#include "AliceW.h"

CValve::CValve(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CValve::CValve(const CValve & rhs)
	:
	CGameObject(rhs)
{
}

CValve::~CValve(void)
{
}

HRESULT CValve::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Valve"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Valve.col");
	m_pCollider = GetColliderFromTag(L"Valve");
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	return S_OK;
}

int CValve::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	m_fRadian += D3DXToRadian(90.f) * _fDeltaTime;
	if (m_fRadian >= D3DX_PI * 2.f) {
		m_fRadian -= D3DX_PI * 2.f;
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CValve::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CValve::LoadColliders(const _tchar* _pFileName)
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

void CValve::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			CAliceW* pAlice = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
			if (pAlice) {
				_float fVelocityY = pAlice->GetPhysics()->GetVelocity().y;
				if (fVelocityY <= -20.f)
					pAlice->GetPhysics()->SetVelocityY(-20.f);
				else if(fVelocityY >= 10.f)
					pAlice->GetPhysics()->SetVelocityY(10.f);
				_float fIntensity = 1.f + Engine::Clamp(pAlice->GetPhysics()->GetVelocity().y, -10.f, 0.f) * 0.1f;	// 0 -> 최대 낙하 속도
				fIntensity = Engine::GetValueByWeight(fIntensity, 1.2f, 0.8f);
				/*_float fToAliceY = Engine::Clamp(_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos().y - GetTransform()->GetPos().y, 5.f, 10.f);
				_float fT = Engine::GetWeightByValue(fToAliceY, 5.f, 10.f);*/
				pAlice->GetPhysics()->AddVelocityY(fIntensity/* + Engine::GetValueByWeight(fT, 0.f, 0.1f)*/);
			}
		}
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		
	}
}

void CValve::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
	//if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
	//	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetCaolliderTag(), L"Player") == 0) {
	//		CAliceW* pAlice = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
	//		if (pAlice) {
	//			_int a = 10;
	//			//_float fIntensity =(2.f + Engine::Clamp(pAlice->GetPhysics()->GetVelocity().y, -2.f, 0.f)) * 0.5f;	// 0 -> 최대 낙하 속도
	//			//fIntensity = Engine::GetValueByWeight(fIntensity, 8.f, 2.f);
	//			//_float fToAliceY = Engine::Clamp(_tCollisionInfo.pCollidedCollider->GetTransform()->GetPos().y - GetTransform()->GetPos().y, 1.f, 10.f);
	//			//_float fT = Engine::GetWeightByValue(fToAliceY, 1.f, 10.f);
	//			//pAlice->GetPhysics()->AddVelocityY(fIntensity * Engine::GetValueByWeight(fT, 5.f, 3.f));
	//		}
	//	}
	//}
	//else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
	//	_int a = 10;
	//}
	//else {

	//}
}

CValve * CValve::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CValve*	pInstance = new CValve(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CValve::Free(void)
{
	CGameObject::Free();
}
