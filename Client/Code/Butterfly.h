#ifndef Butterfly_h__
#define Butterfly_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CButterfly : public Engine::CGameObject
{
#define WING_SPEED_DEGREE 1080.f * 2.f
public:
	enum E_DESTROY_TYPE { DESTROY_INVALID, DESTROY_UNACTIVATED, DESTROY_END };

private: // 생성자, 소멸자
	explicit  CButterfly(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CButterfly(const  CButterfly& rhs);
	virtual ~CButterfly(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CButterfly*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetButterflyInfo(const _vec3& _vPos, const _vec3& _vDir, _float _fSpeed, _float _fSize = 1.f, _float _fLifeTime = 1.f, E_DESTROY_TYPE _eDestroyType = DESTROY_INVALID);
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

private:
	Engine::CStaticMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CShader* m_pShader = nullptr;

	_float m_fSize = 1.f;
	_float m_fMaxSize = 1.f;
	_float m_fRadian = 0.f;
	_float m_fMaxLifeTime;
	_float m_fLifeTime;
	E_DESTROY_TYPE m_eDestroyType = DESTROY_INVALID;
};

END

#endif // !LogoObject_h__