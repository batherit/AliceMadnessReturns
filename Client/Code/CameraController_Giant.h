#ifndef CameraController_Giant_h__
#define CameraController_Giant_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CCameraController_Giant : public Engine::CCameraController
{
protected:
	explicit CCameraController_Giant(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCameraController_Giant(void);

public:
	virtual HRESULT			Ready_Object(void) { return S_OK; }
	virtual _int			Update_Object(const _float& fTimeDelta) { return 0; }

	// 매니져에서 업데이트마다 호출됩니다.
	virtual void ControlCamera(const _float& _fDeltaTime, Engine::CCamera* _pCamera, const _float& _fShiftFactor) override;
	static CCameraController_Giant* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void SetTargetObject(Engine::CGameObject* _pTargetObject) { m_pTargetObject = _pTargetObject; }
	void SetBenchmarkObject(Engine::CGameObject* _pBenchmarkObject) { m_pBenchmarkObject = _pBenchmarkObject; }

protected:
	virtual void Free(void);

protected:
	Engine::CGameObject* m_pTargetObject = nullptr;
	Engine::CGameObject* m_pBenchmarkObject = nullptr;
};


END
#endif // CameraController_Giant_h__
