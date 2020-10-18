#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr final : public CBase
{
	DECLARE_SINGLETON(CKeyMgr)

public:
	typedef const _tchar* KEY_STRING;
	typedef int KEY;
	typedef DWORD BITMASK;
	typedef pair<KEY, BITMASK> MAPPING;

private:
	CKeyMgr();
	~CKeyMgr();

public:
	void Update(void);
	//void Release(void);

public:
	/*bool IsKeyUp(KEY::E_KEY _eKey);
	bool IsKeyDown(KEY::E_KEY _eKey);
	bool IsKeyPressing(KEY::E_KEY _eKey);
	bool IsKeyNone(KEY::E_KEY _eKey);*/
	void BindKeyStringToKey(KEY_STRING _szKeyString, KEY _iKey);

	_bool IsKeyUp(KEY_STRING _szKeyString);
	_bool IsKeyDown(KEY_STRING _szKeyString);
	_bool IsKeyPressing(KEY_STRING _szKeyString);
	_bool IsKeyNone(KEY_STRING _szKeyString);

	void SetOldCursorPoint(POINT& _ptOld) { m_ptOldClickedPoint = _ptOld; }
	POINT GetOldCursorPoint(void) { return m_ptOldClickedPoint; }

private:
// CBase을(를) 통해 상속됨
	virtual void Free(void) override;

private:
	map<KEY_STRING, MAPPING> m_mapKeyMapping;
	DWORD m_dwOldKeyState = 0;
	DWORD m_dwCurrentKeyState = 0;
	POINT m_ptOldClickedPoint{0, 0};
};
END