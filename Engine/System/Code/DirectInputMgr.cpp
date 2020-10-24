#include "DirectInputMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CDirectInputMgr)

CDirectInputMgr::CDirectInputMgr()
{
}


CDirectInputMgr::~CDirectInputMgr()
{
	Free();
}

HRESULT CDirectInputMgr::Ready(HINSTANCE _hInst, HWND _hWnd)
{
	// 윈도우 핸들 얻기
	m_hWnd = _hWnd;

	// 윈도우 초기 커서 좌표 얻기
	m_ptOldClientMousePos = GetClientCursorPoint(_hWnd);

	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);
	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);
	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	return S_OK;
}

void CDirectInputMgr::Update(void)
{
	// 현재 상태를 백업
	memcpy(m_byOldKeyState, m_byCurrentKeyState, sizeof(m_byCurrentKeyState));
	memcpy(&m_tOldMouseState, &m_tCurrentMouseState, sizeof(m_tCurrentMouseState));
	m_ptOldClientMousePos = m_ptCurrentClientMousePos;

	// 현재 상태를 갱신
	m_pKeyBoard->GetDeviceState(sizeof(m_byCurrentKeyState), m_byCurrentKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tCurrentMouseState), &m_tCurrentMouseState);
	m_ptCurrentClientMousePos = GetClientCursorPoint(m_hWnd);
}

void CDirectInputMgr::BindKeyStringToKey(KEY_STRING _szKeyString, KEY _iKey)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter != m_mapKeyMapping.end()) {
		// 매핑된 키를 찾았다면, 변수값을 변경한다.
		(*iter).second = _iKey;
		return;
	}

	m_mapKeyMapping[_szKeyString] = _iKey;
}

_bool CDirectInputMgr::IsKeyUp(KEY_STRING _szKeyString) const
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	KEY key = iter->second;
	if (DI_IS_ON_KEY(m_byOldKeyState, key) && !DI_IS_ON_KEY(m_byCurrentKeyState, key))
		return true;

	return false;
}

_bool CDirectInputMgr::IsKeyDown(KEY_STRING _szKeyString) const
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	KEY key = iter->second;
	if (!DI_IS_ON_KEY(m_byOldKeyState, key) && DI_IS_ON_KEY(m_byCurrentKeyState, key))
		return true;

	return false;
}

_bool CDirectInputMgr::IsKeyPressing(KEY_STRING _szKeyString) const
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	KEY key = iter->second;
	if (DI_IS_ON_KEY(m_byOldKeyState, key) && DI_IS_ON_KEY(m_byCurrentKeyState, key))
		return true;

	return false;
}

_bool CDirectInputMgr::IsKeyNone(KEY_STRING _szKeyString) const
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	KEY key = iter->second;
	if (!DI_IS_ON_KEY(m_byOldKeyState, key) && !DI_IS_ON_KEY(m_byCurrentKeyState, key))
		return true;

	return false;
}

_bool CDirectInputMgr::IsMouseUp(E_MOUSE_BUTTON _eMouseButton) const
{
	if (DI_IS_ON_MB(m_tOldMouseState, _eMouseButton) && !DI_IS_ON_MB(m_tCurrentMouseState, _eMouseButton))
		return true;
	return false;
}

_bool CDirectInputMgr::IsMouseDown(E_MOUSE_BUTTON _eMouseButton) const
{
	if (!DI_IS_ON_MB(m_tOldMouseState, _eMouseButton) && DI_IS_ON_MB(m_tCurrentMouseState, _eMouseButton))
		return true;
	return false;
}

_bool CDirectInputMgr::IsMousePressing(E_MOUSE_BUTTON _eMouseButton) const
{
	if (DI_IS_ON_MB(m_tOldMouseState, _eMouseButton) && DI_IS_ON_MB(m_tCurrentMouseState, _eMouseButton))
		return true;
	return false;
}

_bool CDirectInputMgr::IsMouseNone(E_MOUSE_BUTTON _eMouseButton) const
{
	if (!DI_IS_ON_MB(m_tOldMouseState, _eMouseButton) && !DI_IS_ON_MB(m_tCurrentMouseState, _eMouseButton))
		return true;
	return false;
}

POINT CDirectInputMgr::GetDeltaMousePos() const
{
	// 위상 변화량 => 하드웨어
	//return POINT{ m_tCurrentMouseState.lX, m_tCurrentMouseState.lY };

	// 클라이언트상 변화량 => 소프트웨어
	return POINT{ m_ptCurrentClientMousePos.x - m_ptOldClientMousePos.x, m_ptCurrentClientMousePos.y - m_ptOldClientMousePos.y };
}

POINT CDirectInputMgr::GetCurrentMousePos() const
{
	return m_ptCurrentClientMousePos;
}

POINT CDirectInputMgr::GetOldMousePos() const
{
	return m_ptOldClientMousePos;
}

void CDirectInputMgr::Free(void)
{
	m_mapKeyMapping.clear();
	
	// 생성의 역순으로 해제할 것!
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}
