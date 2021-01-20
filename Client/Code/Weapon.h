#ifndef Weapon_h__
#define Weapon_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"
#include "StaticObject.h"

BEGIN(Client)

class CWeapon : public CStaticObject
{
protected: // ������, �Ҹ���
	explicit  CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CWeapon(const  CWeapon& rhs);
	virtual ~CWeapon(void);

	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	virtual void OnCollision(Engine::CollisionInfo) override;

public:
	static CWeapon*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetWeaponInfo(const _tchar* _tcWeaponTag, const _tchar* _tcAttackColliderTag, _float _fDamage);
	Engine::CColliderObject* GetAttackCollider() const { return m_pAttackCollider; }
	
	void SetDamage(_float _fDamage) { m_fDamage = _fDamage; }
	_float GetDamage() const { return m_fDamage; }

private:
	Engine::CColliderObject* m_pAttackCollider = nullptr;
	_float m_fDamage = 0.f;
};

END

#endif // !LogoObject_h__
