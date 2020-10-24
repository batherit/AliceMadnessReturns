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
	// ������ �ڵ� ���
	m_hWnd = _hWnd;

	// ������ �ʱ� Ŀ�� ��ǥ ���
	m_ptOldClientMousePos = GetClientCursorPoint(_hWnd);

	// DInput �İ�ü�� �����ϴ� �Լ�
	FAILED_CHECK_RETURN(DirectInput8Create(_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL), E_FAIL);

	// Ű���� ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);
	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);
	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pMouse->Acquire();

	return S_OK;
}

void CDirectInputMgr::Update(void)
{
	// ���� ���¸� ���
	memcpy(m_byOldKeyState, m_byCurrentKeyState, sizeof(m_byCurrentKeyState));
	memcpy(&m_tOldMouseState, &m_tCurrentMouseState, sizeof(m_tCurrentMouseState));
	m_ptOldClientMousePos = m_ptCurrentClientMousePos;

	// ���� ���¸� ����
	m_pKeyBoard->GetDeviceState(sizeof(m_byCurrentKeyState), m_byCurrentKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tCurrentMouseState), &m_tCurrentMouseState);
	m_ptCurrentClientMousePos = GetClientCursorPoint(m_hWnd);
}

void CDirectInputMgr::BindKeyStringToKey(KEY_STRING _szKeyString, KEY _iKey)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter != m_mapKeyMapping.end()) {
		// ���ε� Ű�� ã�Ҵٸ�, �������� �����Ѵ�.
		(*iter).second = _iKey;
		return;
	}

	m_mapKeyMapping[_szKeyString] = _iKey;
}

_bool CDirectInputMgr::IsKeyUp(KEY_STRING _szKeyString) const
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
	// ���� ��ȭ�� => �ϵ����
	//return POINT{ m_tCurrentMouseState.lX, m_tCurrentMouseState.lY };

	// Ŭ���̾�Ʈ�� ��ȭ�� => ����Ʈ����
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
	
	// ������ �������� ������ ��!
	Engine::Safe_Release(m_pKeyBoard);
	Engine::Safe_Release(m_pMouse);
	Engine::Safe_Release(m_pInputSDK);
}
