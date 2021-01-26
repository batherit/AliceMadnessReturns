#ifndef EFT_FloatingEffect_h__
#define EFT_FloatingEffect_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CEFT_FloatingEffect : public Engine::CGameObject
{
#define FLOATINGEFFECT_LIFETIME 1.8f
#define FLOATINGEFFECT_COLOR	_vec4(1.f, 0.75f, 0.8f, 1.f)
#define FLOATINGEFFECT_ROT_DEGREE_SPEED	360.f
public:
	enum E_DESTROY_TYPE { DESTROY_INVALID, DESTROY_UNACTIVATED, DESTROY_END };

private: // 생성자, 소멸자
	explicit  CEFT_FloatingEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_FloatingEffect(const  CEFT_FloatingEffect& rhs);
	virtual ~CEFT_FloatingEffect(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_FloatingEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetFloatingEffectInfo(Engine::CGameObject* _pTargetObject, E_DESTROY_TYPE _eDestroyType = DESTROY_INVALID);
	//Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	//Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CShader* m_pShader = nullptr;

	Engine::CGameObject* m_pTargetObject = nullptr;
	_float m_fRadian = 0.f;
	_float m_fLifeTime = 0.f;
	_float m_fSign = 1.f;
	E_DESTROY_TYPE m_eDestroyType = DESTROY_INVALID;
};

END

#endif // !LogoObject_h__