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
		// ���ε� Ű�� ã�Ҵٸ�, �������� �����Ѵ�.
		(*iter).second.first = _iKey;
		return;
	}
		
	// ��Ʈ����ũ�� ���� �� �ִ� �ѵ��� �Ѿ�� �������� �ʴ´�.
	if (m_mapKeyMapping.size() >= sizeof(DWORD) * CHAR_BIT) return;

	// ������� ��Ʈ����ũ�� ����.
	m_mapKeyMapping[_szKeyString] = make_pair(_iKey, 1 << m_mapKeyMapping.size());
}

_bool CKeyMgr::IsKeyUp(KEY_STRING _szKeyString)
{
	const auto& iter = find_if(m_mapKeyMapping.begin(), m_mapKeyMapping.end(), CTag_Finder(_szKeyString));
	if (iter == m_mapKeyMapping.end())
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
		// ���ε� Ű�� ã�� ���ߴٸ� �����Ѵ�.
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
