#ifndef EFT_Blood_h__
#define EFT_Blood_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAnimUV;
class CEFT_Blood : public Engine::CGameObject
{
#define BLOOD_COLOR _vec4(0.5f, 0.f, 0.f, 1.f)
public:
	enum E_DESTROY_TYPE { DESTROY_INVALID, DESTROY_UNACTIVATED, DESTROY_END };

private: // 생성자, 소멸자
	explicit  CEFT_Blood(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CEFT_Blood(const  CEFT_Blood& rhs);
	virtual ~CEFT_Blood(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEFT_Blood*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	void SetBloodInfo(const _vec3& _vPos, const _vec3& _vScale = _vec3(0.4f, 0.4f, 0.4f), E_DESTROY_TYPE _eDestroyType = DESTROY_INVALID);

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CShader* m_pShader = nullptr;
	CAnimUV* m_pAnimUV = nullptr;

	_float m_fElapsedTime = 0.f;
	E_DESTROY_TYPE m_eDestroyType = DESTROY_INVALID;
};

END

#endif // !LogoObject_h__