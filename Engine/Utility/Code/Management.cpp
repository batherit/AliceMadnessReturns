//#include "..\..\..\Reference\Header\Management.h"
//#include "Management.h"
#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	:
	m_pSceneMgr(CSceneMgr::Create())
{
}

CManagement::~CManagement(void)
{
	Free();
}

//CComponent * CManagement::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
//{
//	if (nullptr == m_pSceneMgr->GetCurScene())
//		return nullptr;
//
//	return m_pSceneMgr->GetCurScene()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
//}

void CManagement::SetNextScene(CScene * _pNextScene)
{
	m_pSceneMgr->SetNextScene(_pNextScene);
}

//HRESULT CManagement::Ready_Shader(LPDIRECT3DDEVICE9 & _pGraphicDev)
//{
//	CShader* pShader = nullptr;
//
//	// Sample
//	pShader = CShader::Create(_pGraphicDev, L"../../Reference/Header/Shader_Sample.hpp");
//	NULL_CHECK_RETURN(pShader, E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Sample", pShader), E_FAIL);
//
//	// Terrain
//	pShader = CShader::Create(_pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
//	NULL_CHECK_RETURN(pShader, E_FAIL);
//	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Shader_Terrain", pShader), E_FAIL);
//
//	return S_OK;
//}

_bool CManagement::ConfirmValidScene()
{
	return m_pSceneMgr->ConfirmValidScene();
}

void CManagement::RequestSceneInit(void)
{
	m_pSceneMgr->RequestSceneInit();
}

void CManagement::UpdateScene(const _float & _fDeltaTime)
{
	m_pSceneMgr->Update(_fDeltaTime);
}

void CManagement::RenderScene(void)
{	
	m_pSceneMgr->Render();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pSceneMgr);
}
