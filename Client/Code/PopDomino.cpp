#include "pch.h"
#include "PopDomino.h"
#include "AliceW.h"

CPopDomino::CPopDomino(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CPopDomino::CPopDomino(const CPopDomino & rhs)
	:
	CGameObject(rhs)
{
}

CPopDomino::~CPopDomino(void)
{
}

HRESULT CPopDomino::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Domino"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Domino.col");
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_DEFERRED, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Dissolve"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	// Dissolve Texture
	m_pTexture = static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"EFT_Dissolve"));

	return S_OK;
}

int CPopDomino::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_bIsEventOn) {
		if (m_pAlice) {
			if (m_pAlice->IsSmalling()) {
				if (!m_bIsKeeped) {
					if ((m_fElapsedTime += _fDeltaTime) >= DOMINO_DELAY_TIME) {
						m_fElapsedTime = DOMINO_DELAY_TIME;
						m_bIsKeeped = true;
						m_fKeepTime = DOMINO_KEEP_TIME;
					}
				}

				m_fDissolveAmount = 1.f - m_fElapsedTime / DOMINO_DELAY_TIME;
			}
			else {
				if (m_bIsKeeped) {
					if ((m_fKeepTime -= _fDeltaTime) <= 0.f) {
						m_fElapsedTime = DOMINO_DELAY_TIME;
						m_bIsKeeped = false;
					}
				}
				else {
					m_fElapsedTime -= _fDeltaTime;
					if (m_fElapsedTime < 0.f)
						m_fElapsedTime = 0.f;
				}
				m_fDissolveAmount = 1.f - m_fElapsedTime / DOMINO_DELAY_TIME;
			}
		}
		else {
			m_pAlice = dynamic_cast<CAliceW*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Player").begin());
		}
	}

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CPopDomino::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//if (m_bIsVisible) {
		auto pEffect = m_pShader->Get_EffectHandle();
		m_pTexture->Set_Texture(pEffect, "g_DissolveTexture");
		pEffect->SetFloat("g_fDissolveAmount", m_fDissolveAmount);

		m_pRenderer->Render(pEffect);
	//}
	
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CPopDomino::LoadColliders(const _tchar* _pFileName)
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

CPopDomino * CPopDomino::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPopDomino*	pInstance = new CPopDomino(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CPopDomino::Free(void)
{
	CGameObject::Free();
}

void CPopDomino::EventOn()
{
	m_bIsEventOn = true;
	m_bIsVisible = true;
	m_fDissolveAmount = 0.f;
}
