#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)

class CDynamicCamera : public Engine::CCamera
{
private: // 생성자, 소멸자
	explicit  CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CDynamicCamera(const  CDynamicCamera& rhs);
	virtual ~CDynamicCamera(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;

public:
	static CDynamicCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

private:
	Engine::CMoveComponent* m_pMoveComponent = nullptr;
};

END

#endif // !DynamicCamera_h__
