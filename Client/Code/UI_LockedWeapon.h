#ifndef UI_LockedWeapon_h__
#define UI_LockedWeapon_h__

#include "Define.h"
#include "Base.h"
#include "Engine_Define.h"
#include "Export_Function.h"

BEGIN(Client)
class CAliceW;
class CUI_Image;
template <typename T> class CUI_Button;
class CUI_LockedWeapon : public Engine::CGameObject
{
private: // 생성자, 소멸자
	explicit  CUI_LockedWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit  CUI_LockedWeapon(const  CUI_LockedWeapon& rhs);
	virtual ~CUI_LockedWeapon(void);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Object(void) override;
	virtual int Update_Object(const _float & _fDeltaTime) override;
	virtual void Render_Object(void) override;

public:
	static CUI_LockedWeapon*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void		Free(void);

public:
	void SetPlayer(CAliceW* _pPlayer);
	void SetWeaponTexture(const _tchar* _pTextureTag);
	void SetPos(LONG _iPosX, LONG _iPosY);
	void SetLocked(_bool _bIsLocked);
	_bool IsLocked() const { return m_bIsLocked; }
	void SetToothNum(_int _iToothNum) { m_iToothNum = _iToothNum; }
	_int GetToothNum() const { return m_iToothNum; }

private:
	void ClickUnlockButton(void*);

private:
	Engine::CRenderer* m_pRenderer = nullptr;

	CAliceW* m_pAliceW = nullptr;

	_bool m_bIsLocked = true;
	CUI_Image* m_pWeaponImage = nullptr;
	CUI_Image* m_pLockImage = nullptr;
	CUI_Button<CUI_LockedWeapon>* m_pUnlockButton = nullptr;
	CUI_Image* m_pToothImage = nullptr;
	_int m_iToothNum = 0;
};

END

#endif // !LogoObject_h__