#ifndef PlateEffect_h__
#define PlateEffect_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CPlateEffect : public Engine::CGameObject
{
public:
	enum E_DESTROY_TYPE { DESTROY_INVALID, DESTROY_UNACTIVATED, DESTROY_END };

private: // 생성자, 소멸자
	explicit  CPlateEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CPlateEffect(const  CPlateEffect& rhs);
	virtual ~CPlateEffect(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CPlateEffect*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlateEffectInfo(const _tchar* _pTextureTag, const _vec3& _vPos, const _vec2& _vStartSize, const _vec2& _vEndSize, _float _fAngleZ, _float _fLifeTime, const _vec3& _vEffectColor, E_DESTROY_TYPE _eDestroyType = DESTROY_INVALID, _float _fActivationTime = 0.f, _float _fInitAlpha = 0.f);
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	Engine::CShader* m_pShader = nullptr;

	_vec2 m_vStartSize;
	_vec2 m_vEndSize;
	_float m_fActivationTime = 0.f;
	_float m_fMaxLifeTime;
	_float m_fLifeTime;
	_float m_fInitAlpha = 0.f;
	_vec4 m_vEffectColor;
	E_DESTROY_TYPE m_eDestroyType = DESTROY_INVALID;
};

END

#endif // !LogoObject_h__