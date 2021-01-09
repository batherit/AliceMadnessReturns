#ifndef TitleScene_h__
#define TitleScene_h__

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
class CTitleScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CTitleScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTitleScene(const CTitleScene& rhs);
	virtual ~CTitleScene(void);

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
	static CTitleScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	//HRESULT		Ready_LightInfo();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	//HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

private:
	// button event
	void HoveredOnStartButton(void*);
	void UnhoveredOnStartButton(void*);
	void ClickStartButton(void*);
	void HoveredOnExitButton(void*);
	void UnhoveredOnExitButton(void*);
	void ClickQuitButton(void*);

private:
	_bool m_bIsOnStartButtonHovered = false;
	CUI_Image* m_pStartButtonOn = nullptr;
	CUI_Image* m_pStartButtonOff = nullptr;
	CUI_Button<CTitleScene>* m_pStartButton = nullptr;

	_bool m_bIsOnExitButtonHovered = false;
	CUI_Image* m_pExitButtonOn = nullptr;
	CUI_Image* m_pExitButtonOff = nullptr;
	CUI_Button<CTitleScene>* m_pExitButton = nullptr;

	CUI_Cursor* m_pCursorUI = nullptr;

	CUI_FadeInOut* m_pFadeInOutUI = nullptr;
};

END
#endif // MainApp_h__



//virtual void Ready(void) override;
//virtual void LateUpdate(void) override;
//virtual void Render(CCamera* _pCamera) override;
//virtual void Release(void) override;
//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);