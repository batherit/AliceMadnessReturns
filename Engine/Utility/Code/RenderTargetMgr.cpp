#include "RenderTargetMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTargetMgr)

Engine::CRenderTargetMgr::CRenderTargetMgr(void)
{

}

Engine::CRenderTargetMgr::~CRenderTargetMgr(void)
{
	Free();
}

HRESULT Engine::CRenderTargetMgr::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget*	 pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iWidth, iHeight, Format, Color);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_mapRenderTarget.emplace(pTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>	MRTList;
		MRTList.push_back(pRenderTarget);

		m_mapMRT.emplace(pMRTTag, MRTList);
	}

	else
	{
		pMRTList->push_back(pRenderTarget);
	}

	return S_OK;
}

HRESULT CRenderTargetMgr::Begin_MRT(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& iter : *pMRTList)
		iter->Clear_RenderTarget();

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
	{
		iter->SetUp_OnGraphicDev(iIndex);
		iIndex++;
	}


	return S_OK;
}

HRESULT CRenderTargetMgr::End_MRT(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
	{
		iter->Release_OnGraphicDev(iIndex);
		iIndex++;
	}


	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_DebugBuffer(const _tchar * pTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	FAILED_CHECK_RETURN(pRenderTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY), E_FAIL);

	return S_OK;
}

void CRenderTargetMgr::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
		return;

	for (auto& iter : *pMRTList)
		iter->Render_DebugBuffer();

}

void Engine::CRenderTargetMgr::Throw_RenderTargetTexture(LPD3DXEFFECT & pEffect, const _tchar * pTargetTag, const char * pConstantName)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pTargetTag);
	NULL_CHECK(pRenderTarget);

	pRenderTarget->Throw_RenderTargetTexture(pEffect, pConstantName);
}

CRenderTarget* Engine::CRenderTargetMgr::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTargetMgr::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void Engine::CRenderTargetMgr::Free(void)
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CDeleteMap());
	m_mapRenderTarget.clear();

	for (auto& iter : m_mapMRT)
		iter.second.clear();

	m_mapMRT.clear();
}

