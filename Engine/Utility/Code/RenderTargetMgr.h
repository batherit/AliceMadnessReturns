#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Define.h"
#include "Base.h"

#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(void);
	virtual ~CRenderTargetMgr(void);

public:
	HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pTargetTag, const _uint& iWidth,
		const _uint& iHeight,
		D3DFORMAT Format,
		D3DXCOLOR Color);

	HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT		Begin_MRT(const _tchar* pMRTTag);
	HRESULT		End_MRT(const _tchar* pMRTTag);

	HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void		Render_DebugBuffer(const _tchar* pMRTTag);

	void		Throw_RenderTargetTexture(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantName);

private:
	CRenderTarget*			Find_RenderTarget(const _tchar* pTargetTag);
	list<CRenderTarget*>*	Find_MRT(const _tchar* pMRTTag);

private:
	map<const _tchar*, CRenderTarget*>			m_mapRenderTarget;
	map<const _tchar*, list<CRenderTarget*>>	m_mapMRT;

private:
	virtual void Free(void);
};

END
#endif // RenderTargetMgr_h__
