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

// Renderer
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

void Release_Utility(void)
{
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
}