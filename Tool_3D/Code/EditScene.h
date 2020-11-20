#ifndef DefaultScene_h__
#define DefaultScene_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

//END
BEGIN(Client)
class CEditScene : public Engine::CScene
{
private: // 생성자, 소멸자
	explicit CEditScene(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEditScene(const CEditScene& rhs);
	virtual ~CEditScene(void);

public:
	// CScene을(를) 통해 상속됨
	virtual void ResetScene(void) override;
	virtual HRESULT Ready(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CEditScene*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);

public:
	Engine::CGameObject* GetPickedObject() const;

private:
	_vec3 m_vObjectPostion{ 0.f, 0.f, 0.f };
	_vec3 m_vObjectRotation{ 0.f, 0.f, 0.f };
};

END
#endif // MainApp_h__