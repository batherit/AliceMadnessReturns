#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)

Engine::CResourcesMgr::CResourcesMgr(void)
	: m_pmapResource(nullptr)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pmapResource)
		return E_FAIL;

	m_wSize = wSize;

	m_pmapResource = new map<const _tchar*, CResources*>[wSize];

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, _ulong dwCntX /*= 1*/, _ulong dwCntZ /*= 1*/, _ulong dwItv /*= 1*/)
{
	if (nullptr == m_pmapResource)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pBufferTag);
	if (nullptr != pResources)
		return E_FAIL;

	switch (eID)
	{
	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;
	case BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;
	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;
	case BUFFER_TERRAINTEX:
		pResources = CTerrain::Create(pGraphicDev);
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);
	
	m_pmapResource[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

HRESULT CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTURETYPE eType, const _tchar * pFilePath, const _uint & iCnt)
{
	if (nullptr == m_pmapResource)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResources*		pResources = Find_Resources(wContainerIdx, pTextureTag);
	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pFilePath, eType, iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pmapResource[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourcesTag)
{
	auto	iter = find_if(m_pmapResource[wContainerIdx].begin(),
		m_pmapResource[wContainerIdx].end(), CTag_Finder(pResourcesTag));

	if (iter == m_pmapResource[wContainerIdx].end())
		return nullptr;

	return iter->second;
}

void Engine::CResourcesMgr::Free(void)
{
	for (_uint i = 0; i < m_wSize; ++i)
	{
		for_each(m_pmapResource[i].begin(), m_pmapResource[i].end(), CDeleteMap());
		m_pmapResource[i].clear();
	}
	Safe_Delete_Array(m_pmapResource);
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResources*		pResources = Find_Resources(wContainerIdx, pBufferTag);
	
	if (nullptr == pResources)
		return;

	dynamic_cast<CVIBuffer*>(pResources)->Render_Buffer();
}

void CResourcesMgr::Render_Texture(const _ushort & wContainerIdx, const _tchar * pTextureTag, const _uint & iIndex)
{
	CResources*		pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr == pResources)
		return;

	dynamic_cast<CTexture*>(pResources)->Render_Texture(iIndex);
}

CComponent * CResourcesMgr::Clone(const _ushort & wContainerIdx, const _tchar * pResourcesTag)
{
	if (nullptr == m_pmapResource)
		return nullptr;

	CResources*		pProto = Find_Resources(wContainerIdx, pResourcesTag);
	NULL_CHECK_RETURN(pProto, nullptr);

	return pProto->Clone();
}

