#include "pch.h"
#include "IronHand.h"
#include "AliceW.h"

CIronHand::CIronHand(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CIronHand::CIronHand(const CIronHand & rhs)
	:
	CGameObject(rhs)
{
}

CIronHand::~CIronHand(void)
{
}

HRESULT CIronHand::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"IronHand"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"IronHand.col");
	GetColliderFromTag(L"EnemyAttack")->SetDamage(10.f);
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	GetTransform()->RotateByLook(D3DX_PI);
	GetTransform()->SetScaleXYZ(0.3f, 0.3f, 0.3f);

	return S_OK;
}

int CIronHand::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	//if (m_pAlice) {
	//	if (m_pAlice->IsSmalling()) {
	//		m_bIsVisible = true;
	//	}
	//	else {
	//		m_bIsVisible = false;
	//	}
	//}
	//else {
	//	m_pAlice = dynamic_cast<CAliceW*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
	//}

	if (!m_bIsOn) {
		if ((m_fElapsedTime += _fDeltaTime) >= m_fActivationTime) {
			m_fElapsedTime = 0.f;
			m_bIsOn = true;
		}
	}
	else {
		if (!m_bIsEnd) {
			m_fElapsedTime += _fDeltaTime;
			_float fT = Engine::Clamp(m_fElapsedTime / 0.3f, 0.f, 1.f);	// 0.6f초만에 엔드 지점으로 이동.
			GetTransform()->SetPos(m_vStartPos * (1.f - fT) + m_vEndPos * fT);

			if (fT >= 1.f) {
				GetTransform()->SetPos(m_vEndPos);
				m_bIsEnd = true;
				m_fElapsedTime = 0.f;
			}
		}
		else {
			if (!m_bIsOff) {
				// 상태 유지
				if ((m_fKeepTime -= _fDeltaTime) <= 0.f) {
					/*GetTransform()->SetPos(m_vStartPos);
					SetValid(true);
					return 0;*/
					m_bIsOff = true;
					m_fElapsedTime = 0.f;
				}
			}
			else {
				m_fElapsedTime += _fDeltaTime;
				_float fT = Engine::Clamp(m_fElapsedTime / 1.5f, 0.f, 1.f);	// 1.f초만에 시작 지점으로 이동.
				GetTransform()->SetPos(m_vEndPos * (1.f - fT) + m_vStartPos * fT);

				if (fT >= 1.f) {
					GetTransform()->SetPos(m_vStartPos);
					m_fElapsedTime = 0.f;
					SetValid(false);
					return 1;
				}
			}
		}
	}


	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CIronHand::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_bIsVisible) {
		m_pRenderer->Render(m_pShader->Get_EffectHandle());
	}

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CIronHand::LoadColliders(const _tchar* _pFileName)
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

CIronHand * CIronHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CIronHand*	pInstance = new CIronHand(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CIronHand::Free(void)
{
	CGameObject::Free();
}

void CIronHand::SetIronHandInfo(const _vec3 & _vStartPos, _float _fActivationTime)
{
	m_vStartPos = _vStartPos;
	m_vEndPos = _vStartPos;
	m_vEndPos.y += 5.f;
	GetTransform()->SetPos(_vStartPos);
	m_fActivationTime = _fActivationTime;
}
