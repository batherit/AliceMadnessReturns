#ifndef PlateEffect_h__
#define PlateEffect_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CPlateEffect : public Engine::CGameObject
{
	enum E_DESTROY_TYPE { DESTROY_UNVALID, DESTROY_UNACTIVATED, DESTROY_END };

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
	void SetPlateEffectInfo(const _tchar* _pTextureTag, const _vec3& _vPos, const _vec2& _vStartSize, const _vec2& _vEndSize, _float _fAngleZ, _float _fLifeTime, E_DESTROY_TYPE _eDestroyType = DESTROY_UNVALID);

private:
	Engine::CPolygonRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;

	_vec2 m_vStartSize;
	_vec2 m_vEndSize;
	_float m_fMaxLifeTime;
	_float m_fLifeTime;
	E_DESTROY_TYPE m_eDestroyType = DESTROY_UNVALID;
};

END

#endif // !LogoObject_h__