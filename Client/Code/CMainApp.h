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
private: // ������, �Ҹ���
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public �Լ�, ����
public:
	virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);
	// protected �Լ�, ����
	// private �Լ�, ����
private:
	Engine::CGraphicDev*		m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	// ����, �Ҹ� �Լ�
public:
	static CMainApp*	Create(void);
	virtual void		Free(void);

};

END
#endif // MainApp_h__
