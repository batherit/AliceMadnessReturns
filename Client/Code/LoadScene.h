#ifndef LoadScene_h__
#define LoadScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "Loading.h"

BEGIN(Client)

class CLoadScene : public Engine::CScene
{
private:
	explicit CLoadScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadScene(void);

public:
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	CLoading*	m_pLoading = nullptr;

public:
	static CLoadScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END
#endif // Logo_h__
