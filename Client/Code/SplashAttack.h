#ifndef SplashAttack_h__
#define SplashAttack_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "Attribute.h"

BEGIN(Client)
class CAttribute;
class CSplashAttack : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CSplashAttack(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CSplashAttack(const  CSplashAttack& rhs);
	virtual ~CSplashAttack(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo);
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo);

public:
	static CSplashAttack*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetSplashAttackInfo(const _tchar* _pColliderTag, const _tchar* _pTargetTag, const _vec3& _vPos, _float m_fDamage, _float _fRadius, _float _fLifeTime, const _vec3& _vAttackDirXZ, _float _fSplashRangeDegree);
	_bool IsInRange(Engine::CGameObject* _pTargetObject);

private:
	// 충돌 중복 방지를 위한 컴포넌트
	CAttribute* m_pAttribute = nullptr;

	// 충돌체 세팅
	Engine::CColliderObject_Sphere* m_pAttackCollider = nullptr;

	// 타겟 태그(공격 대상의 태그)
	_tchar m_tcTargetTag[MAX_PATH] = L"NULL";

	// 데미지
	_float m_fDamage = 0.f;
	_float m_fLifeTime = 1.f;
	_vec3 m_vAttackDirXZ = _vec3(1.f, 0.f, 0.f);
	_float m_fSplashRangeDegree = 1.f;
};

END

#endif // !LogoObject_h__