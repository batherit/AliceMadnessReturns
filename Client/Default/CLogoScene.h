#ifndef LogoScene_h__
#define LogoScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;
//
//END
BEGIN(Client)

class CMainApp : public Engine::CScene
{
private: // ������, �Ҹ���
	explicit CMainApp(void);
	virtual ~CMainApp(void);

	// public �Լ�, ����
public:
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);
	// protected �Լ�, ����
	// private �Լ�, ����
private:
	Engine::CGraphicDev*		m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9			m_pGraphicDev = nullptr;

	_vec3 m_vScale{ 50.f, 50.f, 1.f };
	_vec3 m_vRot{ 0.f, 0.f, 0.f };
	_vec3 m_vPos{ 0.f, 0.f, 50.f };

	// ����, �Ҹ� �Լ�
public:
	static CMainApp*	Create(void);
	virtual void		Free(void);

};

END
#endif // MainApp_h__
