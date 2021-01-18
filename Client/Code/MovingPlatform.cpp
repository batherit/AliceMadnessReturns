#include "pch.h"
#include "MovingPlatform.h"
#include "AliceW.h"
#include "Attribute.h"

CMovingPlatform::CMovingPlatform(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CMovingPlatform::CMovingPlatform(const CMovingPlatform & rhs)
	:
	CGameObject(rhs)
{
}

CMovingPlatform::~CMovingPlatform(void)
{
}

HRESULT CMovingPlatform::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"MovingPlatform"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"MovingPlatform.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	return S_OK;
}

int CMovingPlatform::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated()) {
		//m_bIsOn = false;
		return 1;
	}

	if (!m_bIsOn) {
		if ((m_fElapsedTime += _fDeltaTime) >= m_fKeepTime) {
			m_fElapsedTime = 0.f;
			m_bIsOn = true;
		}
	}
	else {
		if (!m_bIsEnd) {
			m_fElapsedTime += _fDeltaTime;
			_float fT = Engine::Clamp(m_fElapsedTime / m_fMovingTime, 0.f, 1.f);	// 0.6f초만에 엔드 지점으로 이동.
			_vec3 vPostPos = m_vStartPos * (1.f - fT) + m_vEndPos * fT;
			m_vDeltaPos = vPostPos - GetTransform()->GetPos();
			GetTransform()->SetPos(vPostPos);

			if (fT >= 1.f) {
				m_vDeltaPos = m_vEndPos - GetTransform()->GetPos();
				GetTransform()->SetPos(m_vEndPos);
				m_bIsEnd = true;
				m_fElapsedTime = 0.f;
			}
		}
		else {
			if (!m_bIsOff) {
				// 상태 유지
				if ((m_fElapsedTime += _fDeltaTime) >= m_fKeepTime) {
					m_bIsOff = true;
					m_fElapsedTime = 0.f;
				}
			}
			else {
				m_fElapsedTime += _fDeltaTime;
				_float fT = Engine::Clamp(m_fElapsedTime / m_fMovingTime, 0.f, 1.f);	// 1.f초만에 시작 지점으로 이동.
				_vec3 vPostPos = m_vEndPos * (1.f - fT) + m_vStartPos * fT;
				m_vDeltaPos = vPostPos - GetTransform()->GetPos();
				GetTransform()->SetPos(vPostPos);

				if (fT >= 1.f) {
					m_vDeltaPos = m_vStartPos - GetTransform()->GetPos();
					GetTransform()->SetPos(m_vStartPos);
					m_fElapsedTime = 0.f;
					m_bIsOn = false;
					m_bIsOff = false;
					m_bIsEnd = false;
				}
			}
		}
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);


	//m_bIsOn = false;
	return 0;
}

void CMovingPlatform::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CMovingPlatform::LoadColliders(const _tchar* _pFileName)
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

void CMovingPlatform::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	/*if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0 ||
		lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"BunnyBomb") == 0) {
		m_bIsOn = true;
	}*/
}

void CMovingPlatform::OnNotCollision(Engine::CollisionInfo _tCollisionInfo)
{
}

CMovingPlatform * CMovingPlatform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMovingPlatform*	pInstance = new CMovingPlatform(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CMovingPlatform::Free(void)
{
	CGameObject::Free();
}

//void CMovingPlatform::On(const _float& _fDeltaTime)
//{
//	_vec3 vPos = GetTransform()->GetPos();
//	_vec3 vPostPos = vPos + m_vVelocity * _fDeltaTime;
//
//	if (D3DXVec3Dot(&m_vDir, &(m_vEndPos - vPostPos)) < 0.f) {
//		vPostPos = m_vEndPos;
//	}
//
//	m_vDeltaPos = vPostPos - vPos;
//	GetTransform()->SetPos(vPostPos);
//}
//
//void CMovingPlatform::Off(const _float& _fDeltaTime)
//{
//	_vec3 vPos = GetTransform()->GetPos();
//	_vec3 vPostPos = vPos - m_vVelocity * _fDeltaTime;
//
//	if (D3DXVec3Dot(&(-m_vDir), &(m_vInitPos - vPostPos)) < 0.f) {
//		vPostPos = m_vInitPos;
//	}
//
//	m_vDeltaPos = vPostPos - vPos;
//	GetTransform()->SetPos(vPostPos);
//}
