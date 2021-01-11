#ifndef PoolScene_h__
#define PoolScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//END
BEGIN(Client)
class CAliceL;
class CMonster;
class CSkyBox;
class CTerrain;
class CPoolScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CPoolScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPoolScene(const CPoolScene& rhs);
	virtual ~CPoolScene(void);

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
	static CPoolScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	HRESULT		Ready_LightInfo();
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_Resource(Engine::RESOURCETYPE eType);

private:
	virtual void		Free(void);

private:
	Engine::CCamera* m_pCamera = nullptr;
	CAliceL* m_pPlayer = nullptr;
	//CMonster* m_pMonster = nullptr;
	CSkyBox* m_pSkyBox = nullptr;
	CTerrain* m_pTerrain = nullptr;
};

END
#endif // MainApp_h__



//virtual void Ready(void) override;
//virtual void LateUpdate(void) override;
//virtual void Render(CCamera* _pCamera) override;
//virtual void Release(void) override;
//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);