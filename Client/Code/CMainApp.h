#ifndef MainApp_h__
#define MainApp_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;

END
BEGIN(Client)

class CMainApp : public CBase
{
private: // 생성자, 소멸자
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public 함수, 변수
public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);
	// protected 함수, 변수
	// private 함수, 변수
private:
	Engine::CGraphicDev*		m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	// 생성, 소멸 함수
public:
	static CMainApp*	Create(void);
	virtual void		Free(void);

};

END
#endif // MainApp_h__
