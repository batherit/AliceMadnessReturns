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

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOriginView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOriginProj);

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
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pRenderer->Render();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOriginView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOriginProj);
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
	m_pRenderer->SetRenderInfo(Engine::RENDER_UI, dynamic_cast<Engine::CVIBuffer*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, L"Buffer_RcTex")), static_cast<Engine::CTexture*>(Engine::GetOriResource(Engine::RESOURCE_STATIC, _pTextureTag)));
}
