#ifndef Resources_h__
#define Resources_h__

#include "Engine_Define.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResources(const CResources& rhs);
	virtual ~CResources(void);

public:
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Ready(void) override { return S_OK; };
	virtual int Update(const _float & _fDeltaTime) override { return 0; };
	static const _tchar* GetComponentTag() { return L"Resources"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

public:
	virtual void		Free(void);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
};

END
#endif // Resources_h__
