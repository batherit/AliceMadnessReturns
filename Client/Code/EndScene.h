#ifndef EndScene_h__
#define EndScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//END
BEGIN(Client)
template<typename T> class CUI_Button;
class CUI_FadeInOut;
class CUI_Image;
class CUI_Cursor;
class CGiantAliceW;
class CSkyBox;
class CDynamicObject;
class CEndScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CEndScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEndScene(const CEndScene& rhs);
	virtual ~CEndScene(void);

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
	static CEndScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	//HRESULT		Ready_LightInfo();
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	//HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

private:
	Engine::CCamera* m_pCamera = nullptr;
	CGiantAliceW* m_pPlayer = nullptr;
	CDynamicObject* m_pBoss = nullptr;
	CSkyBox* m_pSkyBox = nullptr;
	CUI_FadeInOut* m_pFadeInOut = nullptr;
};

END
#endif // MainApp_h__



//virtual void Ready(void) override;
//virtual void LateUpdate(void) override;
//virtual void Render(CCamera* _pCamera) override;
//virtual void Release(void) override;
//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);