#ifndef CreditScene_h__
#define CreditScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//Credit
BEGIN(Client)
class CUI_Image;
class CCreditScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CCreditScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCreditScene(const CCreditScene& rhs);
	virtual ~CCreditScene(void);

public:
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render() override;

	virtual void OnLoaded() override;
	virtual void OnExited() override;

public:
	static CCreditScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	//HRESULT		Ready_LightInfo();
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	//HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

	CUI_Image* m_pBackground = nullptr;
	CUI_Image* m_pFin = nullptr;
};

END
#endif // MainApp_h__



//virtual void Ready(void) override;
//virtual void LateUpdate(void) override;
//virtual void RCrediter(CCamera* _pCamera) override;
//virtual void Release(void) override;
//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);