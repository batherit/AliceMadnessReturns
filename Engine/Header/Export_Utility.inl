HRESULT Create_Management(CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}
CComponent*	   Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

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

// Renderer
//CRenderer*		Get_Renderer(void)
//{
//	return CRenderer::GetInstance();
//}

void Release_Utility(void)
{
	//CRenderer::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}