#ifndef AliceW_h__
#define AliceW_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CMap;
class CAttribute;
class CUI_InGame;
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
	void SetSuperJumped(const _bool& _bIsSuperJumped) { m_bIsSuperJumped = _bIsSuperJumped; }
	void SetDashing(const _bool& _bIsDashing) { m_bIsDashing = _bIsDashing; }

	_bool IsMoving(const _float& _fDeltaTime, _vec3* _pDir = nullptr);
	_bool IsSmalling();
	_bool IsSmallingTunnel() const { return m_bIsSmalling; }
	_bool IsAttackOn(const _float& _fDeltaTime);
	_bool IsAttacking(const _float& _fDeltaTime);
	_bool IsGunModeReleased();
	_bool IsSuperJumpOn() const { return m_bIsSuperJumped; }
	_bool IsJumpOn(const _float& _fDeltaTime);
	_bool IsDashOn();
	_bool IsDashing();
	_bool IsSliding(const _float& _fDeltaTime);
	_bool IsFloatingOn(const _float& _fDeltaTime);
	_bool IsFalling(const _float& _fDeltaTime);
	_bool IsRunOn(const _float&, _vec3* _pDir = nullptr);
	_bool IsLockOn() const { return m_bIsLockOn; }
	_bool IsDead() const;
	_bool IsLanded() const { return m_bIsLanded; }
	_bool IsWeaponChanging();
	_bool IsBombOn();
	CMap* GetMap() const { return m_pMap; }
	_int GetCellIndex() const { return m_iCellIndex; }

	Engine::CGameObject* GetWeapon() const { return m_pWeapons[m_eWeaponType]; }
	Engine::CColliderObject* GetAttackCollider() const { return m_pAttackColliders[m_eWeaponType]; }
	//Engine::CColliderObject* GetHead() const { return m_pHead; }
	E_WEAPON_TYPE GetWeaponType() const { return m_eWeaponType; }

	void ToggleLockOn();		// CAPS_LOCK 버튼을 눌렀을때
	void ReleaseLockOn();		// 락온 해제
	_bool ChangeLockOnTarget();	// TAB버튼을 눌렀을때 (락온이 켜져있을때 유효) // 타겟 대상이 바뀌면 true반환
	Engine::CGameObject* GetTargetObject() const { return m_pTargetObject; }

	void SetToothNum(_int _iToothNum) { m_iToothNum = _iToothNum; }
	void IncreaseTooth(_int _iAmount) { m_iToothNum += _iAmount; }
	void DecreaseTooth(_int _iAmount) { m_iToothNum -= _iAmount; if (_iAmount < 0) _iAmount = 0; }
	_int GetToothNum() const { return m_iToothNum; }

	void SetInGameUI(CUI_InGame* _pInGameUI) { m_pInGameUI = _pInGameUI; }

private:
	CMap* m_pMap = nullptr;

	Engine::CGameObject* m_pWeapons[E_WEAPON_TYPE::TYPE_END] = { nullptr, };
	Engine::CColliderObject* m_pAttackColliders[E_WEAPON_TYPE::TYPE_END] = { nullptr, };
	//Engine::CColliderObject* m_pHead = nullptr;
	E_WEAPON_TYPE m_eWeaponType = TYPE_END;
	CStateMgr<CAliceW>* m_pStateMgr = nullptr;
	Engine::CDynamicMesh* m_pMesh = nullptr;
	Engine::CMeshRenderer* m_pRenderer = nullptr;
	Engine::CShader* m_pShader = nullptr;
	Engine::CPhysics* m_pPhysics = nullptr;
	CAttribute* m_pAttribute = nullptr;
	_int m_iCellIndex = -1;
	_bool m_bIsLanded = false;

	_bool m_bIsLockOn = false;
	Engine::CGameObject* m_pTargetObject = nullptr;
	_bool m_bIsSmalled = false;
	_bool m_bIsSmalling = false;
	_bool m_bIsSuperJumped = false;
	_bool m_bIsDashing = false;

	_int m_iToothNum = 100;

	CUI_InGame* m_pInGameUI = nullptr;
};

END

#endif // !LogoObject_h__
