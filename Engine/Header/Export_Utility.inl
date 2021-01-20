#include "Export_Utility.h"
inline CLayer * GetLayer(const _tchar * _pLayerTag)
{
	return CManagement::GetInstance()->GetSceneMgr()->GetCurScene()->GetLayer(_pLayerTag);
}
//HRESULT Create_Management(CManagement** ppManagement, LPDIRECT3DDEVICE9& pGraphicDev)
//{
//	CManagement*		pManagement = CManagement::GetInstance();
//	NULL_CHECK_RETURN(pManagement, E_FAIL);
//
//	*ppManagement = pManagement;
//
//	return S_OK;
//}
//CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
//{
//	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
//}

//HRESULT	SetUp_Scene(CScene* pScene)
//{
//	return CManagement::GetInstance()->SetUp_Scene(pScene);
//}
//_int	Update_Scene(const _float& fTimeDelta)
//{
//	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
//}
//void	Render_Scene(void)
//{
//	return CManagement::GetInstance()->Render_Scene();
//}

// protomgr
HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pInstance);
}
CComponent*		Clone(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone(pProtoTag);
}

CComponent * GetOriProto(const _tchar * _pProtoTag)
{
	return CProtoMgr::GetInstance()->GetOriProto(_pProtoTag);
}

inline const D3DLIGHT9 * Get_Light(const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}

inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9 * pLightInfo, const _uint & iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

inline void Render_Light(LPD3DXEFFECT & pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

// Renderer
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

// RenderTargetMgr
HRESULT		Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pTargetTag, const _uint& iWidth,
	const _uint& iHeight,
	D3DFORMAT Format,
	D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pGraphicDev, pTargetTag, iWidth, iHeight, Format, Color);
}

HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}

HRESULT		Begin_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}
HRESULT		End_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}

HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}

void		Render_DebugBuffer(const _tchar* pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}

inline void Throw_RenderTargetTexture(LPD3DXEFFECT & pEffect, const _tchar * pTargetTag, const char * pConstantName)
{
	CRenderTargetMgr::GetInstance()->Throw_RenderTargetTexture(pEffect, pTargetTag, pConstantName);
}

void Release_Utility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
}