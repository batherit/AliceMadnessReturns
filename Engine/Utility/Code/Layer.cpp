#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer()
{

}

//CComponent * CLayer::Get_Component(const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
//{
//	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
//
//	if (iter == m_mapObject.end())
//		return nullptr;
//
//	return iter->second->Get_Component(pComponentTag, eID);
//}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	//m_mapObject.emplace(pObjTag, pGameObject);

	m_mapObjectList[pObjTag].emplace_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * _pGameObject)
{
	if (nullptr == _pGameObject)
		return E_FAIL;

	m_mapObjectList[OBJECTLIST_TCHAR].emplace_back(_pGameObject);

	return S_OK;
}

CLayer::LayerList& CLayer::GetLayerList(const _tchar * _pObjTag)
{
	return m_mapObjectList[_pObjTag];
}

CLayer::LayerList& CLayer::GetLayerList()
{
	return m_mapObjectList[OBJECTLIST_TCHAR];
}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

Engine::_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	// ��ȿȭ ��ü ����(���� ���Ŀ� ���������� nullptr�� �� �� �ֱ� ������, ���ʿ� ��ġ��.)
	CollectInvalidObjects();

	_int iExit = 0;

	for (auto& iter : m_mapObjectList)
	{
		for (auto& rObj : iter.second) {
			if (!rObj->IsValid())
				continue;

			iExit = rObj->Update_Object(fTimeDelta);

			if (iExit & 0x80000000)
			{
				MSG_BOX("GameObject Problem");
				return iExit;
			}
		}
	}

	return iExit;
}

void CLayer::CollectInvalidObjects()
{
	for (auto& iter : m_mapObjectList)
	{
		for (auto& rObj : iter.second) {
			// ��ȿȭ ��ü ����(�θ� ���� => �ֻ�����ü�̴�. ������ü�� ���Ŵ� �ֻ�����ü�� ���ŵǸ鼭 �� ���� ������ ���� ó����.)
			if (!rObj->GetParent() && !rObj->IsValid()) {
				Safe_Release(rObj);
			}
		}

		iter.second.remove_if([](auto& rObj)
		{
			return rObj == nullptr;
		});
	}
}

Engine::CLayer* Engine::CLayer::Create(void)
{
	CLayer*		pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& iter : m_mapObjectList) {
		for_each(iter.second.begin(), iter.second.end(), CDeleteObj());
	}
	m_mapObjectList.clear();
}

