#ifndef PlayScene_h__
#define PlayScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//BEGIN(Engine)
//
//class CGraphicDev;

//END
BEGIN(Client)

class CPlayScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CPlayScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayScene(const CPlayScene& rhs);
	virtual ~CPlayScene(void);

public:
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);


	//virtual void Ready(void) override;
	//virtual void LateUpdate(void) override;
	//virtual void Render(CCamera* _pCamera) override;
	//virtual void Release(void) override;
	//virtual LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
public:
	static CPlayScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	//_vec3 m_vScale{ 50.f, 50.f, 1.f };
	//_vec3 m_vRot{ 0.f, 0.f, 0.f };
	//_vec3 m_vPos{ 0.f, 0.f, 50.f };
	Engine::CGameObject* m_pLogoObject = nullptr;
	Engine::CGameObject* m_pMonster = nullptr;
};

END
#endif // MainApp_h__