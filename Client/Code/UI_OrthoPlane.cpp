#include "pch.h"
#include "UI_OrthoPlane.h"
#include "UI_Image.h"
#include "Attribute.h"
#include "AliceW.h"

CUI_OrthoPlane::CUI_OrthoPlane(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUI_OrthoPlane::CUI_OrthoPlane(const CUI_OrthoPlane & rhs)
	:
	CGameObject(rhs)
{
}

CUI_OrthoPlane::~CUI_OrthoPlane(void)
{
}

HRESULT CUI_OrthoPlane::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", m_pShader);
	return S_OK;
}

_int CUI_OrthoPlane::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);
	m_pRenderer->Update(_fDeltaTime);
	return 0;
}

void CUI_OrthoPlane::Render_Object(void)
{
	/*m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());*/
	_matrix		matWorld, matView, matOriginView, matOriginProj;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOriginView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOriginProj);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fPosX - WINCX * 0.5f;
	matWorld._42 = -m_fPosY + WINCY * 0.5f;
	matWorld._43 = m_fPosZ;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pRenderer->SetWorldMatrix(matWorld);
	m_pRenderer->SetViewMatrix(matView);
	m_pRenderer->SetProjMatirx(m_matProj);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	/*m_pRenderer->Render();*/
	//LPD3DXEFFECT	 pEffect = m_pShader->Get_EffectHandle();
	//NULL_CHECK(pEffect);
	//Engine::Safe_AddRef(pEffect);

	//_uint	iMaxPass = 0;

	//pEffect->Begin(&iMaxPass, 0);	// 현재 쉐이더 파일이 갖고 있는 최대 패스의 개수를 리턴, 사용하는 방식
	//pEffect->BeginPass(0);

	//_matrix		matWorld, matView, matProj;

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);
		
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	//m_pMeshCom->Render_Meshes(pEffect);
	
	//pEffect->EndPass();
	//pEffect->End();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOriginView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOriginProj);
}

CUI_OrthoPlane * CUI_OrthoPlane::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_OrthoPlane*	pInstance = new CUI_OrthoPlane(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CUI_OrthoPlane::Free(void)
{
	CGameObject::Free();
}

void CUI_OrthoPlane::SetTexture(const _tchar * _pTextureTag)
{
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")), static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, _pTextureTag)));
}
