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

CComponent * CManagement::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pSceneMgr->GetCurScene())
		return nullptr;

	return m_pSceneMgr->GetCurScene()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

void CManagement::SetNextScene(CScene * _pNextScene)
{
	m_pSceneMgr->SetNextScene(_pNextScene);
}

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
