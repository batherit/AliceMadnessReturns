#include "pch.h"
#include "Trigger.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTrigger::CTrigger(const CTrigger & rhs)
	:
	CGameObject(rhs)
{
}

CTrigger::~CTrigger(void)
{
}

HRESULT CTrigger::Ready_Object(void)
{
	Engine::CColliderObject* pCollider = Engine::CColliderObject_AABB::Create(m_pGraphicDev);
	pCollider->SetColliderTag(L"Trigger");
	AddCollider(pCollider);

	m_pCullingSphere = Engine::CColliderObject_Sphere::Create(m_pGraphicDev);
	m_pCullingSphere->SetColliderTag(L"CULL");
	AddCollider(m_pCullingSphere);

	Engine::CCollisionMgr::GetInstance()->AddGameObject(this);

	return S_OK;
}

int CTrigger::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CTrigger::Render_Object(void)
{
}

CTrigger * CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrigger*	pInstance = new CTrigger(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CTrigger::Free(void)
{
	CGameObject::Free();
}

_bool CTrigger::SaveInfo(HANDLE & _hfOut)
{
	DWORD dwByte = 0;

	//// 1) 메시 태그
	//_int iStrLen = lstrlen(m_tcMeshTag);
	//WriteFile(_hfOut, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	//WriteFile(_hfOut, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	//// 2) 렌더 아이디
	//Engine::RENDERID eRenderID = m_pRenderer->GetRenderID();
	//WriteFile(_hfOut, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 1) 트리거 타입 세팅
	WriteFile(_hfOut, &m_eTriggerType, sizeof(m_eTriggerType), &dwByte, nullptr);

	// 2) 트랜스폼 정보 세팅
	_vec3 vPos = GetTransform()->GetPos();
	_vec3 vAngle = GetTransform()->GetAngle();
	_vec3 vScale = GetTransform()->GetScale();
	WriteFile(_hfOut, &vPos, sizeof(vPos), &dwByte, nullptr);
	WriteFile(_hfOut, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	WriteFile(_hfOut, &vScale, sizeof(vScale), &dwByte, nullptr);

	return true;
}

_bool CTrigger::LoadInfo(HANDLE & _hfIn)
{
	DWORD dwByte = 0;

	//// 1) 메시 태그 로드
	//_int iStrLen = 0;
	//ReadFile(_hfIn, &iStrLen, sizeof(iStrLen), &dwByte, nullptr);
	//ReadFile(_hfIn, &m_tcMeshTag, sizeof(_tchar) * (iStrLen + 1), &dwByte, nullptr);

	//// 2) 렌더 아이디
	//Engine::RENDERID eRenderID = Engine::RENDER_END;
	//ReadFile(_hfIn, &eRenderID, sizeof(eRenderID), &dwByte, nullptr);

	// 1) 트리거 타입 세팅
	ReadFile(_hfIn, &m_eTriggerType, sizeof(m_eTriggerType), &dwByte, nullptr);

	// 2) 트랜스폼 정보 세팅
	_vec3 vPos, vAngle, vScale;
	ReadFile(_hfIn, &vPos, sizeof(vPos), &dwByte, nullptr);
	ReadFile(_hfIn, &vAngle, sizeof(vAngle), &dwByte, nullptr);
	ReadFile(_hfIn, &vScale, sizeof(vScale), &dwByte, nullptr);
	GetTransform()->SetPos(vPos);
	GetTransform()->SetAngle(vAngle);
	GetTransform()->SetScale(vScale);

	return true; //SetRenderInfo(m_tcMeshTag, eRenderID);
}

//_bool CTrigger::LoadCollidersInfo()
//{
//	TCHAR szCurPath[MAX_PATH] = L"";
//	TCHAR szDataPath[MAX_PATH] = L"";
//	GetCurrentDirectory(MAX_PATH, szCurPath);
//	PathRemoveFileSpec(szCurPath);
//	PathRemoveFileSpec(szCurPath);
//	PathCombine(szDataPath, szCurPath, L"Resource\\Colliders\\");
//
//	lstrcat(szDataPath, m_tcMeshTag);
//	lstrcat(szDataPath, L".col");
//
//	HANDLE hFile = CreateFileW(szDataPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//		return false;
//
//	CGameObject::LoadCollidersInfo(hFile);
//
//	CloseHandle(hFile);
//	return true;
//}
