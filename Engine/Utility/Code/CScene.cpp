#include "CScene.h"
#include "Renderer.h"
#include "CollisionMgr.h"
#include "Export_Utility.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	m_pGraphicDev(pGraphicDev)
{
	Safe_AddRef(m_pGraphicDev);
	//CCollisionMgr::GetInstance()->ClearGameObjectList();
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

CLayer * CScene::GetLayer(const _tchar * pLayerTag) const
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

void CScene::CollectInvalidObjects()
{
	for (auto& rLayer : m_mapLayer) {
		rLayer.second->CollectInvalidObjects();
	}
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

	CCollisionMgr::GetInstance()->ProcessCollision();

	return iExit;
}

void CScene::Render(void)
{
	Engine::Get_Renderer()->Render_GameObject();
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
