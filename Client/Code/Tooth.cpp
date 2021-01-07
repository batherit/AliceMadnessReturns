#include "pch.h"
#include "Tooth.h"
#include "AliceW.h"
#include "Tooth.h"

CTooth::CTooth(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTooth::CTooth(const CTooth & rhs)
	:
	CGameObject(rhs)
{
}

CTooth::~CTooth(void)
{
}

HRESULT CTooth::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Tooth"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Tooth.col");
	m_pCollider = GetColliderFromTag(L"Tooth");
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	m_bIsArrived = true;

	return S_OK;
}

int CTooth::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	if (!m_bIsArrived) {
		_vec3 vToArrivalPos = m_vArrivalPos - GetTransform()->GetPos();

		if (D3DXVec3Length(&vToArrivalPos) <= 0.1f) {
			//m_pCollider->SetActivated(true);
			m_bIsArrived = true;
		}
		else
			GetTransform()->Translate(vToArrivalPos * 0.25f);
	}
	else if (m_pAlice && m_bIsArrived) {
		_vec3 vPlayerPos = m_pAlice->GetTransform()->GetPos();
		vPlayerPos.y += 1.f;
		_vec3 vToPlayer = vPlayerPos - GetTransform()->GetPos();

		if ((m_fElapsedTime += _fDeltaTime) <= 0.3f) {
			_float fT = Engine::GetWeightByValue(m_fElapsedTime, 0.f, 0.25f);
			GetTransform()->Translate(vToPlayer * fT);
		}
		else {
			SetValid(false);
		}
	}

	GetTransform()->RotateByUp(D3DXToRadian(720.f) * _fDeltaTime);


	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CTooth::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render();
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CTooth::LoadColliders(const _tchar* _pFileName)
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

void CTooth::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (!m_pAlice && lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			m_pAlice = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
		}
	}
}

CTooth * CTooth::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTooth*	pInstance = new CTooth(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CTooth::Free(void)
{
	CGameObject::Free();
}

void CTooth::SetPopInfo(_vec3 _vArrivalPos, _bool _bIsArrived)
{
	m_vArrivalPos = _vArrivalPos;
	m_bIsArrived = _bIsArrived;
	//m_pCollider->SetActivated(false);
}
