#ifndef AliceW_h__
#define AliceW_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CAttribute;
template<typename T> class CStateMgr;
class CAliceW : public Engine::CGameObject
{
public:
	enum E_WEAPON_TYPE { TYPE_BLADE, TYPE_HORSE, TYPE_GUN, TYPE_END };
private: // 생성자, 소멸자
	explicit  CAliceW(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CAliceW(const  CAliceW& rhs);
	virtual ~CAliceW(void);

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;
	_bool LoadColliders(const _tchar* _pFileName);
	virtual void OnCollision(Engine::CollisionInfo _tCollisionInfo) override;
	virtual void OnNotCollision(Engine::CollisionInfo _tCollisionInfo) override;

public:
	static CAliceW*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	CStateMgr<CAliceW>* GetStateMgr() const { return m_pStateMgr; }
	Engine::CDynamicMesh* GetDynamicMesh() const { return m_pMesh; }
	Engine::CPhysics* GetPhysics() const { return m_pPhysics; }
	CAttribute* GetAttribute() const { return m_pAttribute; }

	void SetLanded(const _bool& _bIsLanded) { m_bIsLanded = _bIsLanded; }
	void SetWeaponType(E_WEAPON_TYPE _eWeaponType);

	_bool IsMoving(const _float& _fDeltaTime, _vec3* _pDir = nullptr);
	_bool IsAttackOn(const _float& _fDeltaTime);
	_bool IsAttacking(const _float& _fDeltaTime);
	_bool IsGunModeReleased();
	_bool IsJumpOn(const _float& _fDeltaTime);
	_bool IsSliding(const _float& _fDeltaTime);
	_bool IsFloatingOn(const _float& _fDeltaTime);
	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	_bool IsLockOn() const { return m_bIsLockOn; }
	_bool IsDead() const;
	_bool IsLanded() const { return m_bIsLanded; }
	_bool IsWeaponChanging();
	CMap* GetMap() const { return m_pMap; }
	_int GetCellIndex() const { return m_iCellIndex; } 

	Engine::CGameObject* GetWeapon() const { return m_pWeapons[m_eWeaponType]; }
	E_WEAPON_TYPE GetWeaponType() const { return m_eWeaponType; }

	void ToggleLockOn();		// CAPS_LOCK 버튼을 눌렀을때
	void ReleaseLockOn();		// 락온 해제
	_bool ChangeLockOnTarget();	// TAB버튼을 눌렀을때 (락온이 켜져있을때 유효) // 타겟 대상이 바뀌면 true반환
	Engine::CGameObject* GetTargetObject() const { return m_pTargetObject; }

private:
	CMap* m_pMap = nullptr;

	Engine::CGameObject* m_pWeapons[E_WEAPON_TYPE::TYPE_END] = { nullptr, };
	E_WEAPON_TYPE m_eWeaponType = TYPE_END;
	CStateMgr<CAliceW>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CAttribute* m_pAttribute = nullptr;
	_int m_iCellIndex = -1;
	_bool m_bIsLanded = false;

	_bool m_bIsLockOn = false;
	Engine::CGameObject* m_pTargetObject = nullptr;
};

END

#endif // !LogoObject_h__
