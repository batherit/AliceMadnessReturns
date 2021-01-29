#include "pch.h"
#include "CrushingFist.h"
#include "AliceW.h"
#include "Attribute.h"

CCrushingFist::CCrushingFist(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CCrushingFist::CCrushingFist(const CCrushingFist & rhs)
	:
	CGameObject(rhs)
{
}

CCrushingFist::~CCrushingFist(void)
{
}

HRESULT CCrushingFist::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"CrushingFist"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"CrushingFist.col");
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

int CCrushingFist::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_bIsEventOn) {
		if (!m_bIsOn) {
			if ((m_fElapsedTime += _fDeltaTime) >= m_fKeepTime) {
				m_fElapsedTime = 0.f;
				m_bIsOn = true;
			}
		}
		else {
			if (!m_bIsEnd) {
				m_fElapsedTime += _fDeltaTime;
				_float fT = Engine::Clamp(m_fElapsedTime / m_fPressTime, 0.f, 1.f);	// 0.6f초만에 엔드 지점으로 이동.
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
					if ((m_fElapsedTime += _fDeltaTime) >= m_fKeepTime) {
						/*GetTransform()->SetPos(m_vStartPos);
						SetValid(true);
						return 0;*/
						m_bIsOff = true;
						m_fElapsedTime = 0.f;
					}
				}
				else {
					m_fElapsedTime += _fDeltaTime;
					_float fT = Engine::Clamp(m_fElapsedTime / m_fBackTime, 0.f, 1.f);	// 1.f초만에 시작 지점으로 이동.
					GetTransform()->SetPos(m_vEndPos * (1.f - fT) + m_vStartPos * fT);

					if (fT >= 1.f) {
						GetTransform()->SetPos(m_vStartPos);
						m_fElapsedTime = 0.f;
						m_bIsOn = false;
						m_bIsOff = false;
						m_bIsEnd = false;
					}
				}
			}
		}
	}
	

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CCrushingFist::Render_Object(void)
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

_bool CCrushingFist::LoadColliders(const _tchar* _pFileName)
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

void CCrushingFist::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
		if (m_pPlayerAttribute) {
			if (!m_pPlayerAttribute->IsDead()) {
				m_pPlayerAttribute->Damaged(1000.f);
			}
		}
		else {
			m_pPlayerAttribute = _tCollisionInfo.pCollidedObject->GetComponent<CAttribute>();
			if (!m_pPlayerAttribute->IsDead()) {
				m_pPlayerAttribute->Damaged(1000.f);
			}
		}
	}
}

CCrushingFist * CCrushingFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrushingFist*	pInstance = new CCrushingFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CCrushingFist::Free(void)
{
	CGameObject::Free();
}

void CCrushingFist::SetCrushingFistInfo(const _vec3 & _vEndPos, _float _fPressTime, _float _fKeepTime, _float _fBackTime)
{
	m_vStartPos = GetTransform()->GetPos();
	m_vEndPos = _vEndPos;
	m_fPressTime = _fPressTime;
	m_fKeepTime = _fKeepTime;
	m_fBackTime = _fBackTime;

	m_bIsOn = false;
	m_bIsOff = false;
	m_bIsEnd = false;
}

void CCrushingFist::EventOn()
{
	m_bIsEventOn = true;
	GetTransform()->SetPos(m_vStartPos);
}
