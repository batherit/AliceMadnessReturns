#ifndef EffectMgr_h__
#define EffectMgr_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CEffectMgr : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CEffectMgr(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CChronoTimer* _pTimer);
	explicit  CEffectMgr(const  CEffectMgr& rhs);
	virtual ~CEffectMgr(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CEffectMgr*	Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CChronoTimer* _pTimer);
	virtual void		Free(void);
	void SetTimeSpeed(_float _fDeltaMultiple, _float _fKeepTime = 0.f);

private:
	Engine::CChronoTimer* m_pTimer = nullptr;
	_float m_fKeepTime = 0.f;
	_bool m_bIsTimeSpeed = false;
};

END

#endif // !LogoObject_h__