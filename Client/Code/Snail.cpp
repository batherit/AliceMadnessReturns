#include "pch.h"
#include "Snail.h"
#include "AliceW.h"
#include "Tooth.h"

CSnail::CSnail(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CSnail::CSnail(const CSnail & rhs)
	:
	CGameObject(rhs)
{
}

CSnail::~CSnail(void)
{
}

HRESULT CSnail::Ready_Object(void)
{
	// Mesh
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(Engine::RESOURCE_STAGE, L"Snail"));
	m_mapComponent[Engine::CStaticMesh::GetComponentID()].emplace(Engine::CStaticMesh::GetComponentTag(), m_pMesh);
	//m_pMesh->Set_AnimationSet(48);

	// Load Colliders
	LoadColliders(L"Snail.col");

	// MeshRenderer
	m_pRenderer = AddComponent<Engine::CMeshRenderer>();
	m_pRenderer->SetRenderInfo(Engine::RENDER_NONALPHA, m_pMesh);

	// Shader
	m_pShader = dynamic_cast<Engine::CShader*>(Engine::Clone(L"Proto_Shader_Mesh"));
	m_mapComponent[Engine::CShader::GetComponentID()].emplace(Engine::CShader::GetComponentTag(), m_pShader);

	return S_OK;
}

int CSnail::Update_Object(const _float & _fDeltaTime)
{
	if (!IsActivated())
		return 1;

	m_pRenderer->Update(_fDeltaTime);
	CGameObject::Update_Object(_fDeltaTime);

	return 0;
}

void CSnail::Render_Object(void)
{
	m_pRenderer->SetWorldMatrix(GetTransform()->GetObjectMatrix());

	if (m_pCullingSphere && Engine::IsSphereCulled(m_pGraphicDev, m_pCullingSphere->GetTransform()->GetPos(), m_pCullingSphere->GetRadiusW()))
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRenderer->Render(m_pShader->Get_EffectHandle());
	
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

_bool CSnail::LoadColliders(const _tchar* _pFileName)
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

void CSnail::OnCollision(Engine::CollisionInfo _tCollisionInfo)
{
	if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_AABB) {
		_int a = 10;
	}
	else if (_tCollisionInfo.pCollidedCollider->GetColliderType() == Engine::TYPE_OBB) {
		_int a = 10;
	}
	else {
		if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"PlayerAttack") == 0) {
			PopTooth(Engine::GetNumberBetweenMinMax(2, 4));
			SetValid(false);
		}
		else if (lstrcmp(_tCollisionInfo.pCollidedCollider->GetColliderTag(), L"Player") == 0) {
			CAliceW* pAlice = dynamic_cast<CAliceW*>(_tCollisionInfo.pCollidedObject);
			if (pAlice && pAlice->IsSliding(0.f)) {
				PopTooth(Engine::GetNumberBetweenMinMax(2, 4));
				SetValid(false);
			}
		}
	}
}

void CSnail::PopTooth(_int _iToothNum, _bool _bIsArrived)
{
	_vec3 vPos = GetTransform()->GetPos();
	CTooth* pTooth = nullptr;
	for (_int i = 0; i < _iToothNum; ++i) {
		pTooth = CTooth::Create(m_pGraphicDev);
		pTooth->GetTransform()->SetPos(vPos);
		pTooth->SetPopInfo(_vec3(
			vPos.x + Engine::GetNumberBetweenMinMax(-2.f, 2.f), 
			vPos.y + 1.f, 
			vPos.z + Engine::GetNumberBetweenMinMax(-2.f, 2.f)), _bIsArrived);
		Engine::GetLayer(L"Environment")->Add_GameObject(pTooth);
	}
}


CSnail * CSnail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSnail*	pInstance = new CSnail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CSnail::Free(void)
{
	CGameObject::Free();
}

