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
class CColliderScene;
class CStaticObject;
class CDynamicObject;
class CTool3D_Kernel : public CBase
{
private: // ������, �Ҹ���
	explicit CTool3D_Kernel(void);
	virtual ~CTool3D_Kernel(void);

	// public �Լ�, ����
public:
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
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
	static CTool3D_Kernel*	Create(void);
	virtual void		Free(void);

public:
	LPDIRECT3DDEVICE9 GetGraphicDev() { return m_pGraphicDev; }
	CEditScene* GetEditScene() const;
	CColliderScene* GetColliderScene() const;
	Engine::CScene* GetCurScene() const;

	Engine::CInputProcessor* GetInputProcessor() const;
	Engine::CInputProcessorMgr* GetInputProcessorMgr() const { return m_pInputProcessorMgr; }

	// �Է� ó�� �Ŵ��� ��ü
	Engine::CInputProcessorMgr* m_pInputProcessorMgr = nullptr;
	
	// �ӽ� ���� 
	vector<CStaticObject*> m_vecStoredStaticObjects;
	vector<CDynamicObject*> m_vecStoredDynamicObjects;

	vector<CStaticObject*> m_vecStoredStaticObjects_Collider;
	vector<CDynamicObject*> m_vecStoredDynamicObjects_Collider;
};

END
#endif // MainApp_h__
