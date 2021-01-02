#include "pch.h"
#include "SkyBox.h"
#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CSkyBox::~CSkyBox(void)
{

}


CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox*	pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CSkyBox::Free(void)
{
	Engine::CGameObject::Free();
}


HRESULT Client::CSkyBox::Ready_Object(void)
{

	m_pCubeTex = dynamic_cast<Engine::CCubeTex*>(Engine::Clone(Engine::RESOURCE_STATIC, L"Buffer_CubeTex"));
	m_mapComponent[Engine::CCubeTex::GetComponentID()].emplace(Engine::CCubeTex::GetComponentTag(), m_pCubeTex);


	m_pTexture = dynamic_cast<Engine::CTexture*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Texture_SkyBox"));
	m_mapComponent[Engine::CTexture::GetComponentID()].emplace(Engine::CTexture::GetComponentTag(), m_pTexture);

	m_pRenderer = AddComponent<Engine::CPolygonRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_PRIORITY, m_pCubeTex, m_pTexture);
	m_pRenderer->SetTextureIndex(3);
	//m_pRenderer

	return S_OK;
}
Client::_int Client::CSkyBox::Update_Object(const _float& fTimeDelta)
{
	

	//m_pRenderer->Add_RenderGroup(Engine::RENDER_PRIORITY, this);
	m_pRenderer->Update(fTimeDelta);

	return 0;
}
void Client::CSkyBox::Render_Object(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pTexture->Render_Texture(3);
	//m_pCubeTex->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_matrix	matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	// 카메라 월드 행렬
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	m_pTransform->SetPos(matCamWorld._41, matCamWorld._42, matCamWorld._43);
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());
	m_pRenderer->Render();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}
