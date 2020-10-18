#include "KeyMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
}


CKeyMgr::~CKeyMgr()
{
	Release();
}

void CKeyMgr::Update(void)
{
	m_dwOldKeyState = m_dwCurrentKeyState;

	MAPPING mapping;
	for (auto& KeyData : m_mapKeyMapping) {
		mapping = KeyData.second;
		if (IS_PRESS_(mapping.first)) {
			ON_FLAG(m_dwCurrentKeyState, mapping.second);
		}
		else {
			OFF_FLAG(m_dwCurrentKeyState, mapping.second);
		}
	}
}

void CKeyMgr::BindKeyStringToKey(KEY_STRING _szKeyString, KEY _iKey)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter != m_mapKeyMapping.end()) {
		// 매핑된 키를 찾았다면, 변수값을 변경한다.
		(*iter).second.first = _iKey;
		return;
	}
		
	// 비트마스크를 만들 수 있는 한도를 넘어서면 매핑하지 않는다.
	if (m_mapKeyMapping.size() >= sizeof(DWORD) * CHAR_BIT) return;

	// 사이즈로 비트마스크를 취함.
	m_mapKeyMapping[_szKeyString] = make_pair(_iKey, 1 << m_mapKeyMapping.size());
}

_bool CKeyMgr::IsKeyUp(KEY_STRING _szKeyString)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	BITMASK dwBitMask = (*iter).second.second;
	if (IS_ON_FLAG(m_dwOldKeyState, dwBitMask) && !IS_ON_FLAG(m_dwCurrentKeyState, dwBitMask)) 
		return true;

	return false;
}

_bool CKeyMgr::IsKeyDown(KEY_STRING _szKeyString)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	BITMASK dwBitMask = (*iter).second.second;
	if (!IS_ON_FLAG(m_dwOldKeyState, dwBitMask) && IS_ON_FLAG(m_dwCurrentKeyState, dwBitMask)) 
		return true;

	return false;
}

_bool CKeyMgr::IsKeyPressing(KEY_STRING _szKeyString)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	BITMASK dwBitMask = (*iter).second.second;
	if (IS_ON_FLAG(m_dwOldKeyState, dwBitMask) && IS_ON_FLAG(m_dwCurrentKeyState, dwBitMask))
		return true;

	return false;
}

_bool CKeyMgr::IsKeyNone(KEY_STRING _szKeyString)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// 매핑된 키를 찾지 못했다면 종료한다.
		return false;

	BITMASK dwBitMask = (*iter).second.second;
	if (!IS_ON_FLAG(m_dwOldKeyState, dwBitMask) && !IS_ON_FLAG(m_dwCurrentKeyState, dwBitMask))
		return true;

	return false;
}

void CKeyMgr::Free(void)
{
	m_mapKeyMapping.clear();
}
