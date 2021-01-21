#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResources(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
	: CResources(rhs)
{
	_uint	iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;

	for (_uint i = 0; i < iContainerSize; ++i)
		Safe_AddRef(m_vecTexture[i].pTexture);

}

Engine::CTexture::~CTexture(void)
{

}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	//IDirect3DBaseTexture9*		pTexture = nullptr;
	TextureInfo tTextureInfo;
	
	for (_uint i = 0; i < iCnt; ++i)
	{
		_tchar		szFileName[256] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEX_NORMAL:
			if (FAILED(D3DXGetImageInfoFromFile(szFileName, &tTextureInfo.tImageInfo)))
				return E_FAIL;
			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&tTextureInfo.pTexture)))
				return E_FAIL;
			break;

		case TEX_CUBE:
			if (FAILED(D3DXGetImageInfoFromFile(szFileName, &tTextureInfo.tImageInfo)))
				return E_FAIL;
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&tTextureInfo.pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(tTextureInfo);
	
	}

	return S_OK;
}

void Engine::CTexture::Render_Texture(const _uint& iIndex /*= 0*/)
{
	if (m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex].pTexture);
}

void CTexture::Set_Texture(LPD3DXEFFECT & pEffect, const char * pConstantName, const _uint & iIndex)
{
	if (m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pConstantName, m_vecTexture[iIndex].pTexture);
	pEffect->CommitChanges();
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
{
	CTexture*	pInstance = new CTexture(pGraphicDev);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTexture::Clone(void)
{
	return new CTexture(*this);
}

void Engine::CTexture::Free(void)
{
	CResources::Free();

	//for_each(m_vecTexture.begin(), m_vecTexture.end(), CDeleteObj());
	for (auto info : m_vecTexture) {
		Safe_Release(info.pTexture);
	}

	m_vecTexture.clear();
}

