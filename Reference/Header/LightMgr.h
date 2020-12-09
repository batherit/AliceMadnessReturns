#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	const D3DLIGHT9*		Get_Light(const _uint& iIndex);

public:
	HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	list<CLight*>				m_LightList;

private:
	virtual void Free(void);
};

END
#endif // LightMgr_h__
