#include "pch.h"
#include "DefaultScene.h"

CDefaultScene::CDefaultScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CDefaultScene::CDefaultScene(const CDefaultScene & rhs)
	:
	CScene(rhs)
{
}

CDefaultScene::~CDefaultScene(void)
{
}

void CDefaultScene::ResetScene(void)
{

}

HRESULT CDefaultScene::Ready(void)
{
	return S_OK;
}

int CDefaultScene::Update(const _float& fTimeDelta)
{
	return CScene::Update(fTimeDelta);
}

void CDefaultScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();
}

CDefaultScene * CDefaultScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefaultScene*	pInstance = new CDefaultScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CDefaultScene::Free(void)
{
	CScene::Free();
}
