#ifndef Tool3D_Kernel_h__
#define Tool3D_Kernel_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;

END
BEGIN(Client)
class CEditScene;
class CTool3D_Kernel : public CBase
{
private: // 생성자, 소멸자
	explicit CTool3D_Kernel(void);
	virtual ~CTool3D_Kernel(void);

	// public 함수, 변수
public:
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
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
	static CTool3D_Kernel*	Create(void);
	virtual void		Free(void);

public:
	CEditScene* GetEditScene() const;

};

END
#endif // MainApp_h__
