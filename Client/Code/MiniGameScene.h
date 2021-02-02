#ifndef MiniGameScene_h__
#define MiniGameScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//END
BEGIN(Client)
class CMap;
class CShip;
class CUI_HPBar;
class CUI_FadeInOut;
class CUI_ToothShip;
class CUI_ShipProgress;
class CMiniGameScene : public Engine::CScene
{
#define MINIGAME_TIME 60.f
private: // 생성자, 소멸자
	explicit CMiniGameScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMiniGameScene(const CMiniGameScene& rhs);
	virtual ~CMiniGameScene(void);

public:
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	//virtual void Render(void);

	virtual void OnLoaded() override;
	virtual void OnExited() override;

public:
	static CMiniGameScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

private:
	CMap* m_pMap = nullptr;
	Engine::CLayer* m_pLayer = nullptr;
	Engine::CCamera* m_pCamera = nullptr;
	CShip* m_pShip = nullptr;
	CUI_HPBar* m_pHPBar = nullptr;
	CUI_ShipProgress* m_pShipProgress = nullptr;
	CUI_ToothShip* m_pToothShip = nullptr;
	CUI_FadeInOut* m_pFadeInOut = nullptr;

	_float m_fGenTime_Shark = 0.f;
	_float m_fGenTime_Mine = 0.f;
	_float m_fGameTime = MINIGAME_TIME;
	_bool m_bIsGameOver = false;
	//CMonster* m_pMonster = nullptr;
	//CSkyBox* m_pSkyBox = nullptr;
	//CTerrain* m_pTerrain = nullptr;
	//_float m_fFogSphereDensity = 1.f;
	//_float m_fFogHeightDensity = 1.f;
};

END
#endif // MainApp_h__



//virtual void Ready(void) override;
//virtual void LateUpdate(void) override;
//virtual void Render(CCamera* _pCamera) override;
//virtual void Release(void) override;
//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);