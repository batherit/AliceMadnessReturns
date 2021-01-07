#include "pch.h"
#include "BunnyBomb.h"
#include "AliceW.h"
#include "Map.h"

CBunnyBomb::CBunnyBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBunnyBomb::CBunnyBomb(const CBunnyBomb & rhs)
	:
	CGameObject(rhs)
{
}

CBunnyBomb::~CBunnyBomb(void)
{
}

HRESULT CBunnyBomb::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"BunnyBomb"));
	m_pMesh->Set_AnimationSet(ANIM::ClockworkBomb_ClockWork);
	m_mapComponent[Engine::CDynamicMesh::GetComponentID()].emplace(Engine::CDynamicMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"BunnyBomb.col");
	// m_pCollider->SetActivated(false);

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	//Physics
	m_pPhysics = AddComponent<Engine::CPhysics>();
	m_pPhysics->SetSpeed(5.f);
	m_pPhysics->SetGravity(9.8f * 3.f);
	m_pPhysics->SetResistanceCoefficientXZ(0.8f);

	return S_OK;
}

int CBunnyBomb::Update_Object(const _float & _fDeltaTime)
{
	if (!m_pMap) {
		m_pMap = dynamic_cast<CMap*>(*Engine::GetLayer(L"Environment")->GetLayerList(L"Map").begin());
	}

	if (!IsActivated())
		return 1;

	// 부모 먼저 렌더러에 들어가야 올바르게 자식도 transform 됨.
	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	m_pMesh->Play_Animation(_fDeltaTime);

	Engine::CNaviMesh* pNaviMesh = m_pMap->GetNaviMesh();
	_vec3 vCurrentPos = GetTransform()->GetPos();
	_vec3 vTargetPos = m_pPhysics->GetUpdatedPos(_fDeltaTime);		// 물리 계산
	vTargetPos = pNaviMesh->GetSlidedPos(vTargetPos);

	_vec3 vSettedPos = vTargetPos;

	if (IsFalling(_fDeltaTime)) {
		// 추락 중이라면, 적절한 셀을 찾는다.
		_int iCellIndex = pNaviMesh->GetNaviIndexByPos(vCurrentPos, vTargetPos + _vec3(0.f, -1.0f, 0.f));
		if (-1 != iCellIndex && pNaviMesh->GetCell(iCellIndex)->GetTagIndex() != Engine::CCell::TYPE_SLIDE_EXIT) {
			m_bIsLanded = true;
			/*m_pPhysics->SetGravity(0.f);
			m_pPhysics->SetSpeed(0.f);*/
			m_iCellIndex = iCellIndex;
			//pNaviMesh->Set_NaviIndex(iCellIndex);
			m_pPhysics->SetVelocityY(0.f);
		}
	}
	else if (IsLanded()) {
		m_pPhysics->SetVelocityY(0.f);
		vSettedPos = m_pMap->GetNaviMesh()->Move_OnNaviMesh(m_iCellIndex, &vCurrentPos, &(vTargetPos + _vec3(0.f, -1.0f, 0.f)), GetPhysics());
		if (vSettedPos == vTargetPos + _vec3(0.f, -1.0f, 0.f)) {
			vSettedPos += _vec3(0.f, 1.f, 0.f);
			m_bIsLanded = false;
		}
	}
	// 이동 확정
	GetTransform()->SetPos(vSettedPos);

	return 0;
}

void CBunnyBomb::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_bIsVisible) {
		m_pRenderer->Render();
	}

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CBunnyBomb::LoadColliders(const _tchar* _pFileName)
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

CBunnyBomb * CBunnyBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBunnyBomb*	pInstance = new CBunnyBomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CBunnyBomb::Free(void)
{
	CGameObject::Free();
}

_bool CBunnyBomb::IsFalling(const _float & _fDeltaTime)
{
	return GetPhysics()->GetVelocity().y <= 0.f && !m_bIsLanded;
}
