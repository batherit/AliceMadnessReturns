#include "pch.h"
#include "LoadScene.h"
#include "Export_Function.h"
#include "EditScene.h"
#include "MainFrm.h"
#include "CTabForm.h"
#include "CMapTab.h"


CLoadScene::CLoadScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLoadScene::~CLoadScene(void)
{

}

void CLoadScene::ResetScene(void)
{
}


HRESULT CLoadScene::Ready(void)
{
	m_pLoading = CLoading::Create(m_pGraphicDev, CLoading::LOADING_STAGE);
	FAILED_CHECK_RETURN(m_pLoading, E_FAIL);


	return S_OK;
}

Engine::_int CLoadScene::Update(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		//UpdateObjectList();
		
		Engine::CManagement::GetInstance()->SetNextScene(CEditScene::Create(m_pGraphicDev));

		return iExit;

	}

	return iExit;
}

void CLoadScene::Render(void)
{
	// DEBUG ¿ë
	Engine::Render_Font(L"Font_Jinji", m_pLoading->Get_LoadString(), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}


CLoadScene* CLoadScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadScene*	pInstance = new CLoadScene(pGraphicDev);

	if (FAILED(pInstance->Ready()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CLoadScene::Free(void)
{
	Safe_Release(m_pLoading);

	Engine::CScene::Free();
}

