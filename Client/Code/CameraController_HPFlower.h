#ifndef CameraController_HPFlower_h__
#define CameraController_HPFlower_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CCameraController_HPFlower : public Engine::CCameraController
{
protected:
	explicit CCameraController_HPFlower(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraController_HPFlower(void);

public:
	virtual HRESULT			Ready_Object(void) { return S_OK; }
	virtual _int			Update_Object(const _float& fTimeDelta) { return 0; }

	// 매니져에서 업데이트마다 호출됩니다.
	virtual void ControlCamera(const _float& _fDeltaTime, Engine::CCamera* _pCamera, const _float& _fShiftFactor) override;

	void SetPlayer(const Engine::CGameObject* _pPlayer) { m_pPlayer = _pPlayer; }

	static CCameraController_HPFlower* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void);

protected:
	const Engine::CGameObject* m_pPlayer = nullptr;
};


END
#endif // CameraController_HPFlower_h__
