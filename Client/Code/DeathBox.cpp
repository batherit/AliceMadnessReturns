#include "pch.h"
#include "DeathBox.h"
#include "AliceW.h"

CDeathBox::CDeathBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CDeathBox::CDeathBox(const CDeathBox & rhs)
	:
	CGameObject(rhs)
{
}

CDeathBox::~CDeathBox(void)
{
}

HRESULT CDeathBox::Ready_Object(void)
{

	// Load Colliders
	LoadColliders(L"DeathBox.col");
	// m_pCollider->SetActivated(false);

	return S_OK;
}

int CDeathBox::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CDeathBox::Render_Object(void)
{
}

_bool CDeathBox::LoadColliders(const _tchar* _pFileName)
{
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");

	lstrcat(szDataPath, _pFileName);

	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return false;

	LoadCollidersInfo(hFile);

	CloseHandle(hFile);

	return true;
}

CDeathBox * CDeathBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDeathBox*	pInstance = new CDeathBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CDeathBox::Free(void)
{
	CGameObject::Free();
}