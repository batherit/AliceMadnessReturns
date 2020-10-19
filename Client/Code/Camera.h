#ifndef Camera_h__
#define Camera_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CCamera : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CCamera(const  CCamera& rhs);
	virtual ~CCamera(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	void SetProjectionMatrix(const _float& _fFOV, const _float& _fAspect, const _float& _fNearZ, const _float& _fFarZ);

public:
	static CCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CMoveComponent* m_pMoveComponent = nullptr;
};

END

#endif // !Camera_h__
