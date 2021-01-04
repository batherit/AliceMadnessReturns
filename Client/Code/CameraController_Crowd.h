#ifndef CameraController_Crowd_h__
#define CameraController_Crowd_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CCameraController_Crowd : public Engine::CCameraController
{
protected:
	explicit CCameraController_Crowd(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraController_Crowd(void);

public:
	virtual HRESULT			Ready_Object(void) { return S_OK; }
	virtual _int			Update_Object(const _float& fTimeDelta) { return 0; }

	// 매니져에서 업데이트마다 호출됩니다.
	virtual void ControlCamera(const _float& _fDeltaTime, Engine::CCamera* _pCamera, const _float& _fShiftFactor) override;

	void SetTarget(const Engine::CGameObject* _pTarget) { m_pTarget = _pTarget; }

	static CCameraController_Crowd* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void);

protected:
	const Engine::CGameObject* m_pTarget = nullptr;
};


END
#endif // CameraController_Crowd_h__
