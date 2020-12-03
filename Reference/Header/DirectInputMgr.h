#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CDirectInputMgr final : public CBase
{
	DECLARE_SINGLETON(CDirectInputMgr)

public:
	enum E_MOUSE_BUTTON { MB_L, MB_R, MB_M };

private:
	typedef const _tchar* KEY_STRING;
	typedef _int KEY_MACRO;
	typedef _ubyte KEY;

private:
	CDirectInputMgr();
	~CDirectInputMgr();

public:
	HRESULT Ready(HINSTANCE _hInst, HWND _hWnd);
	void Update(void);

public:
	void BindKeyStringToKey(KEY_STRING _szKeyString, KEY _iKey);

	_bool IsKeyUp(KEY_STRING _szKeyString) const;
	_bool IsKeyDown(KEY_STRING _szKeyString) const;
	_bool IsKeyPressing(KEY_STRING _szKeyString) const;
	_bool IsKeyNone(KEY_STRING _szKeyString) const;

	_bool IsKeyUp(KEY_MACRO _iKeyMacro) const;
	_bool IsKeyDown(KEY_MACRO _iKeyMacro) const;
	_bool IsKeyPressing(KEY_MACRO _iKeyMacro) const;
	_bool IsKeyNone(KEY_MACRO _iKeyMacro) const;

	_bool IsKeyUp(const MOUSEKEYSTATE& _eMouseKey) const;
	_bool IsKeyDown(const MOUSEKEYSTATE& _eMouseKey) const;
	_bool IsKeyPressing(const MOUSEKEYSTATE& _eMouseKey) const;
	_bool IsKeyNone(const MOUSEKEYSTATE& _eMouseKey) const;

	POINT GetDeltaMousePos() const;
	POINT GetCurrentMousePos() const;
	POINT GetOldMousePos() const;

	_vec3 GetDeltaMouseDegree() const;

private:
	virtual void Free(void) override;

private:
	// 윈도우 핸들
	HWND m_hWnd = nullptr;

	// 입력 관련 컴 객체
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	// 키 매핑
	map<KEY_STRING, KEY> m_mapKeyMapping;

	// 키보드 입력 상태 체크
	_byte m_byOldKeyState[UCHAR_MAX + 1] = { 0, };
	_byte m_byCurrentKeyState[UCHAR_MAX + 1] = { 0, };

	// 마우스 입력 상태 체크
	DIMOUSESTATE m_tOldMouseState;
	DIMOUSESTATE m_tCurrentMouseState;
	POINT m_ptOldClientMousePos;
	POINT m_ptCurrentClientMousePos;
};
END