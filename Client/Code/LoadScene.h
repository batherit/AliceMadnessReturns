#ifndef LoadScene_h__
#define LoadScene_h__

#include "Scene.h"
#include "Define.h"

#include "BackGround.h"
#include "TestPlayer.h"
#include "TestMonster.h"
#include "Loading.h"

BEGIN(Client)

class CLoadScene : public Engine::CScene
{
private:
	explicit CLoadScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadScene(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
		
private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag);

	HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	CLoading*	m_pLoading = nullptr;

public:
	static CLoadScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

END
#endif // Logo_h__
