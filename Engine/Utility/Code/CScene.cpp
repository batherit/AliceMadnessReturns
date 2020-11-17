#include "CScene.h"
#include "Renderer.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CScene::CScene(const CScene & rhs)
	:
	m_pGraphicDev(rhs.m_pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
}

Engine::CScene::~CScene()
{
	//Free();
}

//CComponent * CScene::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
//{
//	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
//
//	if (iter == m_mapLayer.end())
//		return nullptr;
//
//	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
//}

_int CScene::Update(const _float & _fDeltaTime)
{
	_int iExit = 0;

	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(_fDeltaTime);

		if (iExit & 0x80000000)
		{
			MSG_BOX("Layer Problem");
			return iExit;
		}
	}

	return iExit;
}

//void CScene::Render(void)
//{
//	//CRenderer::Render();
//	//for (auto& iter : m_mapLayer)
//	//	iter.second->Render_Layer();
//}

void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
