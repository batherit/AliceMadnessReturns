#include "pch.h"
#include "NaviMesh.h"


CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh & rhs)
	:
	CGameObject(rhs)
{
}

CNaviMesh::~CNaviMesh(void)
{
}


HRESULT CNaviMesh::Ready_Object(void)
{
	m_pRenderer = AddComponent<Engine::CRenderer>();
	m_pManualCol = AddComponent<Engine::CManualCol>();
	return S_OK;
}

int CNaviMesh::Update_Object(const _float & fTimeDelta)
{
	m_pRenderer->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CNaviMesh::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->GetObjectMatrix());
	m_pGraphicDev->SetTexture(0, NULL);
	//Engine::Render_Buffer(Engine::RESOURCE_STATIC, L"M_Buffer_TriCol");
	m_pManualCol->Render_Buffer();
}

CNaviMesh * CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Client::Safe_Release(pInstance);

	return pInstance;
}

void CNaviMesh::Free(void)
{
	CGameObject::Free();
}

void CNaviMesh::PushTriangleVertices(_vec3 _vTriPos1, _vec3 _vTriPos2, _vec3 _vTriPos3)
{
	m_pManualCol->PushTriangleVertices(_vTriPos1, _vTriPos2, _vTriPos3);
}

void CNaviMesh::PopTriangleVertices(_int _iTriangleIndex)
{
	m_pManualCol->PopTriangleVertices(_iTriangleIndex);
}

vector<_vec3>& CNaviMesh::GetNaviVertices()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pManualCol->GetVertices();
}
