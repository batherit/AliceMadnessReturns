#ifndef LogoScene_h__
#define LogoScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "Camera.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//END
BEGIN(Client)

class CLogoScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CLogoScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLogoScene(const CLogoScene& rhs);
	virtual ~CLogoScene(void);

public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	
public:
	static CLogoScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

private:
	//Engine::CGameObject* m_pLogoObject = nullptr;
	//Engine::CGameObject* m_pMonster = nullptr;
	CCamera* m_pCamera = nullptr;
	Engine::CGameObject* m_pTerrain = nullptr;
};

END
#endif // MainApp_h__
	


	//virtual void Ready(void) override;
	//virtual void LateUpdate(void) override;
	//virtual void Render(CCamera* _pCamera) override;
	//virtual void Release(void) override;
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);